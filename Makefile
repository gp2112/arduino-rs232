
FQBN = arduino:avr:uno
PORT = /dev/ttyUSB0

all: upload
	echo "uploaded to Arduino!"
	
compile:
	arduino-cli compile --fqbn "$(FQBN)"

upload: compile
	arduino-cli upload --fqbn "$(FQBN)" -p $(PORT)

configure:
	stty -F $(PORT) cs8 9600 ignbrk -brkint -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
