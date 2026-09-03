#include "device_debug_parser.h"
#include <cstdlib>
#include <cstring>

namespace device_string {
    ParsedDeviceInfo parse_device_string(std::string_view raw) {
        ParsedDeviceInfo info;
        if (raw.empty()) return info;

        std::string_view parts[5];
        size_t count = 0;
        size_t start = 0;

        while (count < 5) {
            size_t end = raw.find('|', start);
            std::string_view token = (end == std::string_view::npos)
                                         ? raw.substr(start)
                                         : raw.substr(start, end - start);

            size_t first = token.find_first_not_of(" ");
            size_t last = token.find_last_not_of(" ");
            if (first != std::string_view::npos && last != std::string_view::npos) {
                token = token.substr(first, (last - first + 1));
            } else {
                token = {};
            }

            parts[count++] = token;
            if (end == std::string_view::npos) break;
            start = end + 1;
        }

        if (count >= 5) {
            info.esphome_version = std::string(parts[0]);
            info.board_device = std::string(parts[1]);
            info.framework_version = std::string(parts[4]);
            info.valid = true;

            const std::string raw_str(raw);
            const char *freq_ptr = strstr(raw_str.c_str(), "Freq:");
            if (freq_ptr != nullptr) {
                freq_ptr += 5;
                char *endptr = nullptr;
                float freq_val = strtof(freq_ptr, &endptr);
                if (freq_val > 0.0f) {
                    info.cpu_freq = freq_val;
                }
            }
        }
        return info;
    }
}
