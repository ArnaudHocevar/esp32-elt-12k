#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "device_string.h"

#include <string_view>

TEST_CASE("parse ESP32-C6 debug string") {
    constexpr std::string_view raw =
        "2026.8.2|Chip: ESP32-C6 Features:BLE, 2.4GHz WiFi, Other:0x40 Cores:1 Revision:0|CPU Frequency: 160 MHz|"
        "Framework: ESP-IDF|ESP-IDF: v5.5.5|EFuse MAC: 40:4C:CA:57:FD:98|Reset: Reboot request from esphome.ota|"
        "Wakeup: undefined";

    const auto info = esp32_elt12k::device_string::parse_device_string(raw);

    CHECK(info.valid);
    CHECK(info.esphome_version == "2026.8.2");
    CHECK(info.board_device == "Chip: ESP32-C6 Features:BLE, 2.4GHz WiFi, Other:0x40 Cores:1 Revision:0");
    CHECK(info.framework_version == "ESP-IDF: v5.5.5");
    CHECK(info.cpu_freq == doctest::Approx(160.0F).epsilon(0.001));
}

TEST_CASE("empty and invalid input strings") {
    SUBCASE("empty string") {
        const auto empty_info = esp32_elt12k::device_string::parse_device_string("");
        CHECK_FALSE(empty_info.valid);
    }

    SUBCASE("partial string") {
        const auto partial_info = esp32_elt12k::device_string::parse_device_string("2026.8.2|ESP32|CPU 160MHz");
        CHECK_FALSE(partial_info.valid);
    }
}

TEST_CASE("frequency variant and trimming") {
    constexpr std::string_view raw =
        " 2024.12.0 | esp32-s3-devkitc-1 | CPU Freq: 240 MHz | Framework: arduino | 2.0.14 ";

    const auto info = esp32_elt12k::device_string::parse_device_string(raw);

    CHECK(info.valid);
    CHECK(info.esphome_version == "2024.12.0");
    CHECK(info.board_device == "esp32-s3-devkitc-1");
    CHECK(info.framework_version == "2.0.14");
    CHECK(info.cpu_freq == doctest::Approx(240.0F).epsilon(0.001));
}

TEST_CASE("extract_cpu_frequency valid formats") {
    using esp32_elt12k::device_string::extract_cpu_frequency;

    SUBCASE("standard CPU Frequency with unit") {
        CHECK(extract_cpu_frequency("CPU Frequency: 160 MHz") == doctest::Approx(160.0F).epsilon(0.001));
    }

    SUBCASE("shortened CPU Freq with unit") {
        CHECK(extract_cpu_frequency("CPU Freq: 240 MHz") == doctest::Approx(240.0F).epsilon(0.001));
    }

    SUBCASE("no spaces around value") {
        CHECK(extract_cpu_frequency("Freq:80MHz") == doctest::Approx(80.0F).epsilon(0.001));
    }

    SUBCASE("extra spaces and tabs after colon") {
        CHECK(extract_cpu_frequency("Freq: \t 240 MHz") == doctest::Approx(240.0F).epsilon(0.001));
    }

    SUBCASE("floating point frequency value") {
        CHECK(extract_cpu_frequency("CPU Frequency: 160.5 MHz") == doctest::Approx(160.5F).epsilon(0.001));
    }

    SUBCASE("embedded in longer debug string with earlier colons") {
        constexpr std::string_view raw =
            "Chip: ESP32-C6 Revision:0 | CPU Frequency: 160 MHz | Framework: ESP-IDF";
        CHECK(extract_cpu_frequency(raw) == doctest::Approx(160.0F).epsilon(0.001));
    }
}

TEST_CASE("extract_cpu_frequency invalid or missing indicator") {
    using esp32_elt12k::device_string::extract_cpu_frequency;

    SUBCASE("empty string") {
        CHECK(extract_cpu_frequency("") == 0.0F);
    }

    SUBCASE("missing Freq indicator") {
        CHECK(extract_cpu_frequency("Chip: ESP32-S3 | Clock: 240 MHz") == 0.0F);
    }

    SUBCASE("case sensitive indicator mismatch") {
        CHECK(extract_cpu_frequency("CPU freq: 240 MHz") == 0.0F);
        CHECK(extract_cpu_frequency("CPU FREQ: 240 MHz") == 0.0F);
    }

    SUBCASE("indicator without colon") {
        CHECK(extract_cpu_frequency("CPU Freq 240 MHz") == 0.0F);
    }

    SUBCASE("colon occurs only before indicator") {
        CHECK(extract_cpu_frequency("Header: CPU Freq 240 MHz") == 0.0F);
    }
}

TEST_CASE("extract_cpu_frequency invalid or non-positive values") {
    using esp32_elt12k::device_string::extract_cpu_frequency;

    SUBCASE("non-numeric string after colon") {
        CHECK(extract_cpu_frequency("CPU Freq: invalid") == 0.0F);
        CHECK(extract_cpu_frequency("CPU Freq: N/A MHz") == 0.0F);
    }

    SUBCASE("empty after colon") {
        CHECK(extract_cpu_frequency("CPU Freq:") == 0.0F);
    }

    SUBCASE("only whitespace after colon") {
        CHECK(extract_cpu_frequency("CPU Freq:   \t  ") == 0.0F);
    }

    SUBCASE("zero frequency") {
        CHECK(extract_cpu_frequency("CPU Freq: 0 MHz") == 0.0F);
        CHECK(extract_cpu_frequency("CPU Freq: 0.0 MHz") == 0.0F);
    }

    SUBCASE("negative frequency") {
        CHECK(extract_cpu_frequency("CPU Freq: -160 MHz") == 0.0F);
    }
}
