#include "INIParser.h"

#include <iostream>
#include <fstream>
#include <type_traits>

bool INIParser::load(const std::string& path) {
    std::ifstream ini_file(path);
    if (!ini_file.is_open()) {
        std::cerr << "[ERR] Unable to open .ini file" << std::endl;
        return false;
    }

    std::string line, cur_section;
    while (std::getline(ini_file, line)) {
        line = trim(line);

        /* Clear Comment & Blank Line */
        const char* comment_chs = "#;";
        if (line.empty() || line[0] == ';' || line[0] == '#') continue;
        size_t comment_start = line.find_first_of(comment_chs);
        if (comment_start != std::string::npos) {
            line = trim(line.substr(0, comment_start));
        }

        /* Parse Section & Key-Value */
        if (line[0] == '[' && line.back() == ']') {
            cur_section = line.substr(1, line.size() - 2);
        } else if (!cur_section.empty()) {
            size_t pos_eql = line.find('=');
            if (pos_eql != std::string::npos) {
                std::string key = trim(line.substr(0, pos_eql));
                std::string val = trim(line.substr(pos_eql + 1));
                ini_db[cur_section][key] = val;
            }
        }
        ///// Parse End
    }
    return true;
}

// template<typename T>
// T INIParser::get(const std::string& section, const std::string& key, const T& default_value) const {
//     auto it = ini_db.find(section);
//     if (it != ini_db.end()) {
//         auto sect = it->second;

//         auto sit = sect.find(key);
//         if (sit != sect.end()) {
//             return convert<T>(sit->second);
//         }
//     }
//     return default_value;
// }

std::string INIParser::trim(const std::string& str) {
    const char* white_space = " \t\n\r";
    size_t start = str.find_first_not_of(white_space);
    size_t end = str.find_last_not_of(white_space);
    return start == std::string::npos ? "" : str.substr(start, end - start + 1);
}

// template<typename T>
// T INIParser::convert(const std::string& value) const {
//     if constexpr (std::is_same<T, int>::value) {
//         return std::stoi(value);
//     } else if constexpr (std::is_same<T, bool>::value) {
//         return (value == "true") || (value == "1");
//     } else if constexpr (std::is_same<T, std::string>::value) {
//         return value;
//     } else {
//         throw std::runtime_error("Unsupport type for get()");
//     }
// }