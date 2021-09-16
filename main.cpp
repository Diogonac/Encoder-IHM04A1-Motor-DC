#include "mbed.h"

InterruptIn Encoder(D9);
// Serial pc (D0, D1); //Comunicação com USB TX, RX

// Declara pinos de controle
PwmOut IN1_A(D5);    // Sentido de rotação 1
PwmOut IN2_A(D4);    // Sentido de rotação 2
DigitalOut EN_A(D2); // Permição para executar

AnalogIn POT(A0); // Leitura do potênciomentro

InterruptIn BT_USER(USER_BUTTON); // Botão

// Funções para acionar o motor
void sentido_1(float velocidade_1);
void sentido_2(float velocidade_2);

// Funções dos contadores
void count_BT(void);
void count_fall(void);
void count_rise(void);

// Função da velocidade
void calc_velocidade(void);

// Variáveis
int contador_BT = 0;

float velociade_RPM = 0.0;
float valor_tempo = 0.0;

// Temer
Timer tempo;

// main() runs in its own thread in the OS
int main() {


  // Defino a frequência de acionamento do motor
  IN1_A.period(0.01); // 500Hz
  IN2_A.period(0.01); // 500Hz

  EN_A = 0.0; // Desacopla (Estado de alta impedancia_)

  BT_USER.fall(&count_BT);
  Encoder.fall(&count_fall);
  Encoder.rise(&count_rise);


  while (true) {

      calc_velocidade();


    if (contador_BT == 1)
      sentido_1(0.7);
    if (contador_BT == 2)
      sentido_1(0.0);
    if (contador_BT > 2) {

      contador_BT = 1;

      EN_A = 0.0; // Desacopla (Estado de alta impedancia)
    }

  }
}

void count_BT(void) { contador_BT++; }

void sentido_1(float velocidade_1) {

  EN_A = 1.0; // Acopla o circuito

  IN1_A =
      velocidade_1; // Velocidade é o pwm, variável velociade varia de 0.0 - 1.0
  IN2_A = 0.0;
}

void sentido_2(float velocidade_2) {

  EN_A = 1.0; // Acopla o circuito

  IN2_A =
      velocidade_2; // Velocidade é o pwm, variável velociade varia de 0.0 - 1.0
  IN1_A = 0.0;
}

void count_fall(void) {
  valor_tempo = tempo.read_us();
  tempo.reset();
  tempo.stop();
}

void count_rise(void) {
  tempo.start();
}

void calc_velocidade(void) {

     if (valor_tempo > 10.0 & valor_tempo < 5000) {

      velociade_RPM =
          (1 / valor_tempo) * 1000000; 

       printf("%f\r\n", velociade_RPM);

     }
     if(valor_tempo > 5000){

         printf("%f\r\n", 0.0);

     }
}

