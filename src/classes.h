#include <vector>
#include <string>
#include <map>

namespace {
    struct dictionary {
        std::vector<int8_t> dummy;
        std::vector<int16_t> dummy2;
        std::vector<uint16_t> dummy3;
        std::vector<std::vector<uint16_t>> dummy4;
        std::vector<std::map<std::string, bool>> dummy5;
        std::pair<std::string, bool> dummy6;
    };
}
