#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/component.h"

#include "device_string_parser.h"

namespace esphome::device_string {
    class DeviceStringComponent final : public Component {
    public:
        virtual ~DeviceStringComponent() = default;

        void set_source(text_sensor::TextSensor *source) { source_ = source; }
        void set_esphome_version(text_sensor::TextSensor *sensor) { esphome_version_ = sensor; }
        void set_board_device(text_sensor::TextSensor *sensor) { board_device_ = sensor; }
        void set_framework_version(text_sensor::TextSensor *sensor) { framework_version_ = sensor; }
        void set_cpu_frequency(sensor::Sensor *sensor) { cpu_frequency_ = sensor; }

        void setup() override;

        void dump_config() override;

    private:
        void update(const std::string &raw) const;

        text_sensor::TextSensor *source_{nullptr};
        text_sensor::TextSensor *esphome_version_{nullptr};
        text_sensor::TextSensor *board_device_{nullptr};
        text_sensor::TextSensor *framework_version_{nullptr};
        sensor::Sensor *cpu_frequency_{nullptr};
    };
} // namespace esphome::device_string
