#include <Bounce2.h>

int SENSOR_IN_LOW = 2;
int SENSOR_IN_HIGH = 3;
int SENSOR_OUT_LOW = 4;
int SENSOR_OUT_HIGH = 5;
// LOW=on, HIGH=off
int VALVE_IN = 8;
int VALVE_OUT = 9;
int PUMP_MOTOR = 10;
int LED = 13;
// для упрощения "понимания"
int ON = LOW;
int OFF = HIGH;
int CLOSED = LOW;
int OPENED = HIGH;

Bounce sensor_in_low = Bounce(); 
Bounce sensor_in_high = Bounce(); 
Bounce sensor_out_low = Bounce(); 
Bounce sensor_out_high = Bounce(); 

// pump - solid type relay
void pump_control(int pump, int operation) {
  digitalWrite(pump, operation);
  digitalWrite(LED, !operation);
}
void pump_on() {
  pump_control(PUMP_MOTOR, ON);
}
void pump_off() {
  pump_control(PUMP_MOTOR, OFF);
}
void valve_control(int valve, int operation) {
  digitalWrite(valve, operation);
}
void valve_in_on() {
  valve_control(VALVE_IN, ON);
}
void valve_in_off() {
  valve_control(VALVE_IN, OFF);
}
void valve_out_on() {
  valve_control(VALVE_OUT, ON);
}
void valve_out_off() {
  valve_control(VALVE_OUT, OFF);
}
boolean inFull() {
  sensor_in_high.update();
  return sensor_in_high.read() == CLOSED; // закорочен на землю
}
boolean inEmpty() {
  sensor_in_low.update();
  return sensor_in_low.read() == OPENED; // разомкнут
}
boolean outFull() {
  sensor_out_high.update();
  return sensor_out_high.read() == CLOSED; // закорочен на землю
}
boolean outEmpty() {
  sensor_out_low.update();
  return sensor_out_low.read() == OPENED; // разомкнут
}
void fill_in() {
    valve_in_on();
    pump_on();
}
void fill_out() {
    valve_out_on();
    pump_on();
}
void setup()
{
  // датчики уровня бочек
  pinMode (SENSOR_IN_LOW, INPUT);
  pinMode (SENSOR_IN_HIGH, INPUT);
  pinMode (SENSOR_OUT_LOW, INPUT);
  pinMode (SENSOR_OUT_HIGH, INPUT);
  // клапаны
  pinMode (VALVE_IN, OUTPUT);
  pinMode (VALVE_OUT, OUTPUT);
  pinMode (PUMP_MOTOR, OUTPUT);
  // светодиод
  pinMode (LED, OUTPUT);
  // конфиг классов по удалению дребезга контактов
  sensor_in_low.attach(SENSOR_IN_LOW);
  sensor_in_low.interval(10);
  sensor_in_high.attach(SENSOR_IN_HIGH);
  sensor_in_high.interval(10);
  sensor_out_low.attach(SENSOR_OUT_LOW);
  sensor_out_low.interval(10);
  sensor_out_high.attach(SENSOR_OUT_HIGH);
  sensor_out_high.interval(10);
  // подтяжка железная. резисторы 15К на +3.3V

  // все выключаем
  pump_off();
  valve_in_off();
  valve_out_off();
  
  // заполняем все если чтото не полное
  if (! inFull()) fill_in();
  if (! outFull()) fill_out();
}

void loop()
{
  // если оба полные - то выключае насос
  if (inFull() && outFull()) pump_off();
  // если какой-то один то только клапан
  if (inFull()) valve_in_off();
  if (outFull()) valve_out_off();
  // если какой-то бак пуст - то включаем и насос и клапан
  if (inEmpty()) fill_in();
  if (outEmpty()) fill_out();
}
