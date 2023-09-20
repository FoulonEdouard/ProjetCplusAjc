#ifndef LOADER_H
#define LOADER_H
#include <cstdlib>
#include "curl/curl.h"
#include <string>

/**
 * @class Loader class
 * Il s'agit de la classe qui va nous permettre de récupérer via l'URL un fichier JSON
 */
class Loader
{
public:
    /**
     * @fn Loarder()
     * @brief Il s'agit du constructeur de Loader
     */
    Loader();
    /**
     * @fn void load(const std::string &url)
     * @param const std::string &url va demander l'url en paramètre
     */
    void load(const std::string &url);
    /**
     * @brief on va créer un readBuffer qui va récupérer l'information du JSON
     */
    std::string readBuffer;
};

#endif // LOADER_H
