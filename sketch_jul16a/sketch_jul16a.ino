#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "driver/i2s.h"
#include "soc/syscon_reg.h"
#include "driver/adc.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"

uint32_t a =0;
uint32_t    SAMPLE_RATE = 6000;
uint32_t    NUM_SAMPLES = 1000;

static QueueHandle_t i2s_event_queue; 

void setup(){
  Serial.begin(115200);
  Serial.printf("%d :",APB_CLK_FREQ);
    i2s_config_t i2s_config ;
    i2s_config.mode =  i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN);
    i2s_config.sample_rate = SAMPLE_RATE;                         
    i2s_config.dma_buf_len = NUM_SAMPLES;                          
    i2s_config.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT;       
    i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    i2s_config.use_apll = false,
    i2s_config.communication_format = I2S_COMM_FORMAT_I2S;
    i2s_config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
    i2s_config.dma_buf_count = 2;
    i2s_driver_install(I2S_NUM_0, &i2s_config, 1, &i2s_event_queue);
    i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0);
    i2s_adc_enable(I2S_NUM_0); 
}

void loop(){
    uint16_t i2s_read_buff[NUM_SAMPLES];
        system_event_t evt;
        if (xQueueReceive(i2s_event_queue, &evt, portMAX_DELAY) == pdPASS) {
            if (evt.event_id==2) {
               a = i2s_read_bytes(I2S_NUM_0, (char*)i2s_read_buff,NUM_SAMPLES*2, portMAX_DELAY);
                //Serial.write(100);
                Serial.println(a);
            }
        }
}
