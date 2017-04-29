#include <Bounce2.h>

int SENSOR_LOW = 2;
int SENSOR_HIGH = 3;

int PUMP_MOTOR = 6;
int PUMP_LED = PUMP_MOTOR + 1;

Bounce sensor_low = Bounce(); 
Bounce sensor_high = Bounce(); 

// pump - tt relay
// pump + 1 - conventional relay
void pump_control_on(int pump)
{
  	digitalWrite(pump, HIGH); // TT relay
	delay(50);
	digitalWrite(pump+1, HIGH); // conventional relay
	delay(50);
  	digitalWrite(pump, LOW); // TT relay
}

void pump_control_off(int pump)
{
  	digitalWrite(pump, HIGH); // TT relay
	delay(50);
	digitalWrite(pump+1, LOW); // conventional relay
	delay(50);
  	digitalWrite(pump, LOW); // TT relay
}

void pump_control(int pump, int operation)
{
	digitalWrite(pump, operation);
	digitalWrite(pump+1, operation);
}

void main_pumpOn() {
	pump_control(PUMP_MOTOR, HIGH);
}

void main_pumpOff() {
	pump_control(PUMP_MOTOR, LOW);
}

boolean isFull()
{
  	sensor_high.update();
	return sensor_high.read() == LOW;
}

boolean isEmpty()
{
  	sensor_low.update();
	return sensor_low.read() == HIGH;
}

void control_main_pump()
{
  	if ( isFull() ) main_pumpOff();
  	  else if ( isEmpty() ) main_pumpOn();
}


void setup()
{
	pinMode (SENSOR_LOW, INPUT);
	pinMode (SENSOR_HIGH, INPUT);
	digitalWrite(SENSOR_LOW ,HIGH);
	digitalWrite(SENSOR_HIGH ,HIGH);

        sensor_low.attach(SENSOR_LOW);
        sensor_low.interval(10);

        sensor_high.attach(SENSOR_HIGH);
        sensor_high.interval(10);


	pinMode (PUMP_MOTOR, OUTPUT);
	pinMode (PUMP_LED, OUTPUT);
        
        main_pumpOff();
	if ( !isFull() ) main_pumpOn();
}

void loop()
{
	
	control_main_pump();
}

B