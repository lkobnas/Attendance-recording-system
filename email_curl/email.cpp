#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include "email.h"

bool Email::readCredentialsFile(){
    std::ifstream file("credentials.txt");
    std::string API_KEY;
    if (file.is_open()) {
        std::getline(file, API_KEY);
        file.close();
    } else {
        std::cerr << "Unable to open credentials file." << std::endl;
        return false;
    }

    api_key = API_KEY;

    if(api_key==""){
        return false;
    }

    return true;
}

/// @brief Send email using SendGrid with pre-designed dynamic templates
/// @param recipient Recipient email address
/// @param email_type 1: send attendance recorded email, 2: send new account confirmation email
bool Email::send_email(std::string recipient, int email_type){
    if(!readCredentialsFile()){
        return false;
    }
    //recipient = "eng5220.group38@gmail.com";
    sender = "eng5220.group38@gmail.com";
    if(email_type==1){
        templateID = "d-493c8fdd05074a4fbf7a609f59130b30";
    }else{
        templateID = "d-7da7739c6a0d44178bc2b56a1eaa1998";
    }
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
                            \"email\": \"" + sender + "\" \
                            }, \
                            \"template_id\": \"" + templateID + "\" \
                        }'";
    system(command.c_str());
    return true;  
}
