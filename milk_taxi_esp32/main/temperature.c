#include "temperature.h"
#include "esp_adc/adc_continuous.h"
#include "esp_adc/adc_monitor.h"
#include "esp_adc/adc_filter.h"
#include "esp_adc/adc_oneshot.h"


// Single mode configuration

adc_oneshot_unit_init_cfg_t adc_oneshot_unit_config = {
    .unit_id = ADC_UNIT_1,
    .clk_src = ADC_DIGI_CLK_SRC_DEFAULT,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
};

adc_oneshot_chan_cfg_t adc_oneshot_channel_config = {
    .bitwidth = ADC_BITWIDTH_DEFAULT,
    .atten = ADC_ATTEN_DB_12,
};



adc_oneshot_unit_handle_t adc_oneshot_unit_handle;

void adc_oneshot_init()
{
    //Configure ADC
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_oneshot_unit_config, &adc_oneshot_unit_handle));

    //Configure ADC channel
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_oneshot_unit_handle, ADC_CHAN_2_GPIO_2, &adc_oneshot_channel_config));


}

void adc_oneshot_raw_read(int* out_raw)
{
    ESP_ERROR_CHECK(adc_oneshot_read(adc_oneshot_unit_handle, ADC_CHAN_2_GPIO_2, out_raw));
}



void adc_oneshot_deinit()
{
    esp_err_t error = ESP_OK;

    error = adc_oneshot_del_unit(adc_oneshot_unit_handle);

    if(error)
    {
        printf("ADC single mode deinitialization failed, error: %d\r\n", error);
    }
}














// Continouse mode configuration

static adc_continuous_handle_cfg_t adc_continuous_handle_config = {
    .max_store_buf_size = 1024,
    .conv_frame_size = SOC_ADC_DIGI_DATA_BYTES_PER_CONV * 25,
    .flags = {
        .flush_pool = 1,
    },
};

static adc_continuous_iir_filter_config_t iir_continuous_config = {
    .unit = ADC_UNIT_1,
    .channel = ADC_CHANNEL_0,
    .coeff = ADC_DIGI_IIR_FILTER_COEFF_16,
};

static adc_monitor_config_t monitor_continuous_config = {
    .adc_unit = ADC_UNIT_1,
    .channel = ADC_CHANNEL_0,
    .h_threshold = -1,
    .l_threshold = -1,

};

static adc_continuous_handle_t adc_continuous_handle;
static adc_iir_filter_handle_t iir_continuous_handle;
static adc_monitor_handle_t monitor_continuous_handle;

void adc_continuous_init()
{
    esp_err_t error = ESP_OK;
    error = adc_continuous_new_handle(&adc_continuous_handle_config, &adc_continuous_handle);

    if(!error)
    {
        printf("ADC Continuous mode initialized successfully\r\n");
    }
    else
    {
        printf("ADC Continuous mode initialization failed, error: %d\r\n", error);
    }

    adc_new_continuous_iir_filter(adc_continuous_handle, &iir_continuous_config, &iir_continuous_handle);

    error = adc_new_continuous_monitor(adc_continuous_handle, &monitor_continuous_config, &monitor_continuous_handle);

    if (!error)
    {
        printf("ADC Monitor initialized successfully\r\n");
    }
    else
    {
        printf("ADC Monitor initialization failed, error: %d\r\n", error);
    }

    adc_continuous_monitor_enable(adc_continuous_handle);

}
