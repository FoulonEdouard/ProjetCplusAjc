#include <cstdlib>
#include <curl/curl.h>
#include <string>
#include<loader.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char** argv) {

    Loader loader;
    loader.load("https://www.data.gouv.fr/fr/datasets/r/ecb94e56-22d4-40ee-bf40-c7890569d5bd","/home/edouard/projet_linux_embarque/data.JSON");

    return (EXIT_SUCCESS);
}
