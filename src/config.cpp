#include "config.h"

#include "INIParser.h"

/* Section: target */
std::string io_basedir = "";
std::string io_srcfile = "target.cpp";
std::string io_target = "";

/* Section: type */
int type_interval = 5000;
bool type_hide_window = false;

/* Section: fit */
bool fit_view_space_as_tab = false;
bool fit_brackets_autopair = false;
bool fit_brace_nextline_pair = false;
bool fit_space_autofill = false;

#define SET_CONFIG(section, key, default_value) section##_##key = parser.get(#section, #key, (default_value))

/// Functions
void initConfig() {
    INIParser parser("config.ini");

    SET_CONFIG(io, basedir, io_basedir);
    SET_CONFIG(io, target, io_target);
    SET_CONFIG(io, srcfile, io_srcfile);

    SET_CONFIG(type, interval, 5000);
    SET_CONFIG(type, hide_window, false);

    /* Fit section: could be overwrite by io_target */
    if (io_target == "acsaber") {
        fit_view_space_as_tab = true;
        fit_brackets_autopair = true;
        fit_brace_nextline_pair = true;
        fit_space_autofill = true;
    } else if (io_target == "vscode") {
        fit_view_space_as_tab = true;
        fit_brackets_autopair = true;
        fit_brace_nextline_pair = false;
        fit_space_autofill = true;
    } else if (io_target == "gedit") {
        fit_view_space_as_tab = false;
        fit_brackets_autopair = false;
        fit_brace_nextline_pair = false;
        fit_space_autofill = true;
    } else if (io_target == "notepad") {
        fit_view_space_as_tab = false;
        fit_brackets_autopair = false;
        fit_brace_nextline_pair = false;
        fit_space_autofill = false;
    } else {
        SET_CONFIG(fit, view_space_as_tab, false);
        SET_CONFIG(fit, brackets_autopair, false);
        SET_CONFIG(fit, brace_nextline_pair, false);
        SET_CONFIG(fit, space_autofill, false);
    }
}

void printConfig() {
    std::cout << "[Config]" << std::endl;
    
    printf("input: %s\n", io_srcfile.c_str());
    printf("target: %s\n", io_target.c_str());

    printf("\tview_space_as_tab: %d\n", fit_view_space_as_tab);
    printf("\tbrackets_autopair: %d\n", fit_brackets_autopair);
    printf("\tbrace_nextline_pair: %d\n", fit_brace_nextline_pair);
    printf("\tspace_autofill: %d\n", fit_space_autofill);
}