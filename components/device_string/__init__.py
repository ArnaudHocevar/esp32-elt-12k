import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, text_sensor
from esphome.const import CONF_ID

DEPENDENCIES = ["text_sensor"]

device_string_ns = cg.esphome_ns.namespace("device_string")
DeviceStringComponent = device_string_ns.class_("DeviceStringComponent", cg.Component)

CONF_SOURCE_ID = "source_id"
CONF_ESPHOME_VERSION_ID = "esphome_version_id"
CONF_BOARD_DEVICE_ID = "board_device_id"
CONF_FRAMEWORK_VERSION_ID = "framework_version_id"
CONF_CPU_FREQUENCY_ID = "cpu_frequency_id"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DeviceStringComponent),
        cv.Required(CONF_SOURCE_ID): cv.use_id(text_sensor.TextSensor),
        cv.Required(CONF_ESPHOME_VERSION_ID): cv.use_id(text_sensor.TextSensor),
        cv.Required(CONF_BOARD_DEVICE_ID): cv.use_id(text_sensor.TextSensor),
        cv.Required(CONF_FRAMEWORK_VERSION_ID): cv.use_id(text_sensor.TextSensor),
        cv.Required(CONF_CPU_FREQUENCY_ID): cv.use_id(sensor.Sensor),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    component = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(component, config)

    source = await cg.get_variable(config[CONF_SOURCE_ID])
    esphome_version = await cg.get_variable(config[CONF_ESPHOME_VERSION_ID])
    board_device = await cg.get_variable(config[CONF_BOARD_DEVICE_ID])
    framework_version = await cg.get_variable(config[CONF_FRAMEWORK_VERSION_ID])
    cpu_frequency = await cg.get_variable(config[CONF_CPU_FREQUENCY_ID])

    cg.add(component.set_source(source))
    cg.add(component.set_esphome_version(esphome_version))
    cg.add(component.set_board_device(board_device))
    cg.add(component.set_framework_version(framework_version))
    cg.add(component.set_cpu_frequency(cpu_frequency))
