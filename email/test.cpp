#include <curl/curl.h>
#include <openssl/ssl.h>
#include <fstream>
#include <string>
#include <iostream>

int main()
{
    CURL *curl;
    CURLcode res = CURLE_OK;
    curl = curl_easy_init();

    std::ifstream file("credentials.txt");
    std::string username, password;
    if (file.is_open()) {
        std::getline(file, username);
        std::getline(file, password);
        file.close();
    } else {
        std::cerr << "Unable to open file." << std::endl;
    }

    if (curl) {
        struct curl_slist *recipients = NULL;
        recipients = curl_slist_append(recipients, "eng5220.group38@gmail.com");
        //recipients = curl_slist_append(recipients, "recipient2@example.com");
        curl_easy_setopt(curl, CURLOPT_URL, "smtp.sendgrid.net:587");
        curl_easy_setopt(curl, CURLOPT_USERNAME, "apikey");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "apikey");
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "eng5220.group38@gmail.com");
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        const char *header[] = {
            "Content-Type: text/plain",
            "charset=UTF-8",
            "Content-Transfer-Encoding: 8bit",
            NULL
        };
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
        const char *body[] = {
            "Subject: Test Email\n",
            "To: recipient1@example.com, recipient2@example.com\n",
            "\n",
            "This is a test email sent using C++ and SendGrid!\n",
            NULL
        };
        curl_easy_setopt(curl, CURLOPT_READDATA, body);
        res = curl_easy_perform(curl);
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n",
               curl_easy_strerror(res));
    }
    return 0;
}
