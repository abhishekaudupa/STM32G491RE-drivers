#ifndef __NUCLEOG491RE_H__
#define __NUCLEOG491RE_H__

#include "config.h"

/* public interface */
void enable_user_led();
void disable_user_led();
void user_led_on();
void user_led_off();
void user_led_toggle();
void user_led_blink(const uint32_t ms);
void enable_user_button();
void disable_user_button();

#endif
