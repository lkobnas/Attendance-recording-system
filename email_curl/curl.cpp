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
    
    std::string api_key = API_KEY;
    std::string recipient = "eng5220.group38@gmail.com";
    std::string variable_name = "variable_name";
    std::string variable_value = "variable_value";
    std::string command = "curl -X POST https://api.sendgrid.com/v3/mail/send \
                    -H 'Authorization: Bearer " + api_key + "' \
                    -H 'Content-Type: application/json' \
                    -d '{ \
                            \"personalizations\": [ \
                            { \
                                \"to\": [ \
                                { \
                                    \"email\": \"" + recipient + "\" \
                                } \
                                ], \
                            } \
                            ], \
                            \"from\": { \
                            \"email\": \"eng5220.group38@gmail.com\" \
                            }, \
                            \"template_id\": \"d-493c8fdd05074a4fbf7a609f59130b30\" \
                        }'";
    system(command.c_str());
    return 0;
}
