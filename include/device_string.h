#pragma once

#include <string>
#include <string_view>

namespace esp32_elt12k::device_string {
    struct ParsedDeviceDetails {
        std::string esphome_version;
        std::string board_device;
        std::string framework_version;
        float cpu_freq{0.0F};
        bool valid{false};
    };

    [[nodiscard]] auto extract_cpu_frequency(std::string_view raw) -> float;
    [[nodiscard]] auto parse_device_string(std::string_view raw) -> ParsedDeviceDetails;
}
