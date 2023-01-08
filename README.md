# PwrMgmt-Atmega328p_DS3231

#Power Management of Atmega-328p with RTC used for wake-up

This sequence displays the possibility to save power by keeping the processor in sleep mode. Following sleep modes are available:

    SLEEP_MODE_IDLE
    SLEEP_MODE_ADC
    SLEEP_MODE_PWR_SAVE
    SLEEP_MODE_STANDBY
    SLEEP_MODE_PWR_DOWN

Wake-up is done by a real-time clock module (DS3231), which uses the alarm1 and an interrupt on pin PIND2 (INT0).
In this particular case the wake-up occurs every minute, but it can be configured to wake up every second, minute, hour, day or date.

#Schematic
![Schematic](https://user-images.githubusercontent.com/75970114/211187498-391cfca1-7548-4354-8716-05b13d682ac4.png)
