#define CURL_STATICLIB
#include <iostream>
#include <curl/curl.h>
#include <json/json.h>
#include <string>

#ifdef _DEBUG
#pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment (lib, "curl/libcurl_a.lib")
#endif
#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "advapi32.lib")

size_t WriteCallBack(void* contents, size_t size, size_t nmemb, std::string* buffer)
{
    size_t totalSize = size * nmemb;
    buffer->append((char*)contents, totalSize);
    return totalSize;
}

int main()
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.coingecko.com/api/v3/simple/price?ids=ethereum&vs_currencies=usd");

        // capture response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            std::cerr << "Request handler failed" << curl_easy_strerror(res) << std::endl;
        }
        else
        {
            Json::Value jsonData;
            Json::CharReaderBuilder builder;
            std::string errors;
            std::istringstream readBufferStream(readBuffer);
            if (Json::parseFromStream(builder, readBufferStream, &jsonData, &errors))
            {
                std::cout << "Price of ETH: " << jsonData["ethereum"]["usd"].asFloat();
            }
            else
            {
                std::cerr << "JSON parser failed" << errors << std::endl;
            }
        }
        curl_easy_cleanup(curl);
    }
    return 0;
}