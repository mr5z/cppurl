#include <sys/stat.h>
#include <curl/curl.h>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <stdexcept>
#include <map>

class Http {
	using ResponseCallback = std::function<void(CURLcode, const std::string&)>;
public:
	Http() {
		if (curlInstanceCount == 0) {
			curl_global_init(CURL_GLOBAL_ALL);
		}
		handle = curl_easy_init();
		if (handle) {
			curlInstanceCount++;
		}
	}

	~Http() {
		curlInstanceCount--;
		curl_easy_cleanup(handle);
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
	void get(const std::string &url, ResponseCallback callback) {
		initRequest(url);
		std::thread([this, callback] {
			CURLcode res = curl_easy_perform(handle);
			callback(res, response);
		}).detach();
	}

	/**
	* Perform a request with a POST method
	* @param const std::string &url: URL to be requested
	* @param std::map<std::string, std::string> files: list of pair of file name and file path
	* @param callback(CURLcode code, const std::string &response): response callback invoked from other thread
	*/
	void post(const std::string &url, std::map<std::string, std::string> files, ResponseCallback callback) {
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
		std::thread([this, formpost, headerlist, callback] {
			CURLcode res = curl_easy_perform(handle);
			callback(res, response);
			curl_formfree(formpost);
			curl_slist_free_all(headerlist);
		}).detach();

	}

private:
	static size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *data) {
		data->append((char*)contents, size * nmemb);
		return size * nmemb;
	}

	void initRequest(const std::string &url) {
		response = "";
		curl_easy_reset(handle);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &Http::writeCallback);
		curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	}

private:
	CURL *handle;
	std::string response;
	static int curlInstanceCount;
};

int Http::curlInstanceCount = 0;