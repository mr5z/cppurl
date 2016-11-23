#include <iostream>
#include "cppurl.hpp"

int main(int argc, char* argv[]) {

	Http http1, http2;

	http1.request("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123", 
		[](CURLcode code, const std::string &response) {
		std::cout << "CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
	});

	http2.request("http://smokedetector.esy.es/api/query-alerts.php",
		[](CURLcode code, const std::string &response) {
		std::cout << "CURLcode: " << code << ", (query alerts)response: " << response << std::endl;
	});

	std::cout << "hey";

	std::cin.get();
}