#include "gpio.h"

extern QueueHandle_t  gpio_evt_queue;

// Function for handling interrupt
static void IRAM_ATTR gpio_isr_handler(void* arg) {

    enum pinFlags flag = (enum pinFlags) arg;

    xQueueSendFromISR(gpio_evt_queue, &flag, NULL);
}

// Init function for GPIO and interrupt
void gpio_init_with_interrupt() {
    // Config pin as input
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,                 // Set interrupt for falling edge
        .mode = GPIO_MODE_INPUT,                        // Input mode
        .pin_bit_mask = GPIO_INPUT_PIN_SEL,             // Pins mask
        .pull_down_en = GPIO_PULLDOWN_DISABLE,          // Opcjonalnie: wyłączenie pull-down
        .pull_up_en = GPIO_PULLUP_ENABLE                // Opcjonalnie: włączenie pull-up
    };
    gpio_config(&io_conf);

    gpio_evt_queue = xQueueCreate(10, sizeof(enum pinFlags));

    // Init system GPIO interrupt service
    gpio_install_isr_service(0);

    // Rejestracja funkcji obsługi przerwania dla wybranego pinu
    gpio_isr_handler_add(GPIO_PIN_UP,       gpio_isr_handler, (void*) GPIO_PIN_UP);
    gpio_isr_handler_add(GPIO_PIN_DOWN,     gpio_isr_handler, (void*) GPIO_PIN_DOWN);
    gpio_isr_handler_add(GPIO_PIN_SELECT,   gpio_isr_handler, (void*) GPIO_PIN_SELECT);
    gpio_isr_handler_add(GPIO_PIN_SET,      gpio_isr_handler, (void*) GPIO_PIN_SET);

}