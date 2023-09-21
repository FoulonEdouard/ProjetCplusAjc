#include "loader.h"
#include "curl/curl.h"
#include"curl/easy.h"


Loader::Loader()
{

}
/**
 * @fn static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
 * fonction pour écrire la taille des données que l'on récupère avec le load\n
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/**
 * @fn void Loader::load(const std::string &url)
 * La fonction utilisant la librairie CURL pour aller récupéré nos données sur un url ici https et les enregistrer sur le readBuffer)\n
 * @param url
 * Correspond à notre Url.\n
 */
void Loader::load(const std::string &url)
{
        CURL *curl = curl_easy_init();

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL,url.c_str());
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            CURLcode res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl);
        }
    }


