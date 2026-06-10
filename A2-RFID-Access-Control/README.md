I2C (Inter-Integrated Ciruit) was developed by the Philips company in 1982 which uses a 2 wire communication protocol such as the SCL (Serial Clock) and SDA (Serial Data). These wires allow carry signals which enable master and slave devices to send and receive data, allowing a master to control slaves or multiple masters controlling slaves. SPI (Serial Peripheral Interface) was developed by Motorola in the 1980s is a 4 wire communication protocol. They are the SCLK (Serial Clock), MOSI (Master out, Slave in), MISO (Master in, Slave out) and SS (Slave select which each slave connects to the master but not to other slaves). The main differences can be described below:
Speed:                  I2C is slower               SPI is faster
Distance:               I2C short (<2m)             SPI very short (<30cm)
Cost & complexity:      I2C is cheaper and simpler  SPI costs more for its speed 
Power:                  I2C uses more power         SPI for low power application
Interference:           I2C less susceptible        SPI more susceptible
Data Integrity:         I2C acknowledges data       SPI does not verify
Duplex:                 I2C is half duplex          SPI is full duplex

https://www.totalphase.com/blog/2021/07/i2c-vs-spi-protocol-analyzers-differences-and-similarities/
https://electrical-world.com/posts/i2c-vs-spi-protocols-complete-guide