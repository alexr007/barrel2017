#include <Bounce2.h>

int SENSOR_IN_LOW = 2;
int SENSOR_IN_HIGH = 3;
int SENSOR_OUT_LOW = 4;
int SENSOR_OUT_HIGH = 5;
int VALVE_IN = 6;
int VALVE_OUT = 7;
int PUMP_MOTOR = 8;
int DEBUG = 9;
int LED = 13;

Bounce sensor_in_low = Bounce(); 
Bounce sensor_in_high = Bounce(); 
Bounce sensor_out_low = Bounce(); 
Bounce sensor_out_high = Bounce(); 

// pump - solid type relay
void pump_control(int pump, int operation)
{
  digitalWrite(pump, operation);
  digitalWrite(LED, operation);
}
void pump_on()
{
  pump_control(PUMP_MOTOR, HIGH);
}

void pump_off()
{
  pump_control(PUMP_MOTOR, LOW);
}
void valve_control(int valve, int operation)
{
  digitalWrite(valve, operation);
}
void valve_in_on()
{
  valve_control(VALVE_IN, HIGH);
}
void valve_in_off()
{
  valve_control(VALVE_IN, LOW);
}
void valve_out_on()
{
  valve_control(VALVE_OUT, HIGH);
}
void valve_out_off()
{
  valve_control(VALVE_OUT, LOW);
}
boolean inFull()
{
  sensor_in_high.update();
  return sensor_in_high.read() == HIGH; // закорочен на землю
}
boolean inEmpty()
{
  sensor_in_low.update();
  return sensor_in_low.read() == LOW; // разомкнут
}
boolean outFull()
{
  sensor_out_high.update();
  return sensor_out_high.read() == HIGH; // закорочен на землю
}
boolean outEmpty()
{
  sensor_out_low.update();
  return sensor_out_low.read() == LOW; // разомкнут
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
  // насос
  pinMode (PUMP_MOTOR, OUTPUT);
  // тест
  pinMode (DEBUG, OUTPUT);
  // светодиод
  pinMode (LED, OUTPUT);
  // подтяжка
	//digitalWrite(SENSOR_IN_LOW ,HIGH);
	//digitalWrite(SENSOR_IN_HIGH ,HIGH);
  //digitalWrite(SENSOR_IN_LOW ,HIGH);
  //digitalWrite(SENSOR_IN_HIGH ,HIGH);
  
  // конфиг классов по удалению дребезга контактов
  sensor_in_low.attach(SENSOR_IN_LOW);
  sensor_in_low.interval(10);
  
  sensor_in_high.attach(SENSOR_IN_HIGH);
  sensor_in_high.interval(10);
  
  sensor_out_low.attach(SENSOR_OUT_LOW);
  sensor_out_low.interval(10);
  
  sensor_out_high.attach(SENSOR_OUT_HIGH);
  sensor_out_high.interval(10);
/*
  pump_off();
  if (! inFull()) fill_in();
  if (! outFull()) fill_out();
*/  
  int v = HIGH;
  digitalWrite(VALVE_IN ,v);
  digitalWrite(VALVE_OUT ,v);
  digitalWrite(PUMP_MOTOR ,v);
  digitalWrite(DEBUG ,v);
}

void loop()
{
  
  /*
  if (inFull() && outFull()) pump_off();
  if (inFull()) valve_in_off();
  if (outFull()) valve_out_off();
  if (inEmpty()) fill_in();
  if (outEmpty()) fill_out();
  */
}
