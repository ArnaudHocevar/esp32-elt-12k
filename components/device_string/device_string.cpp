#include "device_string.h"

#include "esphome/core/log.h"

namespace esphome::device_string {
    namespace {
        constexpr auto TAG = "device_string";
    }

    void DeviceStringComponent::setup() {
        source_->add_on_state_callback([this](const std::string &state) { this->update(state); });
        if (source_->has_state()) {
            update(source_->state);
        }
    }

    void DeviceStringComponent::dump_config() {
        ESP_LOGCONFIG(TAG, "Device string parser");
        LOG_TEXT_SENSOR("  Source Sensor: ", "Source (raw) data", this->source_);
        LOG_TEXT_SENSOR("  ", "ESPHome Version", this->esphome_version_);
        LOG_TEXT_SENSOR("  ", "Board Device", this->board_device_);
        LOG_TEXT_SENSOR("  ", "Framework Version", this->framework_version_);
        LOG_SENSOR("  ", "CPU Frequency", this->cpu_frequency_);
    }

    void DeviceStringComponent::update(const std::string &raw) const {
        // parse only once (successfully)
        static bool has_parsed = false;
        if (has_parsed) {
            return;
        }

        const auto [esphome_version, board_device, framework_version, cpu_freq, valid] =
                esp32_elt12k::device_string::parse_device_string(raw);
        if (!valid) {
            ESP_LOGW(TAG, "Device debug string structure mismatched or not ready yet");
            return;
        }

        esphome_version_->publish_state(esphome_version);
        board_device_->publish_state(board_device);
        framework_version_->publish_state(framework_version);
        if (cpu_freq > 0.0F) {
            cpu_frequency_->publish_state(cpu_freq);
        }

        has_parsed = true;
    }
} // namespace esphome::device_string
