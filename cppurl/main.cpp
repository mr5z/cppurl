#include <iostream>
#include "cppurl.hpp"

int main(int argc, char* argv[]) {

	CppUrl http1, http2;

	http1.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123",
		[](CURLcode code, const std::string &response) {
		std::cout << "1. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
    }).async();
	http2.get("http://google.com",
		[](CURLcode code, const std::string &response) {
		std::cout << "2. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
	}).async();
	http1.get("http://google.com",
		[](CURLcode code, const std::string &response) {
		std::cout << "3. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
	}).async();
	http1.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123",
		[](CURLcode code, const std::string &response) {
		std::cout << "4. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
	}).async();
	http1.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123",
		[](CURLcode code, const std::string &response) {
		std::cout << "5. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
	}).async();
	http1.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123",
		[](CURLcode code, const std::string &response) {
		std::cout << "6. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
	}).async();
	http2.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123",
		[](CURLcode code, const std::string &response) {
		std::cout << "7. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
	}).execute();
	http2.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123",
		[](CURLcode code, const std::string &response) {
		std::cout << "8. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
	}).async();

	std::map<std::string, std::string> files;

	files["image1"] = "C:\\Users\\mr5\\Desktop\\a.png";
	files["image2"] = "C:\\Users\\mr5\\Desktop\\b.png";

	http2.post("http://smokedetector.esy.es/api/insert-alert.php", files,
		[](CURLcode code, const std::string &response) {
		std::cout << "CURLcode: " << code << ", (insert alerts)response: " << response << std::endl;
	}).async();

	std::cout << "hey\n";

	std::cin.get();
}