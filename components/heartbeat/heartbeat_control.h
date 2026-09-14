#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace esphome::heartbeat {
    enum class HeartbeatStatus : std::uint8_t {
        kAllGood,
        kTotalNetworkAndModbusFailure,
        kWifiDisconnected,
        kModbusConnectionLost,
        kUnknown,
    };

    class HeartbeatControl {
    public:
        static auto get_connection_status(bool wifi_ok, bool modbus_ok) -> HeartbeatStatus;

        static auto get_brightness(std::string_view mode, std::uint32_t phase) -> float;

        static auto get_status_label(HeartbeatStatus status, std::string_view mode) -> std::string;
    };
} // namespace esphome::heartbeat
