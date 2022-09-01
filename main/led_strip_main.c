/* RMT example -- RGB LED Strip

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "led_strip.h"

static const char *TAG = "example";

#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define EXAMPLE_CHASE_SPEED_MS (10)

/**
 * @brief Simple helper function, converting HSV color space to RGB color space
 *
 * Wiki: https://en.wikipedia.org/wiki/HSL_and_HSV
 * 
 *    h: 0 ~ 360
 *    s: 0 ~ 100 
 *    v: 0 ~ 100
 */
void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
{
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (i) {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}

void app_main(void)
{
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint16_t hue = 0;
    uint16_t start_rgb = 0;

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(CONFIG_EXAMPLE_RMT_TX_GPIO, RMT_TX_CHANNEL);
    // set counter clock to 40MHz
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(CONFIG_EXAMPLE_STRIP_LED_NUMBER, (led_strip_dev_t)config.channel);
    led_strip_t *strip = led_strip_new_rmt_ws2812(&strip_config);
    if (!strip) {
        ESP_LOGE(TAG, "install WS2812 driver failed");
    }
    // Clear LED strip (turn off all LEDs)
    ESP_ERROR_CHECK(strip->clear(strip, 100));
    // Show simple rainbow chasing pattern
    ESP_LOGI(TAG, "LED Rainbow Chase Start");
  
#if 0 //orgin 
    while (true) 
    {
        for (int i = 0; i < 3; i++) 
        {
            for (int j = i; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j += 3) 
            {
                // Build RGB values
                hue = j * 360 / CONFIG_EXAMPLE_STRIP_LED_NUMBER + start_rgb;
                led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
               
                // Write RGB values to strip driver
                ESP_ERROR_CHECK(strip->set_pixel(strip, j, red, green, blue));
            }

            // Flush RGB values to LEDs
            ESP_ERROR_CHECK(strip->refresh(strip, 100));
            vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
            strip->clear(strip, 50);
            vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        }
        start_rgb += 60;
    }
#else

    uint32_t saturation = 100-20;
    uint32_t lightness = 100-20;   
    uint32_t cnt = 0;
    int i; //index

    start_rgb = 36;

    while (true) 
    {

        ESP_LOGI(TAG, "STEP.1 LED Setup Index (1 ~ 23)" );      
        
        hue = start_rgb % 360;
        lightness = lightness % 100;
        saturation = saturation % 100;

        // Set other LEDs (Index 1 ~ 23)
        for (i = 1; i < CONFIG_EXAMPLE_STRIP_LED_NUMBER; i++) 
        {
            // Build RGB values 
            led_strip_hsv2rgb(hue, saturation, lightness, &red, &green, &blue);
       
            //red=0;
            //green=0;
            //blue=0;

            // Write RGB values to strip driver
            ESP_ERROR_CHECK(strip->set_pixel(strip, i, red, green, blue));
  
        }
        
        // Set indicate LED (Index 0)
        i=0;
        switch(cnt%7)
        {
            case 0:
                red=255;
                ESP_ERROR_CHECK(strip->set_pixel(strip, i, red, green, blue));
                ESP_LOGI(TAG, "STEP.2 LED Setup Index %d (RED)",i );  
                break;
            case 1:
                green=255;
                ESP_ERROR_CHECK(strip->set_pixel(strip, i, red, green, blue));
                ESP_LOGI(TAG, "STEP.2 LED Setup Index %d (GREEN)",i  );  
                break;         
            case 2:
                blue=255;
                ESP_ERROR_CHECK(strip->set_pixel(strip, i, red, green, blue));
                ESP_LOGI(TAG, "STEP.2 LED Setup Index %d (BLUE)",i  ); 
                break;       
            case 3:
                red=255;
                green=255;
                blue=255;
                ESP_ERROR_CHECK(strip->set_pixel(strip, i, red, green, blue));
                ESP_LOGI(TAG, "STEP.2 LED Setup Index %d (ALL)-WHITE",i  ); 
                break;   
            case 4:
                red=255;
                green=255;
                ESP_ERROR_CHECK(strip->set_pixel(strip, i, red, green, blue));
                ESP_LOGI(TAG, "STEP.2 LED Setup Index %d (RED/GREEN)-YELLOW",i  ); 
                break;    
            case 5:
                red=255;
                blue=255;
                ESP_ERROR_CHECK(strip->set_pixel(strip, i, red, green, blue));
                ESP_LOGI(TAG, "STEP.2 LED Setup Index %d (RED/BLUE)-PURPLE",i  ); 
                break;       
            case 6:
                green=255;
                blue=255;
                ESP_ERROR_CHECK(strip->set_pixel(strip, i, red, green, blue));
                ESP_LOGI(TAG, "STEP.2 LED Setup Index %d (GREEN/BLUE)",i  );
                break;                          
        }


        
        // Refresh RGB values to LEDs
        ESP_ERROR_CHECK(strip->refresh(strip, 100));
        vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        
        ESP_LOGI(TAG, "STEP.4 LED Refresh OK " );

        vTaskDelay(pdMS_TO_TICKS(1000 * 2));
        
        ESP_LOGI(TAG, "STEP.5 LED CNT:%d ( Hue:%d  Sat:%d  Light:%d )",cnt ,hue ,saturation, lightness );

        vTaskDelay(pdMS_TO_TICKS(1000 * 2));


#if 0
        // Flush RGB values to LEDs
        strip->clear(strip, 50);
        vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));

        ESP_LOGI(TAG, "STEP.3 LED Flush Ok");
        vTaskDelay(pdMS_TO_TICKS(1000 * 3));
#endif 

        cnt++;

        //start_rgb += 3;
        lightness +=5;

    }

#endif 

}
