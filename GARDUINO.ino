// BIBLIOTECA
#include <SoftwareSerial.h> // Biblioteca para emulação das portas RX e TX.
#include <DHT.h> // Biblioteca do sensor de temperatura e umidade.
#define DHTPIN A0 // Declarando que o sensor de temperatura e umidade encontra-se na porta analógica A0. 
#define DHTTYPE DHT21 // Puxando na biblioteca as pré-definições para o sensor de temperatura e umidade DHT21.
DHT dht(DHTPIN, DHTTYPE);

// Neste campo está sendo definido as entradas do sistema de ventilação, sistema de irrigação e do sensor de umidade do solo.
#define cooler 8
#define bomba 7
#define umidade analogRead(A1)
SoftwareSerial bluetooth(10,11); // Campo definindo variável do sensor bluetooth e quais são as portas RX e TX(Lembrando que estas portas invertem, assim o RX do sensor conecta no TX do arduino e o TX no RX)

// Campo de declaração de variáveis.
int temp_max = 30; // Temperatura máxima padrão.
int umid_min = 50; // Umidade mínima padrão.
int menu = 0; // Variável para controlar menu no aplicativo.
int info_B = 0; // Status do sistema de irrigação para aplicativo.
int info_C = 0; // Status do sistema de ventilação para aplicativo.

void setup() {
  Serial.begin(9600); // Inicialização serial do Arduino.
  bluetooth.begin(9600); // Inicialização serial do sensor bluetooth.
  
  // Definindo as variáveis do sistema de ventilação e do sistema de irrigação como portas de saída.
  pinMode(cooler, OUTPUT);
  pinMode(bomba, OUTPUT);
  dht.begin();
  
  
} 

void loop() {
// SENSOR DE TEMPERATURA E UMIDADE

 int umidadeReal = map(umidade, 1023, 0, 0, 100); // Converte a leitura do sensor que é de 1023 a 0 para de 0 a 100.
  // Definido que se a umidade lida seja menor que a umidade mínima declarada o sistema de irrigação será ativado. Se não, permanecerá desligado.
  // A variável info_B armazenará o valor 1 caso o sistema de irrigação esteja ligado, e 0 se estiver desligado.
 if (umidadeReal < umid_min){
   digitalWrite(bomba, LOW);
   info_B = 1;
 }
 else {
   digitalWrite(bomba,HIGH);
   info_B = 0;
 }
// SENSOR DE TEMPERATURA

  // Campo pré-definido pela biblioteca para leitura da temperatura em Celsius.
 float t = dht.readTemperature(); // Leitura da temperatura em Celsius.
 
  //Caso o arduino não capture dados no sensor, informará o erro na leitura.
 if (isnan(t)) {
   bluetooth.println(F("Falha na leitura do sensor DHT!"));
   return;
 }
  // Definido que se a Temperatura em Celsius lida seja maior que a temperatura máxima declarada, o sistema de ventilação será ativado. Se não, permanecerá desligado.
  // A variável info_C armazenará o valor 1 caso o sistema de ventilação esteja ligado, e 0 se estiver desligado.
 if (t > temp_max) {
   digitalWrite(cooler, LOW);
   info_C = 1;
 }
 else {
   digitalWrite(cooler, HIGH);
   info_C = 0;
 }
// MENU APLICATIVO

  // O programa irá verificar se ele recebeu alguma informação do aplicativo.
 if (bluetooth.available()){
  menu = (bluetooth.read());

  // Caso receba um valor, irá verificar se o mesmo se encaixa em um dos parâmetros:
  // 0- Informativo das opções que pode escolher.
  // 1- Aparecerá em tela a temperatura lida pelo DHT em Celsius. Caso a leitura do Info_C seja 1 informará também que o sistema de ventilação encontra-se ativo.
  // 2- Aparecerá em tela em porcentagem a umidade do solo lida pelo sensor. Caso a leitura do Info_B seja 1 informará também que o sistema de irrigação encontra-se ativo.
  // 3- Controle manual do sistema de ventilação. O mesmo ficará ativo durante 5 segundos.
  // 4- Controle manual do sistema de irrigação. O mesmo ficará ativo durante 1 segundo.
  
    switch (menu){
      case '0':
      bluetooth.println(F("Bem vindo ao Garduino, informe o que deseja:"));
      bluetooth.println(F("1: Temperatura"));
      bluetooth.println(F("2: Umidade do solo"));
      bluetooth.println(F("3: Ligar cooler"));
      bluetooth.println(F("4: Ligar bomba"));
      
      case '1':
      //Apresentação em tela a Umidade do ar e a Temperatura em Celcius.
      bluetooth.print(F("Temperatura: "));
      bluetooth.print(t);
      bluetooth.println(F(" Cº"));
      if (info_C == 1) {
        bluetooth.println("Sistema de ventilação ligado.");
      }
      break;
      
      case '2':
      // Apresentação em tela da leitura convertida.
      bluetooth.print(F("Umidade do solo: "));
      bluetooth.print(umidadeReal);
      bluetooth.println(F("%"));
      if (info_B == 1){
        bluetooth.println("Sistema de irrigação ligado.");
      } 
      break;
      
      case '3':
      // Controle manual do sistema de ventilação.
      digitalWrite(cooler, LOW);
      bluetooth.println("Sistema de ventilação ligado(durante 5 segundos).");
      delay(5000);
      break;

      case '4' :
      // Controle manual do sistema de irrigação.
      digitalWrite(bomba, LOW);
      bluetooth.println("Sistema de irrigação ligado(durante 1 segundo).");
      delay(1000);
      break;

     }
 }
}
