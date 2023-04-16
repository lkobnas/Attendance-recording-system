#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include "email.h"

/// @brief read a file named "credentials.txt" to obtain an API key used for authentication in an email system
/// @return  file was successfully read and the API key was obtained, false otherwise
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
bool Email::send_email_record(std::string recipient,std::string courseName){
    if(!readCredentialsFile()){
        return false;
    }
    sender = "eng5220.group38@gmail.com";
    std::string templateID = "d-493c8fdd05074a4fbf7a609f59130b30";
        
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
                                \"dynamic_template_data\":{\"coursename\":\"" + courseName + "\"} \
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

/// @brief sends an email to a new student using the SendGrid email delivery service
/// @param recipient Recipient email address
/// @return returns true if the email was sent successfully
bool Email::send_email_newStudent(std::string recipient){
    if(!readCredentialsFile()){
        return false;
    }
    sender = "eng5220.group38@gmail.com";
    std::string templateID = "d-7da7739c6a0d44178bc2b56a1eaa1998";
    std::string endpoint = "https://api.sendgrid.com/v3/mail/send";
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

/// @brief send_email_enrollToCourse in the Email class that sends an email to a recipient who has enrolled in a course, using SendGrid API
/// @param recipient Recipient email address
/// @param courseName coursename
/// @param courseDatetime Course dates and times
/// @return returns true if the email is sent successfully, and false otherwise
bool Email::send_email_enrollToCourse(std::string recipient, std::string courseName, std::string courseDatetime){
    if(!readCredentialsFile()){
        return false;
    }
    sender = "eng5220.group38@gmail.com";
    std::string templateID = "d-a9ac7d6a4b5740058ae9d415cebbd5a5";
    std::string endpoint = "https://api.sendgrid.com/v3/mail/send";
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
                                \"dynamic_template_data\":{\"coursename\":\"" + courseName + "\",\"coursedatetime\":\"" + courseDatetime + "\"} \
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

/// @brief send_email_lateReminder for a class Email which sends an email reminder to a recipient who is late for a course
/// @param recipient Recipient email address
/// @param courseName coursename
/// @param courseDatetime Course dates and times
/// @return the constructed command is executed using the system function, and the function returns true.
bool Email::send_email_lateReminder(std::string recipient, std::string courseName, std::string courseDatetime){
    if(!readCredentialsFile()){
        return false;
    }
    sender = "eng5220.group38@gmail.com";
    std::string templateID = "d-8d6bfc995f8c403f8515e2745497f923";
    std::string endpoint = "https://api.sendgrid.com/v3/mail/send";
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
                                \"dynamic_template_data\":{\"coursename\":\"" + courseName + "\",\"coursedatetime\":\"" + courseDatetime + "\"} \
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