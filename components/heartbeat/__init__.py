import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, light, select, text_sensor
from esphome.const import CONF_ID, CONF_NAME

DEPENDENCIES = ["binary_sensor", "light", "select", "text_sensor"]

heartbeat_ns = cg.esphome_ns.namespace("heartbeat")
HeartbeatComponent = heartbeat_ns.class_("HeartbeatComponent", cg.PollingComponent)

CONF_MODBUS_CONNECTION_STATUS = "modbus_cx_status"
CONF_WORK_MODE_SELECTOR_ID = "select_work_mode"
CONF_HEARTBEAT_STATUS = "heartbeat_status"
CONF_HEARTBEAT_LED = "heartbeat_led"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HeartbeatComponent),
        cv.Required(CONF_MODBUS_CONNECTION_STATUS): cv.use_id(binary_sensor.BinarySensor),
        cv.Required(CONF_WORK_MODE_SELECTOR_ID): cv.use_id(select.Select),
        cv.Required(CONF_HEARTBEAT_STATUS): text_sensor.text_sensor_schema(
            {
                cv.Required(CONF_NAME): cv.string,
            }
        ),
        cv.Required(CONF_HEARTBEAT_LED): cv.use_id(light.LightState),
    }
).extend(cv.polling_component_schema("200ms"))


async def to_code(config):
    component = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(component, config)

    modbus_connection_status = await cg.get_variable(config[CONF_MODBUS_CONNECTION_STATUS])
    work_mode_selector = await cg.get_variable(config[CONF_WORK_MODE_SELECTOR_ID])
    cg.add(component.set_modbus_cx_status(modbus_connection_status))
    cg.add(component.set_work_mode_selector(work_mode_selector))

    heartbeat_status_cfg = config[CONF_HEARTBEAT_STATUS]
    heartbeat_status_cfg[CONF_ID] = cv.declare_id(text_sensor.TextSensor)("heartbeat_status_id")
    heartbeat_status_sensor = await text_sensor.new_text_sensor(heartbeat_status_cfg)
    cg.add(component.set_heartbeat_status(heartbeat_status_sensor))

    heartbeat_led = await cg.get_variable(config[CONF_HEARTBEAT_LED])
    cg.add(component.set_heartbeat_led(heartbeat_led))
