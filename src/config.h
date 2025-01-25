#ifndef CONFIG_H
#define CONFIG_H

#include <string>

/* key name traits: <section>_<key> */
extern std::string io_srcfile;
extern int type_interval;

/* Function */
void initConfig();

#endif // CONFIG_H