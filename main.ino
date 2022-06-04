
#define PINO_TX 13 // Pin de transmissão
#define PINO_CTS 12
#define PINO_RTS 11
#define PINO_CLK 10
#define BAUD_RATE 1
#define HALF_BAUD 1000/(2*BAUD_RATE)


// PAR -> 0 | IMPAR -> 1
#define PARITY 0 

#define DEBUG 1
#define dprint(str) {
    if (DEBUG) Serial.println(str);
}

#include "Temporizador.h"


boolean clock = 0,
     clk_change = 0;
     sig_rts = 0,
     sig_cts = 0;


void transmitBit(int pin, boolean b) {
    digitalWrite(pin, b ? HIGH : LOW);
}

void transmitCLK() {
    transmitBit(PINO_CLK, b);
    clock = !clock;
    clk_change = 1;
}

ISR(TIMER1_COMPA_vect) {
    transmitCLK();
}

// Waits for clock's signal changes to 0
void waitClock() {
    while (clock || !clock && !clk_change);
    clk_change = 0;
}

// serial byte transmition and parity bit at the end
// Data sent: emissor --> Bp-b7-b6-b5-b4-b3-b2-b1-b0  --> receptor
void sendData(char data) {
    int n=0, div=data; 
    boolean parity=0;
    
    while (n < 8) {
        waitClock();
        transmitBit(PINO_TX, div%2);
        ones ^= div%2;
        div = div >> 1;
        n++;
    }

    // transmit parity bit
    waitClock();
    transmitBit(PINO_TX, parity!=PARITY);

}

void readCTS() {
    sig_cts = digitalRead(PINO_CTS)==HIGH;
}

void waitCTS() {
    while (!sig_cts) readCTS();
}

void sendRTS() {
    transmitBit(PINO_RTS, sig_rts);
}

// Executada uma vez quando o Arduino reseta
void setup(){
    //desabilita interrupcoes
    noInterrupts();
    // Configura porta serial (Serial Monitor - Ctrl + Shift + M)
    Serial.begin(9600);
    // Inicializa pins
    pinMode(PINO_TX, OUTPUT);
    pinMode(PINO_RTS, OUTPUT);
    pinMode(PINO_CTS, INPUT);
    pinMode(PINO_CLK, OUTPUT);
    // Configura timer
    configuraTemporizador(BAUD_RATE);
    // habilita interrupcoes
    interrupts();
}

// O loop() eh executado continuamente (como um while(true))
void loop ( ) {
  	if (Serial.avaliable()>0) {
  		c = Serial.read();
        
        waitCTS(0);

        sig_rts = 1;
        sendRTS();
        waitCTS(1);

        sendData(c);

        sig_rts = 0;
        sendRTS();
  	}
}
