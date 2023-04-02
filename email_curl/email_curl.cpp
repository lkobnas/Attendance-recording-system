#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>

int main() {

    std::ifstream file("credentials.txt");
    std::string API_KEY;
    if (file.is_open()) {
        std::getline(file, API_KEY);
        file.close();
    } else {
        std::cerr << "Unable to open file." << std::endl;
    }
    //int exitStatus = std::system(API_KEY.c_str());

    //Send email here
    
    std::string command = "curl --request POST --url https://api.sendgrid.com/v3/mail/send --header 'authorization: Bearer $SENDGRID_API_KEY' --header 'Content-Type: application/json' --data '{\"personalizations\": [{\"to\": [{\"email\": \"eng5220.group38@gmail.com\"}]}],\"from\": {\"email\": \"no-reply@punchin.com\"}, \"template_id\" : \"d-493c8fdd05074a4fbf7a609f59130b30\"}' ";
    std::system(command.c_str());

    return 0;
}