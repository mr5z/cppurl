## A very simple, header only, asynchronous C++ wrapper for [cURL](https://curl.haxx.se/)

An example usage would be:
```cpp
#include <iostream>
#include "http.hpp"

int main(int argc, char* argv[]) {

	Http http1, http2;

	http1.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123", 
		[](CURLcode code, const std::string &response) {
		std::cout << "CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
	}).async();

	std::map<std::string, std::string> files;

	files["image1"] = "C:\\Users\\mr5\\Desktop\\image.png";
	files["image2"] = "C:\\Users\\mr5\\Desktop\\plant1.jpg";

	http2.post("http://smokedetector.esy.es/api/insert-image.php", files,
		[](CURLcode code, const std::string &response) {
		std::cout << "CURLcode: " << code << ", (insert alerts)response: " << response << std::endl;
	}).execute();

	std::cout << "hey";

	std::cin.get();
}
```
