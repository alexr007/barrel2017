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
boolean bFillingIn = false;
boolean bFillingOut = false;

Bounce sensor_in_low = Bounce(); 
Bounce sensor_in_high = Bounce(); 
Bounce sensor_out_low = Bounce(); 
Bounce sensor_out_high = Bounce(); 

// pump - solid type relay
void pump_control(int pump, int operation) {
  // реле замыкается LOW
  digitalWrite(pump, operation);
  // светодиод загорается HIGH
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
void stop_in() {
  valve_control(VALVE_IN, OFF);
  bFillingIn = false;
}
void valve_out_on() {
  valve_control(VALVE_OUT, ON);
}
void stop_out() {
  valve_control(VALVE_OUT, OFF);
  bFillingOut = false;
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
    bFillingIn = true;
    valve_in_on();
    pump_on();
}
void fill_out() {
    bFillingOut = true;
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
  stop_in();
  stop_out();
  
  // заполняем все если чтото не полное
  if (! inFull()) fill_in();
  if (! outFull()) fill_out();
}

void loop()
{
  // если никакой не в процессе наполнения - то выключае насос
  if (!bFillingIn && !bFillingOut) pump_off();
  // если какой-то один то только клапан
  if (bFillingIn && inFull()) stop_in();
  if (bFillingOut && outFull()) stop_out();
  // если какой-то бак пуст - то включаем и насос и клапан
  if (inEmpty()) fill_in();
  if (outEmpty()) fill_out();
}
