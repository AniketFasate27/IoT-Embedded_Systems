#include <driver/i2s.h>
#define ADC_CHANNEL

void configure_i2s(){
  i2s_config_t i2s_config = 
    {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),  // I2S receive mode with ADC
    .sample_rate = 9000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,                  // 16 bit I2S
    .channel_format = I2S_CHANNEL_FMT_ALL_LEFT,                  // all the left channel, no other options works for me!
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),   // I2S format
    .intr_alloc_flags = 1,    // Tried buffer interrupt somehow.
    .dma_buf_count = 8,    // number of DMA buffers. ( Tried different options )
    .dma_buf_len = 1000,  // number of samples.  ( Tried different options )
    .use_apll = 0,              // no Audio PLL ( I dont need the adc to be accurate )
  };
  adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_11db);  // ADC_CHANNEL is ADC_CHANNEL_0 - GPIO34.
  adc1_config_width(ADC_WIDTH_12Bit);                                      // I need 0 - 4095.
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);                  // I2S_NUM_0 works for adc.
 
  i2s_set_adc_mode(ADC_UNIT_1, ADC_CHANNEL);
  SET_PERI_REG_MASK(SYSCON_SARADC_CTRL2_REG, SYSCON_SARADC_SAR1_INV);  // Inverse because fifo.
  i2s_adc_enable(I2S_NUM_0);
}

size_t bytes_read;
static const inline void ADC_Sampling(){
  uint16_t i2s_read_buff[NUM_SAMPLES];
  String data;
  //xQueueReceive(i2s_queue, &event, (2*samplingFrequency)/  portTICK_PERIOD_MS); // Tried some kind of an event thing. Restarts
  i2s_read(I2S_NUM_0, (char*)i2s_read_buff,NUM_SAMPLES * sizeof(uint16_t), &bytes_read, portMAX_DELAY);  // I2S read to buffer
  if(I2S_EVENT_RX_DONE){  // This is the flag that i mentioned. Does nothing if there are no events assigned based on my observation.
  
    //i2s_adc_disable(I2S_NUM_0);  // Some comment war
  //long Millis_Now = millis();
  //if (((Millis_Now - Start_Sending_Millis) >= DUMP_INTERVAL)&& Chart_is_ok_to_Send_Data)
  //{
    //Start_Sending_Millis = Millis_Now;
    
    for (int i=0;i<NUM_SAMPLES;i++) {
      data = String((i2s_read_buff[i])/(float)40.95);  // Actual read from buffer, i need the data in % format on the web.
      if(olvasunk_e){                                                // Want to send String.
      data += "}";
      webSocket.broadcastTXT(data.c_str(), data.length());  // Send the data with webSocket.
      }
    }
    
    //i2s_zero_dma_buffer(I2S_NUM_0);  // Comment war again.
    //i2s_adc_enable(I2S_NUM_0);
  //}
  
  }
}
