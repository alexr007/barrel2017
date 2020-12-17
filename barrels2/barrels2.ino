#include <Bounce2.h>

// sensors, input ports
#define SENSOR_IN_LOW   2
#define SENSOR_IN_HIGH  3
#define SENSOR_OUT_LOW  4
#define SENSOR_OUT_HIGH 5

// relays, output ports
#define VALVE_IN        8
#define VALVE_OUT       9
#define PUMP_MOTOR      10

// pump aliases
#define ON    LOW
#define OFF   HIGH

// sensor aliases
#define CLOSED LOW     // short circuited to ground
#define OPENED HIGH    // open, pulled up to +5V

// bouce time in milliseconds
#define bounce 10

// current valve status
bool in_is_filling;
bool out_is_filling;

Bounce sensor_in_low   = Bounce(); 
Bounce sensor_in_high  = Bounce(); 
Bounce sensor_out_low  = Bounce(); 
Bounce sensor_out_high = Bounce(); 

void update_all_sensors() {
  sensor_in_low  .update();
  sensor_in_high .update();
  sensor_out_low .update();
  sensor_out_high.update();
}

void pump_on()         { digitalWrite(PUMP_MOTOR, ON);  }
void pump_off()        { digitalWrite(PUMP_MOTOR, OFF); }
void valve_in_open()   { digitalWrite(VALVE_IN,   ON);  }
void valve_in_close()  { digitalWrite(VALVE_IN,   OFF); }
void valve_out_open()  { digitalWrite(VALVE_OUT,  ON);  }
void valve_out_close() { digitalWrite(VALVE_OUT,  OFF); }

bool is_in_full()   { return sensor_in_high .read() == CLOSED; }
bool is_in_empty()  { return sensor_in_low  .read() == OPENED; }
bool is_out_full()  { return sensor_out_high.read() == CLOSED; }
bool is_out_empty() { return sensor_out_low .read() == OPENED; }

void stop_in()  { valve_in_close();  in_is_filling  = false; }
void stop_out() { valve_out_close(); out_is_filling = false; }
void fill_in()  { in_is_filling  = true; valve_in_open();  pump_on(); }
void fill_out() { out_is_filling = true; valve_out_open(); pump_on(); }

void do_business() {
  if ( is_in_full()  ) stop_in();
  if ( is_out_full() ) stop_out();
  if (!in_is_filling && !out_is_filling) pump_off();
  
  if ( is_in_empty()  ) fill_in();
  if ( is_out_empty() ) fill_out();
}

void setup() {
  // barrels' sensors
  pinMode (SENSOR_IN_LOW, INPUT);
  pinMode (SENSOR_IN_HIGH, INPUT);
  pinMode (SENSOR_OUT_LOW, INPUT);
  pinMode (SENSOR_OUT_HIGH, INPUT);
  // valves
  pinMode (VALVE_IN, OUTPUT);
  pinMode (VALVE_OUT, OUTPUT);
  pinMode (PUMP_MOTOR, OUTPUT);
  // protection on contact bouncing
  sensor_in_low.attach(SENSOR_IN_LOW);
  sensor_in_low.interval(bounce);
  sensor_in_high.attach(SENSOR_IN_HIGH);
  sensor_in_high.interval(bounce);
  sensor_out_low.attach(SENSOR_OUT_LOW);
  sensor_out_low.interval(bounce);
  sensor_out_high.attach(SENSOR_OUT_HIGH);
  sensor_out_high.interval(bounce);
  // hardware pull up, resistor 15K to +3.3V
 
  // all off
  pump_off();
  stop_in();
  stop_out();
}

void loop() {
  update_all_sensors();
  do_business();
  delay(200);
}
