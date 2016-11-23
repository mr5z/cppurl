#include <curl/curl.h>
#include <string>
#include <thread>
#include <vector>
#include <functional>

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
	 * @param callback(CURLcode, response)
	 */
	void request(const std::string &url, ResponseCallback callback) {
		response = "";
		curl_easy_reset(handle);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &Http::writeCallback);
		curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
		std::thread([this, callback] {
			CURLcode res = curl_easy_perform(handle);
			callback(res, response);
		}).detach();
	}

private:
	static size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *data) {
		data->append((char*)contents, size * nmemb);
		return size * nmemb;
	}
private:
	CURL *handle;
	std::string response;
	static int curlInstanceCount;
};

int Http::curlInstanceCount = 0;