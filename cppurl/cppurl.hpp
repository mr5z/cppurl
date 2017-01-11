#include <curl/curl.h>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <stdexcept>
#include <map>
#include <exception>
#include <atomic>

class CppUrl {
    using ResponseCallback = std::function<void(CURLcode, const std::string&)>;
public:

    class MissingHttpMethodException : public std::exception {
    public:
        virtual const char* what() const override {
            return "No HTTP method used";
        }
    };

    CppUrl() : task(nullptr) {
        static std::once_flag curlInitialized;
        std::call_once(curlInitialized, [] {
            curl_global_init(CURL_GLOBAL_ALL);
            std::atexit([] {
                curl_global_cleanup();
            });
        });
        handle = curl_easy_init();
    }

    CppUrl(const CppUrl&) = delete;
    CppUrl& operator= (CppUrl const&) = delete;

    ~CppUrl() {
        curl_easy_cleanup(handle);
        cleanTask();
        handle = nullptr;
    }

    /**
     * Perform a request with a GET method
     * @param const std::string &url URL to be requested
     * @param callback(CURLcode code, const std::string &response) response callback invoked from other thread
     */
    CppUrl& get(const std::string &url, ResponseCallback callback) {
        waitTask();
        this->callback = callback;
        initRequest(url);
        cleanTask();
        return *this;
    }

    /**
    * Perform a request with a POST method
    * @param const std::string &url: URL to be requested
    * @param std::map<std::string, std::string> files: list of pair of file name and file path
    * @param callback(CURLcode code, const std::string &response): response callback invoked from other thread
    */
    CppUrl& post(const std::string &url, std::map<std::string, std::string> files, ResponseCallback callback) {
        waitTask();
        this->callback = callback;
        setupPostHeader(files);
        initRequest(url);
        curl_easy_setopt(handle, CURLOPT_HTTPPOST, formpost);
        cleanTask();
        return *this;
    }

    void execute() {
        fireTask();
    }

    void async() {
        task = new std::thread([this] {
            this->fireTask();
        });
    }

    bool isBusy() {
        return busy.load();
    }

private:
    static size_t writeCallback(char *contents, size_t size, size_t nmemb, std::string *data) {
        data->append(contents, size * nmemb);
        return size * nmemb;
    }

    void initRequest(const std::string &url) {
        response = "";
        curl_easy_reset(handle);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &CppUrl::writeCallback);
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    }

    void setupPostHeader(std::map<std::string, std::string> files) {
        curl_httppost *lastptr = nullptr;
        formpost = nullptr;
        headerlist = nullptr;
        for (auto &i : files) {
            curl_formadd(&formpost,
                &lastptr,
                CURLFORM_COPYNAME, i.first.c_str(),
                CURLFORM_FILE, i.second.c_str(),
                CURLFORM_END);
        }
        headerlist = curl_slist_append(headerlist, "Content-Type: multipart/form-data");
    }

    void waitTask() {
        while (busy.load()) {}
    }

    void cleanTask() {
        if (task != nullptr) {
            task->join(); // wait for it
            delete task;
            task = nullptr;
        }
    }

    void fireTask() {
        busy.store(true);
        CURLcode res = curl_easy_perform(handle);
        this->callback(res, this->response);
        curl_formfree(formpost);
        curl_slist_free_all(headerlist);
        busy.store(false);
    }

private:
    CURL *handle;
    std::thread *task;
    std::string response;
    std::atomic<bool> busy;

    curl_httppost *formpost;
    curl_slist *headerlist;
    ResponseCallback callback;
};