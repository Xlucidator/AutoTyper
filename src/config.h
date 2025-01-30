#ifndef CONFIG_H
#define CONFIG_H

#include <string>

/* key name traits: <section>_<key> */
extern std::string io_srcfile;
extern std::string io_target;
extern int type_interval;
extern bool fit_view_space_as_tab;
extern bool fit_brackets_autopair;
extern bool fit_brace_nextline_pair;
extern bool fit_space_autofill;

/* Function */
void initConfig();
void printConfig();

#endif // CONFIG_H