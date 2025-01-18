#ifndef __MENU_H__
#define __MENU_H__


#define DEBOUNCE_TIME_MS 400 // Debouncing time in ms

enum menu_state {
    menu_actualTime,
    menu_alarmTime,
    menu_temperature,
    menu_max,
};

void gpio_task(void* arg);

void display_menu(enum menu_state menu_position);

#endif /* __MENU_H__ */
