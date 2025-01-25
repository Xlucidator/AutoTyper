#ifndef INIPARSER_H
#define INIPARSER_H

#include <iostream>
#include <unordered_map>
#include <string>

class INIParser {
public:
    INIParser() {}
    INIParser(const std::string& path) { load(path); }
    ~INIParser() = default;

    bool load(const std::string& path);
    template<typename T> T get(const std::string& section, const std::string& key, const T& default_value = T()) const;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ini_db;

    static std::string trim(const std::string& str);
    template<typename T> T convert(const std::string& value) const;
};

#endif // INIPARSER_H