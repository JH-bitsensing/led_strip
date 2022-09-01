# ESP32 RMT based on LED TEST    

* 테스트 환경       
  * ESP32 Devkit V1    
  * LED Strip (24 LED)      

Flash 할 경우, Boot Button을 누르거나, Cap 달기     
 [ESP32 Devkit V1 Flash](https://randomnerdtutorials.com/solved-failed-to-connect-to-esp32-timed-out-waiting-for-packet-header/)

<br/>

* 테스트 소스  
  * [RMT( Remote Control Peripheral )](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf#rmt)   


<br/>

* 테스트 결과  
  1. LED Index 0: Indicate LED 로 테스트 진행 
  2. LED Index 1~23 : LED 23개 주황색 Dimming TEST 진행 

<br/>

**총 24개 LED WS2818 테스트 진행완료** 

RMT의 경우 ESP32의 어느 GPIO Pin에 RMT Channel 1개 와 연동하여 40MHz로 설정후    
RGB값만 변경하면 쉽게 설정가능    

<br/>
<br/>

ESP32 RMT LED Strip 소스     
  https://github.com/espressif/esp-idf/tree/master/examples/peripherals/rmt/led_strip

함수설정 HSL     
  https://en.wikipedia.org/wiki/HSL_and_HSV

<br/>
<br/>


# RMT Transmit Example -- LED Strip

(See the README.md file in the upper level 'examples' directory for more information about examples.)

Although RMT peripheral is mainly designed for infrared remote applications, it can also support other generic protocols thanks to its flexible data format. [WS2812](http://www.world-semi.com/Certifications/WS2812B.html) is a digital RGB LED which integrates a driver circuit and a single control wire. The protocol data format defined in WS2812 is compatible to that in RMT peripheral. This example will illustrate how to drive an WS2812 LED strip based on the RMT driver.

## How to Use Example

### Hardware Required

* A development board with ESP32 SoC (e.g., ESP32-DevKitC, ESP-WROVER-KIT, etc.)
* A USB cable for Power supply and programming
* A WS2812 LED strip

Connection :

```
                             --- 5V
                              |
                              +
GPIO18 +-----------------+---|>| (WS2812)
                        DI    +
                              |
                             --- GND
```

### Configure the Project

Open the project configuration menu (`idf.py menuconfig`). 

In the `Example Connection Configuration` menu:

* Set the GPIO number used for transmitting the IR signal under `RMT TX GPIO` optin.
* Set the number of LEDs in a strip under `Number of LEDS in a strip` option.

### Build and Flash

Run `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

## Example Output

Connect the `DI` signal of WS2812 LED strip to the GPIO you set in menuconfig.

Run the example, you will see a rainbow chasing demonstration effect. To change the chasing speed, you can update the `EXAMPLE_CHASE_SPEED_MS` value in `led_strip_main.c` file.

## Troubleshooting

For any technical queries, please open an [issue] (https://github.com/espressif/esp-idf/issues) on GitHub. We will get back to you soon.
