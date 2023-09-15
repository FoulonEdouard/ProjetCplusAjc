#ifndef LOADER_H
#define LOADER_H
#include <cstdlib>
#include "curl/curl.h"
#include <string>
#include"gd.h"

class Loader
{
public:
    Loader();
    void load(const std::string &url);
    std::string readBuffer;
};

#endif // LOADER_H
