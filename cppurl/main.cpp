#include <iostream>
#include "cppurl.hpp"

/* <DESC>
* HTTP Multipart formpost with file upload and two additional parts.
* </DESC>
*/
/* Example code that uploads a file name 'foo' to a remote script that accepts
* "HTML form based" (as described in RFC1738) uploads using HTTP POST.
*
* The imaginary form we'll fill in looks like:
*
* <form method="post" enctype="multipart/form-data" action="examplepost.cgi">
* Enter file: <input type="file" name="sendfile" size="40">
* Enter file name: <input type="text" name="filename" size="30">
* <input type="submit" value="send" name="submit">
* </form>
*
* This exact source code has not been verified to work.
*/

int main(int argc, char* argv[]) {

	CppUrl http1, http2;

	std::thread([&http1, &http2] {

		http1.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123",
			[](CURLcode code, const std::string &response) {
			std::cout << "1. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
		}).async();
		http2.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123",
			[](CURLcode code, const std::string &response) {
			std::cout << "2. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
		}).async();
		http1.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123",
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
		http1.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123",
			[](CURLcode code, const std::string &response) {
			std::cout << "7. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
		}).async();
		http2.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123",
			[](CURLcode code, const std::string &response) {
			std::cout << "8. CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
		}).async();

		//std::map<std::string, std::string> files;

		//files["image1"] = "C:\\Users\\mr5\\Desktop\\baby.jpg";
		//files["image2"] = "C:\\Users\\mr5\\Desktop\\baby2.jpg";

		//http2.post("http://smokedetector.esy.es/api/insert-alert.php", files,
		//	[](CURLcode code, const std::string &response) {
		//	std::cout << "CURLcode: " << code << ", (insert alerts)response: " << response << std::endl;
		//}).async();

	}).join();

	std::cout << "hey";

	std::cin.get();
}