# SoftwareStepDirUnipolarDriver
Arduino-based step/dir driver for unipolar motors connected via ULN2003 or similar circuits.

  Only 2 axis supported at the moment.
  
  I don't have special motor drivers, only my own DIY ULN2003 based. 
  To use my motors with "standart" cnc controllers I had to make step/dir driver out of Arduino 
  and put it in-between controller (Arduino with GRBL for example) and my ULN2003 driver.

  This step/dir driver written in 30 mins, there was no intention to make optimized code.

` |----------------|          |------------ | -- coil 1 ->  |-------------| -> `
` | CNC Controller | --step-> | This driver | -- coil 2 ->  | ULN2003/2803| ->  Unipolar motor `
` | (GRBL or any)  | --dir -> | @Arduino Uno| -- coil 3 ->  | or other    | -> `  
` |----------------|          |-------------| -- coil 4 ->  |-------------| -> `
 
  Input: step and dir signals
  Output: motor coil steps x2

  Motor enable not supported yet.
  Motors shut down after 2000 ms of inactivity.

  TODO: 
    - motor enable pin (make it simple, one pin for all motors)
    - pwm to hold motors in plase instead of shutdown (PWM for common VCC pin)
    - microstepping with full PWM for coils. Only via software PWM
  
  Author: Anton Viktorov, latonita@yandex.ru


