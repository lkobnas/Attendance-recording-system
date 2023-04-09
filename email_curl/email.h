#ifndef EMAIL_H
#define EMAIL_H

#include <string>

/// @brief 
class Email{

public:
    /// @brief 
    /// @param recipient 
    /// @param email_type 
    bool send_email_record(std::string recipient, std::string courseName);
    bool send_email_newStudent(std::string recipient);
    bool send_email_enrollToCourse(std::string recipient, std::string courseName, std::string courseDatetime);
    bool send_email_lateReminder(std::string recipient, std::string courseName, std::string courseDatetime);
    bool readCredentialsFile();
private:
    std::string api_key;
    std::string sender ;
    std::string templateID;
};



#endif