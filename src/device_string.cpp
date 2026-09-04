#include "device_string.h"

#include <array>
#include <charconv>
#include <cstddef>
#include <string>
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

        [[nodiscard]] auto parse_board_device(std::string_view board_info) -> std::string {
            board_info = trim(board_info);
            if (board_info.empty()) {
                return {};
            }

            constexpr std::string_view chip_prefix = "Chip:";
            constexpr std::string_view revision_key = "Revision:";
            constexpr std::string_view cores_key = "Cores:";

            if (board_info.starts_with(chip_prefix)) {
                return std::string{board_info};
            }

            auto chip_name = trim(board_info.substr(chip_prefix.size()));
            if (const auto features_pos = chip_name.find("Features:"); features_pos != std::string_view::npos) {
                chip_name = trim(chip_name.substr(0, features_pos));
            }

            const auto parse_uint_field = [&](const std::string_view key) -> std::string_view {
                const auto key_pos = board_info.find(key);
                if (key_pos == std::string_view::npos) {
                    return {};
                }
                auto value = board_info.substr(key_pos + key.size());
                value = trim(value);
                const auto end = value.find_first_not_of("0123456789");
                return (end == std::string_view::npos) ? value : value.substr(0, end);
            };

            const auto revision = parse_uint_field(revision_key);
            const auto cores = parse_uint_field(cores_key);
            if (chip_name.empty() || revision.empty() || cores.empty()) {
                return std::string{board_info};
            }

            return std::string{chip_name} + " (rev: " + std::string{revision} + ", " + std::string{cores} + " CPU)";
        }
    }

    auto extract_cpu_frequency(const std::string_view raw) -> float {
        constexpr std::string_view freq_indicator = "Freq";
        if (const auto freq_pos = raw.find(freq_indicator); freq_pos != std::string_view::npos) {
            if (const auto colon_pos = raw.find(':', freq_pos); colon_pos != std::string_view::npos) {
                auto freq_view = raw.substr(colon_pos + 1);
                if (const auto num_start = freq_view.find_first_not_of(" \t");
                    num_start != std::string_view::npos) {
                    freq_view = freq_view.substr(num_start);
                    float freq_val{0.0F};
                    const auto [ptr, ec] = std::from_chars(freq_view.data(), freq_view.data() + freq_view.size(),
                                                           freq_val);
                    if (ec == std::errc{} && freq_val > 0.0F) {
                        return freq_val;
                    }
                }
            }
        }
        return 0.0F;
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
            info.board_device = parse_board_device(parts[1]);
            info.framework_version = parts[4];
            info.cpu_freq = extract_cpu_frequency(raw);
            info.valid = true;
        }

        return info;
    }
}
