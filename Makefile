
FQBN = arduino:avr:uno
PORT = /dev/ttyUSB0

all: upload
	echo "uploaded to Arduino!"
	
compile:
	arduino-cli compile --fqbn "$(FQBN)"

upload: compile
	arduino-cli upload --fqbn "$(FQBN)" -p $(PORT)
