AVR_IIC library
====+++========
Simple I2C (TWI, IIC) interface library written using [AVR libc](https://www.nongnu.org/avr-libc/) to ensure maximum compatibility accross AVR devices.

Installation Method
-------------------

* Requirements<br>
  AVR libc. Version used was 2.1.0. 
* Installation<br>
  Copy files to your project. When using PlatformIO under VS Code copy the folder to `YOUR_PROJECT/lib`.

Usage & Examples
----------------

Functions `iic_start`, `iic_tx_addr`, `iic_tx_data`, `iic_rx_data` return result of the uint8_t type (unsigned char). 0xFF represents status ok. Other values are considered errors and their code can be checked from AVR datasheet.<br>
Below code needs to be expanded with some checks for the `result` value.
```
#include <iic.h>

uint8_t result;

iic_init(IIC_FREQ, IIC_PRESCALER1);
result = iic_start();
result = iic_tx_addr(SSD1306_ADDRESS, TW_WRITE);
result = iic_tx_data(data);
iic_stop();
```

Author
------
Michal Piekos (michal.public@wp.pl)

License
-------
[MIT License](LICENSE)