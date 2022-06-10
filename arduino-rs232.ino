
#define PINO_TX 13 // Pin de transmissão
#define PINO_RTS 12
#define PINO_CTS 11
#define BAUD_RATE 2
#define HALF_BAUD 1000/(2*BAUD_RATE)

#define START 1
#define END 0

// PAR -> 0 | IMPAR -> 1
#define PARITY 0 

#include "Temporizador.h"

volatile boolean clk = 0,
        clk_change = 0,
        sig_rts = 0,
        sig_cts = 0,
        parity = 0,
        is_on = 0,
        sent = 0;

volatile int n = 0;

volatile char c;

void waitClk() {
    while(!clk);
    clk = 0;
}


void transmitBit(int pin, boolean b) {
    digitalWrite(pin, b ? HIGH : LOW);
}

ISR(TIMER1_COMPA_vect) {
    is_on = sig_rts && sig_cts;
    if (is_on) {

        
        // dado
        if (n >= 0 && n <= 7) {
            Serial.print(c%2);
            parity ^= c%2;
            transmitBit(PINO_TX, c%2);
            c >>= 1;
        }
        else if (n == 8) {
            Serial.print(parity);
            transmitBit(PINO_TX, parity);

        }
        else if (n == 9) {
            transmitBit(PINO_TX, 1);
            Serial.print(1);

        }
        else if (n == 10) {
            transmitBit(PINO_TX, 1);
            Serial.println(1);
            handshake(END);
            paraTemporizador();
            n = 0;
            sent = 1;
        }
        n++;
    }
}

void readCTS() {
    sig_cts = digitalRead(PINO_CTS)==HIGH;
    //sig_cts = 0;
}

void waitCTS(boolean sig) {
    while (sig_cts!=sig) readCTS();
    Serial.print("Sig CTS");
    Serial.println(sig_cts);
}

void sendRTS() {
    transmitBit(PINO_RTS, sig_rts);
}

void handshake(bool state) {
    sig_rts = state;
    sendRTS();
    Serial.println("Waiting CTS");
    waitCTS(state);
    if (state) Serial.println("received CTS");
    else Serial.println("Closed Connection");
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


// O loop() eh executado continuamente (como um while(true))
void loop ( ) {
    char tmp;
    while (Serial.available()>0) {
        sent = 0;
        tmp = Serial.read();

        //if (tmp == '\n')
        //    continue;
        
        c = tmp;


        handshake(START);
        
        iniciaTemporizador();
                
        Serial.print(0);
        transmitBit(PINO_TX, 0);
        


        Serial.println(c);
        Serial.println("Sent!");
        while (!sent);
    }
   
}
