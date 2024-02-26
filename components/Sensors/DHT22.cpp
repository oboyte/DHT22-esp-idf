#include "DHT22.hpp"



Sensors::DHT22::DHT22(gpio_num_t NUM) : m_gpio_port { NUM } {
  for (auto &i : m_received_data) {
    i = 0;
  }
};

std::expected<u8, Sensors::error_types> Sensors::DHT22::check_bus_line(i8 desired_time, bool desired_state) {
  int uSec = 0;
  while (gpio_get_level(m_gpio_port) == desired_state) {
    if (uSec > desired_time) return std::unexpected(Sensors::error_types::timeout_error);
    uSec++;
    esp_rom_delay_us(1);
  }
  return uSec;
}

Sensors::DHT22& Sensors::DHT22::send_start_signal() {
  // Set port to output. DHT22 first requires the port to be low for AT LEAST 2ms.
  gpio_set_direction(m_gpio_port, GPIO_MODE_OUTPUT);
  gpio_set_level(m_gpio_port, 0);
  esp_rom_delay_us(3000);

  // Then set the port to high for 25us. 
  gpio_set_level(m_gpio_port, 1);
  esp_rom_delay_us(25);

  return *this;
}

Sensors::DHT22& Sensors::DHT22::receive_data() {
  // The DHT22 device should now have received the command to start sending its output.
  gpio_set_direction(m_gpio_port, GPIO_MODE_INPUT);
  
  // It will first output 
  auto uSec = check_bus_line(85, 0);
  if (!uSec.has_value()) {
    ESP_LOGE(TAG, "ERROR! 1");
    return *this;
  }

  uSec = check_bus_line(85, 1);
  if (!uSec.has_value()) {
    ESP_LOGE(TAG, "ERROR! 2");
    return *this;
  }

  for (auto& elem : m_received_data) {
    for (i8 i = 7; i >= 0; i--) {
      uSec = check_bus_line(56, 0);
      if (!uSec.has_value()) {
        ESP_LOGE(TAG, "ERROR! 3");
        return *this;
      }

      uSec = check_bus_line(75, 1);
      if (!uSec.has_value()) {
        ESP_LOGE(TAG, "ERROR! 4");
        return *this;
      }

      if (uSec.value() > 40) {
        elem |= (1 << i);
      }
    }
  }

  return *this;
}

Sensors::DHT22& Sensors::DHT22::parse_data() {
  // Unsure yet to use bit manipulation or bitsets. 
  i16 rh_data { m_received_data[0] };
  rh_data <<= 8;
  rh_data += m_received_data[1];

  i16 t_data { m_received_data[2] };
  t_data <<= 8;
  t_data += m_received_data[3];
 
  m_humidity = rh_data / 10.f;
  m_temperature = t_data / 10.f;

  return *this;
}

float Sensors::DHT22::get_humidity() const { return m_humidity; }
float Sensors::DHT22::get_temperature() const { return m_temperature; }
