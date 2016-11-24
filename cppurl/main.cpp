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

	Http http1, http2;

	http1.get("http://smokedetector.esy.es/api/update-alerts.php?alertIds=123123,123123", 
		[](CURLcode code, const std::string &response) {
		std::cout << "CURLcode: " << code << ", (update alerts)response: " << response << std::endl;
	});

	std::map<std::string, std::string> files;

	files["image1"] = "C:\\Users\\mr5\\Desktop\\image.png";
	files["image2"] = "C:\\Users\\mr5\\Desktop\\plant1.jpg";

	http2.post("http://smokedetector.esy.es/api/insert-image.php", files,
		[](CURLcode code, const std::string &response) {
		std::cout << "CURLcode: " << code << ", (insert alerts)response: " << response << std::endl;
	});

	std::cout << "hey";

	std::cin.get();
}