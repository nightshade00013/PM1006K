#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace pm1006k {

class PM1006KComponent : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_pm_2_5_sensor(sensor::Sensor *pm_2_5_sensor) { pm_2_5_sensor_ = pm_2_5_sensor; }

 protected:
  sensor::Sensor *pm_2_5_sensor_{nullptr};
  std::vector<uint8_t> buffer_;
};

}  // namespace pm1006k
}  // namespace esphome
