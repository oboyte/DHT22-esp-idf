#pragma once
#include <cstdint>
#include <array>
#include <expected>

extern "C" {
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_sys.h"
#include "esp_log.h"
}

using u8 = std::uint8_t;
using i8 = std::int8_t;
using u16 = std::uint16_t;
using i16 = std::int16_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;

namespace Sensors {

enum class error_types {
  timeout_error,
};

class DHT22 {
  private:
    static inline const char TAG[] = "DHT22";
    
    gpio_num_t m_gpio_port { }; 
    std::array<u8, 5> m_received_data = { };

    float m_humidity = 0.f;
    float m_temperature = 0.f;


    std::expected<u8, Sensors::error_types> check_bus_line(i8 desired_time, bool desired_state);
    i16 convert_binary_to_decimal(u8 data1, u8 data2);

  public:
    DHT22(gpio_num_t NUM);

    DHT22& send_start_signal();
    DHT22& receive_data();
    DHT22& parse_data();

    float get_humidity() const;
    float get_temperature() const;
};  
}


