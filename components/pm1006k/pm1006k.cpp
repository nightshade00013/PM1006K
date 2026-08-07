#include "pm1006k.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pm1006k {

static const char *const TAG = "pm1006k";

void PM1006KComponent::setup() {
  this->buffer_.reserve(8);
}

void PM1006KComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "PM1006K:");
  LOG_SENSOR("  ", "PM2.5", this->pm_2_5_sensor_);
}

void PM1006KComponent::loop() {
  while (available()) {
    uint8_t c;
    read_byte(&c);

    // Frame must start with 0xA5
    if (buffer_.empty() && c != 0xA5) {
      continue;
    }

    buffer_.push_back(c);

    // Broadcast frame is 4 bytes
    if (buffer_.size() == 4) {
      uint8_t header = buffer_[0];
      uint8_t status = buffer_[1];
      uint8_t pm25_raw = buffer_[2];
      uint8_t checksum = buffer_[3];

      // Validate checksum using 7-bit mask (& 0x7F) to account for higher PM2.5 readings
      uint8_t calculated_cs = (uint8_t)(header + status + pm25_raw) & 0x7F;
      uint8_t expected_cs = checksum & 0x7F;

      if (calculated_cs == expected_cs) {
        float pm25_val = (float)pm25_raw;
        ESP_LOGD(TAG, "PM2.5: %.0f ug/m3", pm25_val);

        if (this->pm_2_5_sensor_ != nullptr) {
          this->pm_2_5_sensor_->publish_state(pm25_val);
        }
      } else {
        ESP_LOGW(TAG, "Checksum error: calculated 0x%02X, got 0x%02X", calculated_cs, checksum);
      }

      buffer_.clear();
    }
  }
}

}  // namespace pm1006k
}  // namespace esphome
