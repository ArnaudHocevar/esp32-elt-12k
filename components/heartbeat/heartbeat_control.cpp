#include "heartbeat_control.h"

#include <cmath>

namespace esphome::heartbeat {
    namespace {
        [[nodiscard]] auto triangle_wave(std::uint32_t phase, std::uint32_t cycle_count) noexcept -> float {
            const auto t = static_cast<float>(phase % cycle_count) / static_cast<float>(cycle_count);
            return 1.0F - (2.0F * std::fabs(t - 0.5F));
        }
    } // namespace

    auto HeartbeatControl::get_connection_status(const bool wifi_ok, const bool modbus_ok) -> HeartbeatStatus {
        if (wifi_ok && modbus_ok) {
            return HeartbeatStatus::kAllGood;
        }
        if (!wifi_ok && !modbus_ok) {
            return HeartbeatStatus::kTotalNetworkAndModbusFailure;
        }
        if (!wifi_ok) {
            return HeartbeatStatus::kWifiDisconnected;
        }
        if (!modbus_ok) {
            return HeartbeatStatus::kModbusConnectionLost;
        }
        return HeartbeatStatus::kUnknown;
    }

    auto HeartbeatControl::get_brightness(const std::string_view mode, const std::uint32_t phase) -> float {
        if (mode == "read-only") {
            return 0.8F;
        }

        const auto pulse = triangle_wave(phase, 64U);
        if (mode == "peak-shaving") {
            return 0.5F + (0.5F * pulse);
        }
        if (mode == "self-consumption") {
            return 0.6F + (0.4F * pulse);
        }
        return 1.0F;
    }

    auto HeartbeatControl::get_status_label(const HeartbeatStatus status, const std::string_view mode) -> std::string {
        std::string color_desc;
        switch (status) {
            case HeartbeatStatus::kAllGood:
                color_desc = "🟢 All Good";
                break;
            case HeartbeatStatus::kTotalNetworkAndModbusFailure:
                color_desc = "🟣 Total Network & Modbus Failure";
                break;
            case HeartbeatStatus::kWifiDisconnected:
                color_desc = "🔵 Wi-Fi Disconnected";
                break;
            case HeartbeatStatus::kModbusConnectionLost:
                color_desc = "🔴 Modbus Connection Lost";
                break;
            default:
                color_desc = "⚪ Unknown";
                break;
        }

        std::string mode_desc = " (Unknown Mode)";
        if (mode == "read-only") {
            mode_desc = " [Read-Only]";
        } else if (mode == "peak-shaving") {
            mode_desc = " [Peak Shaving]";
        } else if (mode == "self-consumption") {
            mode_desc = " [Self-Consumption]";
        }

        return color_desc + mode_desc;
    }
} // namespace esphome::heartbeat
