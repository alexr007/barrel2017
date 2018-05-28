#include <Bounce2.h>

// connections
#define SENSOR_IN_LOW   2
#define SENSOR_IN_HIGH  3
#define SENSOR_OUT_LOW  4
#define SENSOR_OUT_HIGH 5
#define VALVE_IN        8
#define VALVE_OUT       9
#define LED             13
#define PUMP_MOTOR      10
// LOW=on, HIGH=off
#define ON    LOW
#define OFF   HIGH
// to easy understanding
// sensor values
#define CLOSED LOW
#define OPENED HIGH

bool bFillingIn = false;
bool bFillingOut = false;

Bounce sensor_in_low = Bounce(); 
Bounce sensor_in_high = Bounce(); 
Bounce sensor_out_low = Bounce(); 
Bounce sensor_out_high = Bounce(); 

// pump - SSR (solid type relay)
void pump_control(int pump, int operation) {
  // relay switches ON (LOW)
  digitalWrite(pump, operation);
  // led lights (HIGH)
  digitalWrite(LED, !operation);
}

// switches main (weil) pump ON
void pump_on() {
  pump_control(PUMP_MOTOR, ON);
}

// switches main (weil) pump OFF
void pump_off() {
  pump_control(PUMP_MOTOR, OFF);
}

// general valve operation
void valve_control(int valve, int operation) {
  digitalWrite(valve, operation);
}

// switches indoor pump ON
void valve_in_on() {
  valve_control(VALVE_IN, ON);
}

// switches indoor pump OFF
void valve_in_off() {
  valve_control(VALVE_IN, OFF);
}

// switches outdoor pump ON
void valve_out_on() {
  valve_control(VALVE_OUT, ON);
}

// switches outdoor pump OFF
void valve_out_off() {
  valve_control(VALVE_OUT, OFF);
}

// close corresponding valve and set variable to false
void stop_in() {
  valve_in_off();
  bFillingIn = false;
}

// close corresponding valve and set variable to false
void stop_out() {
  valve_out_off();
  bFillingOut = false;
}

// updates the sensor's value and return true it it=closed
bool inFull() {
  sensor_in_high.update();
  return sensor_in_high.read() == CLOSED;
}

// updates the sensor's value and return true it it=opened
bool inEmpty() {
  sensor_in_low.update();
  return sensor_in_low.read() == OPENED;
}

// updates the sensor's value and return true it it=closed
bool outFull() {
  sensor_out_high.update();
  return sensor_out_high.read() == CLOSED; // закорочен на землю
}

// updates the sensor's value and return true it it=opened
bool outEmpty() {
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

void setup() {
  // barrels's sensors
  pinMode (SENSOR_IN_LOW, INPUT);
  pinMode (SENSOR_IN_HIGH, INPUT);
  pinMode (SENSOR_OUT_LOW, INPUT);
  pinMode (SENSOR_OUT_HIGH, INPUT);
  // valves
  pinMode (VALVE_IN, OUTPUT);
  pinMode (VALVE_OUT, OUTPUT);
  pinMode (PUMP_MOTOR, OUTPUT);
  // led
  pinMode (LED, OUTPUT);
  // protection on contact bouncing
  sensor_in_low.attach(SENSOR_IN_LOW);
  sensor_in_low.interval(10);
  sensor_in_high.attach(SENSOR_IN_HIGH);
  sensor_in_high.interval(10);
  sensor_out_low.attach(SENSOR_OUT_LOW);
  sensor_out_low.interval(10);
  sensor_out_high.attach(SENSOR_OUT_HIGH);
  sensor_out_high.interval(10);
  // hardware pull up, resistor 15K to +3.3V
 
  // all off
  pump_off();
  stop_in();
  stop_out();
  
  // try to fill indoor
  if (!inFull()) {
    fill_in();
  }
  // try to fill outdoor
  if (!outFull()) {
    fill_out();
  }
}

void loop() {
  // stop if necessary
  if (bFillingIn && inFull()) stop_in();
  if (bFillingOut && outFull()) stop_out();
  if (!bFillingIn && !bFillingOut) pump_off();
  
  // if someone barrel is empty - we switch on corresponding pump and valve
  if (inEmpty()) fill_in();
  if (outEmpty()) fill_out();

  delay(300);
}
