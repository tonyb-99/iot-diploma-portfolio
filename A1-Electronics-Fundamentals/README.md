Please refer to the circuit diagram in /A1-ELECTRONICS-FUNDAMENTALS

The circuit is a temperature sensor with an led indicator. The setup requires an ESP32, 2 x 100 ohm resistors, a 150 ohm resistor, a cathode RGB led and a thermistor module (or a NTC MF58 B=3950 10K ohm thermistor in series with a 10k ohm resistor).

The circuit pin layout is as follows:
-GPIO25 in series with 150k ohm resistor and the rgb led using the 'red' pin.
-GPIO26 in series with 100k ohm resistor and the rgb led using the 'green' pin.
-GPIO27 in series with 100k ohm resistor and the rgb led using the 'blue' pin.
-GND is connected the ground pin on the rgb led.
-GPIO34 is connected between the thermistor and the 10k ohm resistor which are in all series with a connection to V_IN and GND.

NOTES: GPIO34 are one the pins used for reading inputs.