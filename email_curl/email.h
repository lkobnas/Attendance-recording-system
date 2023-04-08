#ifndef EMAIL_H
#define EMAIL_H

#include <string>

/// @brief 
class Email{

public:
    /// @brief 
    /// @param recipient 
    /// @param email_type 
    bool send_email(std::string recipient, int email_type);
    bool readCredentialsFile();
private:
    std::string api_key;
    std::string sender ;
    std::string templateID;
};



#endif