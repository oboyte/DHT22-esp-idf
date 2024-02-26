#include "DHT22.hpp"

#include <string_view>
extern "C" {
  #include "esp_log.h"
  #include "freertos/task.h"
}




extern "C" void app_main(void)
{
  std::string_view TAG = "main";
  vTaskDelay(2000 / portTICK_PERIOD_MS);
 
  Sensors::DHT22 dht { GPIO_NUM_32 };

  ESP_LOGI(TAG.data(), "Initialized dht object");
  
  dht.send_start_signal()
    .receive_data()
    .parse_data();

  ESP_LOGI(TAG.data(), "Humidty: %f, Temperature: %f", dht.get_humidity(), dht.get_temperature());
}
