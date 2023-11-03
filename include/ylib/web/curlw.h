#pragma once

#include <string>
#include <ylib/core/lang.h>
#include <curl/curl.h>

using namespace std;

namespace ylib::web::curlw {

class CurlHandle {
private:
    CURL *curl;
    
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
        size_t total_size = size * nmemb;
        output->append(static_cast<char *>(contents), total_size);
        return total_size;
    }

public:
    CurlHandle() {
        curl = curl_easy_init();
        if (curl == NULL) {
            throw Exception("Could not initialize CURL handle.");
        }
        // By default, let's set to follow redirect.
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    }
    
    CURLcode perform(const char *url) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        return curl_easy_perform(curl);
    }
    
    void performOk(const char *url) {
        auto err = perform(url);
        if (err != CURLE_OK) {
            auto msg = curl_easy_strerror(err);
            throw Exception(sfput("Error while performing for url {}. Error: {}.", url, msg));
        }
        
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        
        if (!(http_code >= 200 && http_code < 300)) {
            throw Exception(sfput("Not an 2xx response code. Got: {}", http_code));
        }
    }
    
    string httpGetOk(const char *url) {
        string response;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        performOk(url);
        return response;
    }
    
    ~CurlHandle() {
        if (curl) {
            curl_easy_cleanup(curl);
        }
    }
};

namespace http {
string get(const char *url) {
    CurlHandle handle;
    return handle.httpGetOk(url);
}
}
}
