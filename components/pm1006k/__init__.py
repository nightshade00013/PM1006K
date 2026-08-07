import esphome.codegen as cg
from esphome.components import uart

CODEOWNERS = ["@custom"]
DEPENDENCIES = ["uart"]

pm1006k_ns = cg.esphome_ns.namespace("pm1006k")
PM1006KComponent = pm1006k_ns.class_("PM1006KComponent", cg.Component, uart.UARTDevice)
