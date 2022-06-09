# arduino-rs232

This is a simple implementation of a serial comunication protocol, based on [RS-232](https://pt.wikipedia.org/wiki/RS-232), on Arduino, for Computer Networks subject.

### Group Members

- [Guilherme Ramos Costa Paixão](https://guip.dev) - 11796079
- [Adrio Oliveira Alves](https://github.com/adriooa) - 11796830
- [Eduardo Vinicius Barbosa Rossi](https://github.com/RossiEduardo) - 10716887

### Compatibility

This code was made for Arduino UNO. For other Arduinos versions, you may have to change it's registers.

### Compiling and Upload


If you use Arduino's IDE, you can just open the project on it aaand run!

If you prefer to use some other IDE or Text Editor, you have this power too!

#### Without Arduino's IDE on Linux

1. Add your user to UUCP group (you may use sudo for this)
```bash
usermod -aG uucp YOUR-USER
```
- Restart or logout/login your system

2. Check if *cdc_acm* module is already activated in your system:

```bash
lsmod | grep cdc_acm
```
- If nothing is returned, than you need to activate it (you may need sudo):
```bash
modprobe cdc_acm
```

3. Install [arduino-cli](https://arduino.github.io/arduino-cli/0.19/)
4. Create the configuration file:
```bash
arduino-cli config init
```
5. Install your arduino core drivers. This code was made for Arduino UNO, so:
```bash
arduino-cli core update-index
arduino-cli core install arduino:avr
```

6. Now you can compile and upload to your board using the *Makefile*.

- OBS: You must `make configure` every time you plug your rduino in.

7. To get arduino's output just execute `./listen` and to send an input, `./send YOUR-INPUT`



