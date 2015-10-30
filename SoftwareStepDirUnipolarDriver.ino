/* 
  Arduino-based step/dir driver for unipolar motors connected via ULN2003.
  
  Only 2 axis supported at the moment.
  
  I don't have special motor drivers, only my own DIY ULN2003 based. 
  To use my motors with "standart" cnc controllers I had to make step/dir driver out of Arduino 
  and put it in-between controller (Arduino with GRBL for example) and my ULN2003 driver.

  This step/dir driver written in 30 mins, there was no intention to make optimized code.

 |----------------|          |------------ | -- coil 1 ->  |-------------| ->
 | CNC Controller | --step-> | This driver | -- coil 2 ->  | ULN2003/2803| ->  Unipolar motor
 | (GRBL or any)  | --dir -> | @Arduino Uno| -- coil 3 ->  | or other    | -> 
 |----------------|          |-------------| -- coil 4 ->  |-------------| ->
 
  Input: step and dir signals
  Output: motor coil steps

  Motor enable not supported yet.
  Motors shut down after 2000 ms of inactivity.

  TODO: 
    - motor enable pin (make it simple, one pin for all motors)
    - pwm to hold motors in plase instead of shutdown (PWM for common VCC pin)
    - microstepping with full PWM for coils. Only via software PWM
  
  Author: Anton Viktorov, latonita@yandex.ru

*/

// input pins
#define M0_STEP 2  // uno has pins 2&3 available for interrupt
#define M1_STEP 3  // uno has pins 2&3 available for interrupt
#define M0_DIR  4
#define M1_DIR  5

//#define M_ENABLE xxx // active = low

// output pins
unsigned char M0[] = {6,7,8,9};
unsigned char M1[] = {10,11,12,13};

#define SHUTDOWN_TIME 2000 //ms to shutdown motor

#define NUM_STEPS 8
#define steps halfSteps

////simple. not for real use. not tested.
//unsigned char basicSteps[4][4] = {
//{HIGH, LOW, LOW, LOW},
//{LOW,  LOW, HIGH, LOW},
//{LOW, HIGH, LOW, LOW},
//{LOW,  LOW, LOW, HIGH}
//};


//// full 2-2. not tested, might need to switch second and third position in each line
//unsigned char fullSteps[4][4] = {
//{HIGH,  LOW, HIGH,  LOW},
//{HIGH,  LOW,  LOW, HIGH},
//{ LOW, HIGH,  LOW, HIGH},
//{ LOW, HIGH, HIGH,  LOW}
//};

// half-steps
unsigned char halfSteps[8][4] ={
{ LOW,  LOW,  LOW, HIGH},
{ LOW, HIGH,  LOW, HIGH},
{ LOW, HIGH,  LOW,  LOW},
{ LOW, HIGH, HIGH,  LOW},
{ LOW,  LOW, HIGH,  LOW},
{HIGH,  LOW, HIGH,  LOW},
{HIGH,  LOW,  LOW,  LOW},
{HIGH,  LOW,  LOW, HIGH}
};

unsigned char dead[4] = {LOW, LOW, LOW, LOW};

void setup() {
  pinMode(M0_STEP,INPUT);
  pinMode(M1_STEP,INPUT);
  pinMode(M0_DIR,INPUT);
  pinMode(M1_DIR,INPUT);
//  pinMode(M_ENABLE,INPUT);
  
  pinMode(M0[0],OUTPUT);
  pinMode(M0[1],OUTPUT);
  pinMode(M0[2],OUTPUT);
  pinMode(M0[3],OUTPUT);

  pinMode(M1[0],OUTPUT);
  pinMode(M1[1],OUTPUT);
  pinMode(M1[2],OUTPUT);
  pinMode(M1[3],OUTPUT);
  
  setCoils(M0,dead);
  setCoils(M1,dead);

  attachInterrupt(digitalPinToInterrupt(M0_STEP),ISR0,RISING);
  attachInterrupt(digitalPinToInterrupt(M1_STEP),ISR1,RISING);
}

unsigned char m0_step = 0;
volatile long m0_lastrun = 0;

unsigned char m1_step = 0;
volatile long m1_lastrun = 0;

//unsigned char stepMade = 0;

void ISR0(){
//    if (digitalRead(M_ENABLE) == HIGH)
//      return;
      
    if (digitalRead(M0_DIR) == HIGH) {
      m0_step = ++m0_step % NUM_STEPS;
    } else {
      m0_step = --m0_step % NUM_STEPS;
    }
    setCoils(M0,steps[m0_step]);
    m0_lastrun = millis();
}

void ISR1(){
//    if (digitalRead(M_ENABLE) == HIGH)
//      return;

    if (digitalRead(M1_DIR) == HIGH) {
      m1_step = ++m1_step % NUM_STEPS;
    } else {
      m1_step = --m1_step % NUM_STEPS;
    }
    setCoils(M1,steps[m1_step]);
    m1_lastrun = millis();
}

void loop() {
  if (millis() > m0_lastrun + SHUTDOWN_TIME) {
    setCoils(M0,dead);
  }
  if (millis() > m1_lastrun + SHUTDOWN_TIME) {
    setCoils(M1,dead);
  }
}


void setCoils(unsigned char* m, unsigned char* phase) {
  digitalWrite(m[0],phase[0]);
  digitalWrite(m[1],phase[1]);
  digitalWrite(m[2],phase[2]);
  digitalWrite(m[3],phase[3]);
}
