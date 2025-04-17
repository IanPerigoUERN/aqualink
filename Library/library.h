#ifndef LIBRARY_H
#define LIBRARY_H

//Bibliotecas padrão
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>

//Bibliotecas do Pico
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/multicore.h"//Para usar o outro núcleo

//Bibliotecas de hardware
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "hardware/dma.h"

//Bibliotecs de conectividade
#include "lwip/tcp.h"
#include "lwip/dns.h"//Para o thingspeak                  
#include "lwip/init.h" 
#include "lwip/pbuf.h"

//Módulos do projeto
#include "Conection/CREDENTIALS.h"
#include "Conection/http_server.h"
#include "Display/display.h"
#include "Display/Display1306/ssd1306.h"
#include "Joystick/joystick.h"
#include "PWM/pwm.h"
#include "RGB/led.h"
#include "Global/global.h"
#include "LedMatrix/ws2818b.pio.h"
#include "LedMatrix/matriz.h"
#include "Button/button.h"
#include "Buzzer/buzzer.h"

//Uso futuro para simular um super  aquecimento do sistema ou pico de tensão
//#include "Temperature/temp.h"
//#include "Mic/mic.h"

#endif