#include <Keypad.h>
// Configuração do teclado 4x4
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; // R1, R2, R3, R4
5
byte colPins[COLS] = {5, 4, 3, 2}; // C1, C2, C3, C4
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// Pinos do Arduino
const int LED_VERDE = 11;
const int LED_VERMELHO = 12;
const int BUZZER = 10;
String senhaCorreta = "1234";
String senhaDigitada = "";
int tentativas = 0;
bool sistemaArmado = true;
bool alarmeAtivado = false;
unsigned long alarmeInicio = 0;
const unsigned long duracaoAlarme = 10000; // 10 segundos
void setup() {
Serial.begin(9600);
pinMode(LED_VERDE, OUTPUT);
pinMode(LED_VERMELHO, OUTPUT);
pinMode(BUZZER, OUTPUT);
digitalWrite(LED_VERDE, LOW);
digitalWrite(LED_VERMELHO, HIGH); // Sistema inicia armado
digitalWrite(BUZZER, LOW);
6
Serial.println("Sistema ARMADO. Digite a senha.");
}
void loop() {
if (alarmeAtivado) {
// Piscar LED vermelho e tocar buzzer
if ((millis() / 300) % 2 == 0) {
digitalWrite(LED_VERMELHO, HIGH);
digitalWrite(BUZZER, HIGH);
} else {
digitalWrite(LED_VERMELHO, LOW);
digitalWrite(BUZZER, LOW);
}
if (millis() - alarmeInicio >= duracaoAlarme) {
alarmeAtivado = false;
tentativas = 0;
sistemaArmado = true;
digitalWrite(LED_VERMELHO, HIGH);
digitalWrite(BUZZER, LOW);
Serial.println("Alarme finalizado. Sistema ARMADO.");
}
return;
}
char tecla = keypad.getKey();
if (tecla) {
Serial.print("Tecla: ");
Serial.println(tecla);
7
if (tecla == '#') {
// Confirma a senha
if (senhaDigitada == senhaCorreta) {
sistemaArmado = false;
digitalWrite(LED_VERDE, HIGH);
digitalWrite(LED_VERMELHO, LOW);
Serial.println("Senha correta! Sistema DESARMADO.");
} else {
tentativas++;
Serial.print("Senha incorreta! Tentativa ");
Serial.println(tentativas);
senhaDigitada = "";
if (tentativas >= 3) {
alarmeAtivado = true;
alarmeInicio = millis();
Serial.println("ALERTA! Alarme ativado.");
}
}
senhaDigitada = "";
}
else if (tecla == '*') {
// Rearma o sistema
sistemaArmado = true;
senhaDigitada = "";
tentativas = 0;
digitalWrite(LED_VERDE, LOW);
digitalWrite(LED_VERMELHO, HIGH);
8
Serial.println("Sistema ARMADO novamente.");
}
else if (tecla >= '0' && tecla <= '9') {
// Adiciona número à senha
if (senhaDigitada.length() < 6) {
senhaDigitada += tecla;
Serial.print("Senha parcial: ");
Serial.println(senhaDigitada);
}
}
}
}
