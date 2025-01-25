#include "config.h"

#include "INIParser.h"

/* Section: target */
std::string io_srcfile = "target.cpp";

/* Section: type */
int type_interval = 5000;

/// Functions
void initConfig() {
    INIParser parser("config.ini");

    io_srcfile = parser.get("io", "srcfile", io_srcfile);
    type_interval = parser.get("type", "interval", 5000);
}