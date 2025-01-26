#include "INIParser.h"
#include "AutoTyper.h"
#include "config.h"

#include <iostream>

int main() {
    /*=== Set Configs ===*/
    initConfig();
    
    /*=== AutoTyper ===*/
    AutoTyper typer;
    typer.type(io_srcfile);

    return 0;
}