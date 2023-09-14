#include "loader.h"

Loader::Loader()
{

}

void Loader::load(const std::string &url, const std::string &filename)
{
        CURL *curl = curl_easy_init();

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            FILE * output = fopen(filename.c_str(), "w+");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, output);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            }
            fclose(output);
            curl_easy_cleanup(curl);
        }
    }


