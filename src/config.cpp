#include "config.h"

#include "INIParser.h"

/* Section: target */
std::string io_srcfile = "target.cpp";
std::string io_target = "";

/* Section: type */
int type_interval = 5000;
bool fit_view_space_as_tab = false;
bool fit_brackets_autopair = false;
bool fit_space_autofill = false;

#define SET_CONFIG(section, key, default_value) section##_##key = parser.get(#section, #key, (default_value))

/// Functions
void initConfig() {
    INIParser parser("config.ini");

    SET_CONFIG(io, target, io_target);
    SET_CONFIG(io, srcfile, io_srcfile);

    SET_CONFIG(type, interval, 5000);

    /* Fit section: could be overwrite by io_target */
    if (io_target == "acsaber") {
        fit_view_space_as_tab = true;
        fit_brackets_autopair = true;
        fit_space_autofill = true;
    } else if (io_target == "vscode") {
        fit_view_space_as_tab = true;
        fit_brackets_autopair = true;
        fit_space_autofill = true;
    } else if (io_target == "gedit") {
        fit_view_space_as_tab = false;
        fit_brackets_autopair = false;
        fit_space_autofill = true;
    } else if (io_target == "notepad") {
        fit_view_space_as_tab = false;
        fit_brackets_autopair = false;
        fit_space_autofill = false;
    } else {
        SET_CONFIG(fit, view_space_as_tab, false);
        SET_CONFIG(fit, brackets_autopair, false);
        SET_CONFIG(fit, space_autofill, false);
    }
}

void printConfig() {
    std::cout << "[Config]" << std::endl;
    printf("input: %s\n", io_srcfile.c_str());
    printf("target: %s\n", io_target.c_str());
    printf("\tview_space_as_tab: %d\n", fit_view_space_as_tab);
    printf("\tbrackets_autopair: %d\n", fit_brackets_autopair);
    printf("\tspace_autofill: %d\n", fit_space_autofill);
}