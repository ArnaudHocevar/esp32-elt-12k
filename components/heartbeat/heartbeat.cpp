#include "heartbeat.h"

#include "heartbeat_control.h"

#include "esphome/components/wifi/wifi_component.h"
#include "esphome/core/log.h"

namespace esphome::heartbeat {
    namespace {
        constexpr auto TAG = "heartbeat";
    } // namespace

    void HeartbeatComponent::setup() {
        this->phase_ = 0;
        this->badge_throttle_ = 0;

        if (this->heartbeat_led_ != nullptr) {
            auto call = this->heartbeat_led_->turn_off();
            call.perform();
        }
        if (this->heartbeat_status_ != nullptr) {
            this->heartbeat_status_->publish_state("initializing");
        }
    }

    void HeartbeatComponent::update() {
        this->update_status();
    }

    void HeartbeatComponent::dump_config() {
        ESP_LOGCONFIG(TAG, "Heartbeat controller");
        ESP_LOGCONFIG(TAG, "  Modbus connection status: %s",
                      this->modbus_cx_status_ != nullptr ? "configured" : "missing");
        ESP_LOGCONFIG(TAG, "  Work mode selector: %s",
                      this->select_work_mode_ != nullptr ? "configured" : "missing");
        if (this->heartbeat_led_ != nullptr) {
            ESP_LOGCONFIG(TAG, "  Light state: configured");
        }
        if (this->heartbeat_status_ != nullptr) {
            ESP_LOGCONFIG(TAG, "  Text sensor: configured");
        }
    }

    void HeartbeatComponent::update_status() {
        if (this->modbus_cx_status_ == nullptr || this->select_work_mode_ == nullptr) {
            return;
        }

        const bool wifi_ok{
            wifi::global_wifi_component != nullptr &&
            wifi::global_wifi_component->is_connected()
        };
        const bool modbus_ok = this->modbus_cx_status_->state;
        const auto connection_status = HeartbeatControl::get_connection_status(wifi_ok, modbus_ok);

        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;

        switch (connection_status) {
            case HeartbeatStatus::kAllGood:
                g = 1.0f;
                break;
            case HeartbeatStatus::kTotalNetworkAndModbusFailure:
                r = 1.0f;
                b = 1.0f;
                break;
            case HeartbeatStatus::kWifiDisconnected:
                b = 1.0f;
                break;
            case HeartbeatStatus::kModbusConnectionLost:
                r = 1.0f;
                break;
            default:
                break;
        }

        this->phase_++;

        const std::string mode_name = this->select_work_mode_->current_option();
        const std::string_view mode = mode_name;
        const float brightness_modifier = HeartbeatControl::get_brightness(mode, this->phase_);
        const std::string final_status = HeartbeatControl::get_status_label(connection_status, mode);

        if (this->heartbeat_led_ != nullptr) {
            auto hb_led_call = heartbeat_led_->turn_on();
            hb_led_call.set_rgb(r * brightness_modifier, g * brightness_modifier, b * brightness_modifier);
            hb_led_call.set_transition_length(150);
            hb_led_call.perform();
        }

        if (this->heartbeat_status_ != nullptr && this->badge_throttle_++ >= 5) {
            this->badge_throttle_ = 0;
            if (this->heartbeat_status_->state != final_status) {
                this->heartbeat_status_->publish_state(final_status);
            }
        }
    }
} // namespace esphome::heartbeat
