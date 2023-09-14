#ifndef LOADER_H
#define LOADER_H
#include <cstdlib>
#include "curl/curl.h"
#include <string>

class Loader
{
public:
    Loader();
    void load(const std::string &url, const std::string &filename);
};

#endif // LOADER_H
