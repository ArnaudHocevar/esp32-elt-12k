#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "heartbeat_control.h"

#include <string_view>

using esphome::heartbeat::HeartbeatControl;
using esphome::heartbeat::HeartbeatStatus;

TEST_CASE("HeartbeatControl connection status mapping") {
    CHECK(HeartbeatControl::get_connection_status(true, true) == HeartbeatStatus::kAllGood);
    CHECK(HeartbeatControl::get_connection_status(false, false) == HeartbeatStatus::kTotalNetworkAndModbusFailure);
    CHECK(HeartbeatControl::get_connection_status(false, true) == HeartbeatStatus::kWifiDisconnected);
    CHECK(HeartbeatControl::get_connection_status(true, false) == HeartbeatStatus::kModbusConnectionLost);
}

TEST_CASE("HeartbeatControl brightness by mode") {
    CHECK(HeartbeatControl::get_brightness("read-only", 0) == doctest::Approx(0.8F).epsilon(0.0001F));
    CHECK(HeartbeatControl::get_brightness("peak-shaving", 0) == doctest::Approx(0.5F).epsilon(0.0001F));
    CHECK(HeartbeatControl::get_brightness("self-consumption", 0) == doctest::Approx(0.6F).epsilon(0.0001F));
    CHECK(HeartbeatControl::get_brightness("unknown", 42) == doctest::Approx(1.0F).epsilon(0.0001F));
}

TEST_CASE("HeartbeatControl badge labels") {
    CHECK(HeartbeatControl::get_status_label(HeartbeatStatus::kAllGood, "read-only") == "🟢 All Good [Read-Only]");
    CHECK(HeartbeatControl::get_status_label(HeartbeatStatus::kWifiDisconnected, "peak-shaving") ==
        "🔵 Wi-Fi Disconnected [Peak Shaving]");
    CHECK(HeartbeatControl::get_status_label(HeartbeatStatus::kTotalNetworkAndModbusFailure, "self-consumption") ==
        "🟣 Total Network & Modbus Failure [Self-Consumption]");
    CHECK(HeartbeatControl::get_status_label(HeartbeatStatus::kModbusConnectionLost, "unknown") ==
        "🔴 Modbus Connection Lost (Unknown Mode)");
}
