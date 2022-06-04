
#define PINO_TX 13 // Pin de transmissão
#define PINO_CTS 12
#define PINO_RTS 11
#define PINO_CLK 10
#define BAUD_RATE 1
#define HALF_BAUD 1000/(2*BAUD_RATE)


// PAR -> 0 | IMPAR -> 1
#define PARITY 0 

#include "Temporizador.h"

char clock = 0,
     sig_rts = 0,
     sig_cts = 0;


// Rotina de interrupcao do timer1
// O que fazer toda vez que 1s passou?
ISR(TIMER1_COMPA_vect){
  //>>>> Codigo Aqui <<<<
}

void transmitBit(char b) {
    digitalWrite(PINO_TX, b ? HIGH : LOW);
}

void transmitCLK() {
    digitalWrite(PINO_CLK, clock ? HIGH : LOW);
    clock = !clock;
}

void sendData(char data) {
    int n=0, ones=0;
    int div = data;
    while (div>0 || n < 8) {
        transmitBit(div%2);
        ones += div%2;
        div = div >> 1;
        n++;
    }
    transmitBit(ones%2==PARITY);

}

void readCTS() {
    sig_cts = digitalRead(PINO_CTS)==HIGH;
}

void sendRTS() {
    digitalWrite(PINO_RTS, sig_rts ? HIGH : LOW);
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
  //>>>> Codigo Aqui <<<<
}
