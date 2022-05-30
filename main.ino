
#define PINO_RX 13
#define PINO_TX 13
#define BAUD_RATE 1
#define HALF_BAUD 1000/(2*BAUD_RATE)


// PAR -> 0 | IMPAR -> 1
#define PARITY 0 

#include "Temporizador.h"



// Calcula bit de paridade - Par ou impar
bool bitParidade(char dado){
    int count = 0;
    char div = dado;
    
    while (div>0) {
        count += div%2; // se div for impar: div%2=1. Se for par: div%2=0
        div = div >> 1; // bit-shift a direita = divisao inteira por 2
    }
    return count%2==0;
}

// Rotina de interrupcao do timer1
// O que fazer toda vez que 1s passou?
ISR(TIMER1_COMPA_vect){
  //>>>> Codigo Aqui <<<<
}

// Executada uma vez quando o Arduino reseta
void setup(){
  //desabilita interrupcoes
  noInterrupts();
  // Configura porta serial (Serial Monitor - Ctrl + Shift + M)
  Serial.begin(9600);
  // Inicializa TX ou RX
  //>>>> Codigo Aqui <<<<
  // Configura timer
  //>>>> Codigo Aqui <<<<
  // habilita interrupcoes
  interrupts();
}

// O loop() eh executado continuamente (como um while(true))
void loop ( ) {
  //>>>> Codigo Aqui <<<<
}
