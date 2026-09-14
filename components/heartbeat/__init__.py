import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, light, select, text_sensor
from esphome import pins
from esphome.const import CONF_ID, CONF_NAME, CONF_PIN, CONF_CHIPSET, CONF_RGB_ORDER, CONF_NUMBER

DEPENDENCIES = ["binary_sensor", "light", "select", "text_sensor"]

heartbeat_ns = cg.esphome_ns.namespace("heartbeat")
HeartbeatComponent = heartbeat_ns.class_("HeartbeatComponent", cg.PollingComponent)

# Reference the native ESP32 RMT platform class wrapper
esp32_rmt_ns = cg.global_ns.namespace("esphome").namespace("esp32_rmt_led_strip")
ESP32RMTLEDStripLightOutput = esp32_rmt_ns.class_("ESP32RMTLEDStripLightOutput", light.AddressableLight)

CONF_MODBUS_CONNECTION_STATUS = "modbus_cx_status"
CONF_WORK_MODE_SELECTOR_ID = "select_work_mode"

CONF_HEARTBEAT_LED = "heartbeat_led"
CONF_HEARTBEAT_STATUS = "heartbeat_status"

CONF_HEARTBEAT_LED_GPIO_PIN = "gpio_pin"
CONF_HEARTBEAT_LED_CHIPSET = "chipset"
CONF_HEARTBEAT_LED_RGB_ORDER = "rgb_order"

# RMT-compatible chipsets and configurations
HEARTBEAT_LED_CHIPSETS = ["WS2812B", "WS2812", "WS2811", "SK6812"]
HEARTBEAT_LED_RGB_ORDERS = ["RGB", "GRB", "BRG", "BGR"]

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
        cv.Required(CONF_HEARTBEAT_LED): light.ADDRESSABLE_LIGHT_SCHEMA.extend(
            {
                cv.Optional(CONF_NAME, "Heartbeat led"): cv.string,
                cv.Required(CONF_HEARTBEAT_LED_GPIO_PIN): pins.internal_gpio_output_pin_schema,
                cv.Required(CONF_HEARTBEAT_LED_CHIPSET): cv.one_of(*HEARTBEAT_LED_CHIPSETS, upper=True),
                cv.Required(CONF_HEARTBEAT_LED_RGB_ORDER): cv.one_of(*HEARTBEAT_LED_RGB_ORDERS, upper=True),
            }
        ),
    }
).extend(cv.polling_component_schema("200ms"))

# Update code target dependencies to pull in the native ESP32 RMT driver
AUTO_LOAD = ["esp32_rmt_led_strip", "light"]


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

    heartbeat_led_cfg = config[CONF_HEARTBEAT_LED]
    # Initialize the standard ESPHome Addressable Light core state tracker
    heartbeat_led_rmt_id = cv.declare_id(ESP32RMTLEDStripLightOutput)("heartbeat_led_rmt_id")
    heartbeat_led_rmt = cg.new_Pvariable(heartbeat_led_rmt_id)

    # Instantiate the structural RMT Output device configurations
    # Note: parsing logic uses lowercase configurations internally
    gpio_pin_number = heartbeat_led_cfg[CONF_HEARTBEAT_LED_GPIO_PIN][CONF_NUMBER]
    cg.add(heartbeat_led_rmt.set_pin(gpio_pin_number))

    cg.add(heartbeat_led_rmt.set_num_leds(1))
    rgb_order_str = heartbeat_led_cfg[CONF_HEARTBEAT_LED_RGB_ORDER].upper()
    rgb_enum_expr = cg.RawExpression(f"esphome::esp32_rmt_led_strip::ORDER_{rgb_order_str}")
    cg.add(heartbeat_led_rmt.set_rgb_order(rgb_enum_expr))

    await light.register_light(heartbeat_led_rmt, heartbeat_led_cfg)
    heartbeat_led = await cg.get_variable(heartbeat_led_cfg[CONF_ID])

    # Link the formal AddressableLight wrapper pointer to your custom class
    cg.add(component.set_heartbeat_led(heartbeat_led))
