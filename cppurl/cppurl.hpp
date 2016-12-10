#include <curl/curl.h>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <stdexcept>
#include <map>
#include <exception>

class CppUrl {
	using ResponseCallback = std::function<void(CURLcode, const std::string&)>;
public:

	class MissingHttpMethodException : std::exception {
	public:
		std::string what() {
			return "No HTTP method used";
		}
	};

	CppUrl() : task(nullptr) {
		if (curlInstanceCount == 0) {
			curl_global_init(CURL_GLOBAL_ALL);
		}
		handle = curl_easy_init();
		if (handle) {
			curlInstanceCount++;
		}
	}

	CppUrl(const CppUrl&) = delete;
	CppUrl& operator= (CppUrl const&);

	~CppUrl() {
		curlInstanceCount--;
		curl_easy_cleanup(handle);
		cleanTask();
		handle = nullptr;
		// <= incase the value fluctuates
		if (curlInstanceCount <= 0) {
			curl_global_cleanup();
		}
	}

	/**
	 * Perform a request with a GET method
	 * @param const std::string &url URL to be requested
	 * @param callback(CURLcode code, const std::string &response) response callback invoked from other thread
	 */
	CppUrl &get(const std::string &url, ResponseCallback callback) {
		waitTask();
		initRequest(url);
		cleanTask();
		task = new std::thread([this, callback] {
			if (handle != nullptr) {
				CURLcode res = curl_easy_perform(handle);
				callback(res, response);
			}
			else {
				// we assume that the request has been cancelled
			}
			this->busy = false;
		});
		return *this;
	}

	/**
	* Perform a request with a POST method
	* @param const std::string &url: URL to be requested
	* @param std::map<std::string, std::string> files: list of pair of file name and file path
	* @param callback(CURLcode code, const std::string &response): response callback invoked from other thread
	*/
	CppUrl &post(const std::string &url, std::map<std::string, std::string> files, ResponseCallback callback) {
		waitTask();
		curl_httppost *formpost = nullptr;
		curl_httppost *lastptr = nullptr;
		curl_slist *headerlist = nullptr;

		for (auto &i : files) {
			curl_formadd(&formpost,
				&lastptr,
				CURLFORM_COPYNAME, i.first.c_str(),
				CURLFORM_FILE, i.second.c_str(),
				CURLFORM_END);
		}

		headerlist = curl_slist_append(headerlist, "Content-Type: multipart/form-data");
		initRequest(url);
		curl_easy_setopt(handle, CURLOPT_HTTPPOST, formpost);
		cleanTask();
		task = new std::thread([this, formpost, headerlist, callback] {
			if (handle != nullptr) {
				CURLcode res = curl_easy_perform(handle);
				callback(res, response);
			}
			else {
				// we assume that the request has been cancelled
			}
			curl_formfree(formpost);
			curl_slist_free_all(headerlist);
			this->busy = false;
		});

		return *this;
	}

	void execute() {
		if (task == nullptr) {
			throw MissingHttpMethodException();
		}
		busy = true;
		task->join();
	}

	void async() {
		if (task == nullptr) {
			throw MissingHttpMethodException();
		}
		busy = true;
		task->detach();
	}

	bool isBusy() {
		return busy;
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

	void waitTask() {
		while (busy) {}
	}

	void cleanTask() {
		if (task != nullptr) {
			delete task;
			task = nullptr;
		}
	}

private:
	CURL *handle;
	std::thread *task;
	std::string response;
	bool busy;
	static int curlInstanceCount;
};

int CppUrl::curlInstanceCount = 0;