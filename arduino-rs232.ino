
#define PINO_TX 13 // Pin de transmissão
#define PINO_CTS 12
#define PINO_RTS 11
#define BAUD_RATE 1
#define HALF_BAUD 1000/(2*BAUD_RATE)

#define START 1
#define END 0

// PAR -> 0 | IMPAR -> 1
#define PARITY 0 

#include "Temporizador.h"

boolean clk = 0,
        clk_change = 0,
        sig_rts = 0,
        sig_cts = 0;


void waitClk() {
    while(clk);
    clk = 1;
}

// serial byte transmition and parity bit at the end
// Data sent: emissor --> Bp-b7-b6-b5-b4-b3-b2-b1-b0  --> receptor
void sendData(char data) {
    int n=0, div=data; 
    boolean parity=0;
    
    Serial.println("Transmit: ");
    while (n < 8) {

        transmitBit(PINO_TX, div%2);
        parity ^= div%2;
        div >>= 1;

        n++;
    }
    Serial.print("\n");

    // transmit parity bit
    Serial.println("Paridade: ");
    transmitBit(PINO_TX, parity!=PARITY);
    Serial.print(parity);
    Serial.print("\n");
}
void transmitBit(int pin, boolean b) {
    waitClk();
    digitalWrite(pin, b ? HIGH : LOW);
}

ISR(TIMER1_COMPA_vect) {
    clk = 0;
}

void readCTS() {
    sig_cts = digitalRead(PINO_CTS)==HIGH;
}

void waitCTS(boolean sig) {
    while (sig_cts!=sig) readCTS();
}

void sendRTS() {
    transmitBit(PINO_RTS, sig_rts);
}

void handshake(bool state) {
    sig_rts = state;
    sendRTS();
    waitCTS(state);
    Serial.println("received CTS");
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
    // Configura timer
    configuraTemporizador(BAUD_RATE);
    // habilita interrupcoes
    interrupts();
}

void sendString() {
    char c;

    iniciaTemporizador();

    handshake(START);

    while (Serial.available()>0) {
        c = Serial.read();
        if (c == '\n')
            continue;

        Serial.println(c);
      	
        sendData(c);
    }

    handshake(END);
    paraTemporizador();
}

// O loop() eh executado continuamente (como um while(true))
void loop ( ) {
    if (Serial.available() > 0) {
        sendString();
        Serial.println("Sent!");    
    }
}
