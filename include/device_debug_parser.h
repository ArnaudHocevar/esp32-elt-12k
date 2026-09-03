#ifndef ESP32_ELT_12K_DEVICE_DEBUG_PARSER_H
#define ESP32_ELT_12K_DEVICE_DEBUG_PARSER_H

#endif //ESP32_ELT_12K_DEVICE_DEBUG_PARSER_H
#pragma once

#include <string>
#include <string_view>

namespace device_string {
    struct ParsedDeviceInfo {
        std::string esphome_version;
        std::string board_device;
        std::string framework_version;
        float cpu_freq{0.0f};
        bool valid{false};
    };

    ParsedDeviceInfo parse_device_string(std::string_view raw);
}
