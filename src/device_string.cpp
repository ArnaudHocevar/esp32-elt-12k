#include "device_string.h"

#include <array>
#include <charconv>
#include <cstddef>
#include <string_view>

namespace esp32_elt12k::device_string {
    namespace {
        [[nodiscard]] constexpr auto trim(std::string_view str) noexcept -> std::string_view {
            const auto first = str.find_first_not_of(" \t\r\n");
            if (first == std::string_view::npos) {
                return {};
            }
            const auto last = str.find_last_not_of(" \t\r\n");
            return str.substr(first, last - first + 1);
        }
    }

    auto parse_device_string(const std::string_view raw) -> ParsedDeviceDetails {
        ParsedDeviceDetails info{};
        if (raw.empty()) {
            return info;
        }

        std::array<std::string_view, 5> parts{};
        std::size_t count = 0;
        std::size_t start = 0;

        while (count < parts.size()) {
            const auto end = raw.find('|', start);
            const auto token = (end == std::string_view::npos)
                                   ? raw.substr(start)
                                   : raw.substr(start, end - start);

            parts[count] = trim(token);
            ++count;
            if (end == std::string_view::npos) {
                break;
            }
            start = end + 1;
        }

        if (count >= parts.size()) {
            info.esphome_version = parts[0];
            info.board_device = parts[1];
            info.framework_version = parts[4];
            info.valid = true;

            constexpr std::string_view freq_indicator = "Freq";
            const auto freq_pos = raw.find(freq_indicator);
            if (freq_pos != std::string_view::npos) {
                const auto colon_pos = raw.find(':', freq_pos);
                if (colon_pos != std::string_view::npos) {
                    auto freq_view = raw.substr(colon_pos + 1);
                    const auto num_start = freq_view.find_first_not_of(" \t");
                    if (num_start != std::string_view::npos) {
                        freq_view = freq_view.substr(num_start);
                        float freq_val{0.0F};
                        const auto [ptr, ec] = std::from_chars(freq_view.data(), freq_view.data() + freq_view.size(),
                                                               freq_val);
                        if (ec == std::errc{} && freq_val > 0.0F) {
                            info.cpu_freq = freq_val;
                        }
                    }
                }
            }
        }

        return info;
    }
}
