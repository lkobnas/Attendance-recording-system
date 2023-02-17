#include <curl/curl.h>
#include <string>

int main()
{
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    std::string sender = "eng5220.group38@gmail.com";
    std::string recipient = "eng5220.group38@gmail.com";
    std::string subject = "Test email from C++";
    std::string body = "This is a test email sent using libcurl from C++.";

    // Initialize curl
    curl = curl_easy_init();
    if(curl) {
        // Set email details
        curl_easy_setopt(curl, CURLOPT_USERNAME, sender.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "Group38_5220");
        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

        // Set recipient email address
        recipients = curl_slist_append(recipients, recipient.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        // Set email details
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, sender.c_str());
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_MAIL_AUTH, (long)CURLAUTH_ANY);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

        // Set email content
        std::string data = "To: " + recipient + "\r\nSubject: " + subject + "\r\n\r\n" + body;
        curl_easy_setopt(curl, CURLOPT_READDATA, data.c_str());
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        // Send the email
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        // Cleanup
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }

    return 0;
}

