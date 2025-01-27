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
    template<typename T> T get(const std::string& section, const std::string& key, const T& default_value = T()) const {
        auto it = ini_db.find(section);
        if (it != ini_db.end()) {
            auto sect = it->second;
    
            auto sit = sect.find(key);
            if (sit != sect.end()) {
                return convert<T>(sit->second);
            }
        }
        return default_value;
    }

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ini_db;

    static std::string trim(const std::string& str);
    template<typename T> T convert(const std::string& value) const {
        if constexpr (std::is_same<T, int>::value) {
            return std::stoi(value);
        } else if constexpr (std::is_same<T, bool>::value) {
            return (value == "true") || (value == "1");
        } else if constexpr (std::is_same<T, std::string>::value) {
            return value;
        } else {
            throw std::runtime_error("Unsupport type for get()");
        }
    }
};

#endif // INIPARSER_H