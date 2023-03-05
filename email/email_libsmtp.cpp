#include <iostream>
#include <string>
#include <cstdlib>
#include <libsmtp/libsmtp.h>

int main()
{
    const char* server = "smtp.gmail.com";
    int port = 587;
    const char* username = "";
    const char* password = "";
    const char* from = "eng5220.group38@gmail.com";
    const char* to = "eng5220.group38@gmail.com";
    const char* subject = "Test email from Raspberry Pi";
    const char* body = "This is a test email sent from a Raspberry Pi using C++.";

    try {
        SmtpClient smtp(server, port);
        smtp.authenticate(username, password, SmtpClient::AuthMethod::LOGIN);
        smtp.setFrom(from);
        smtp.addTo(to);
        smtp.setSubject(subject);
        smtp.setBody(body);
        smtp.send();
        std::cout << "Email sent successfully." << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}

