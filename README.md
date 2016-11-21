## A very simple, header only C++ wrapper for [cURL](https://curl.haxx.se/)

Example usage would be:
```cpp
#include <iostream>
#include "http.hpp"

int main(int argc, char* argv[]) {

	Http http1, http2;

	http1.request("http://google.com", 
		[](CURLcode, const std::string &response) {
		std::cout << "(google.com)response: " << response << std::endl;
	});

	http2.request("http://stackoverflow.com",
		[](CURLcode, const std::string &response) {
		std::cout << "(stackoverflow.com)response: " << response << std::endl;
	});

	std::cout << "hey";

	std::cin.get();
}
```