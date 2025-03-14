#include "tx_api.h"
#include "stm32h7xx_hal.h"

TX_THREAD led_thread;
uint8_t led_thread_stack[1024];

void led_thread_entry(ULONG initial_input) {
    while (1) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        tx_thread_sleep(100);
    }
}

int main() {
    tx_kernel_enter();
    return 0;
}

void tx_application_define(void *first_unused_memory) {
    tx_thread_create(&led_thread, "LED Thread", 
        led_thread_entry, 0,
        led_thread_stack, sizeof(led_thread_stack),
        15, 15, TX_NO_TIME_SLICE, TX_AUTO_START);
}
