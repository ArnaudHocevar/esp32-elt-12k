#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/light/light_state.h"
#include "esphome/components/select/select.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/component.h"

namespace esphome::heartbeat {
    class HeartbeatComponent final : public PollingComponent {
    public:
        virtual ~HeartbeatComponent() = default;

        void set_modbus_cx_status(binary_sensor::BinarySensor *modbus_cx_status) {
            modbus_cx_status_ = modbus_cx_status;
        }

        void set_work_mode_selector(select::Select *work_mode_selector) {
            select_work_mode_ = work_mode_selector;
        }

        void set_heartbeat_status(text_sensor::TextSensor *heartbeat_status) {
            heartbeat_status_ = heartbeat_status;
        }

        void set_heartbeat_led(light::LightState *heartbeat_led) {
            heartbeat_led_ = heartbeat_led;
        }

        void setup() override;

        void update() override;

        void dump_config() override;

    private:
        void update_status();

        binary_sensor::BinarySensor *modbus_cx_status_{nullptr};
        select::Select *select_work_mode_{nullptr};
        text_sensor::TextSensor *heartbeat_status_{nullptr};
        light::LightState *heartbeat_led_{nullptr};

        uint32_t phase_{0};
        uint8_t badge_throttle_{0};
    };
} // namespace esphome::heartbeat
