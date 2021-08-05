#include <Stepper.h>
#define STEPS 100
#define LED_PIN 5

Stepper small_stepper(STEPS, 8, 10, 9, 11); //Pins

int stepUnit = 4096;
int Steps2Take = 0;
unsigned long runtime=0;

int CounterMIN=0;
int CounterHR=0;
bool CounterAMPM = true;


void time_measure_for_steps(int steps){
    delay(800);
    Steps2Take = steps; //2048 for 1 rotation, 1 rotation ever 4.5seconds
    //Therefore to turn backwards 6 times, Multiply -6/30
    runtime = millis();
    small_stepper.step(Steps2Take);
    runtime = millis() - runtime;
    Serial.println("\nRuntime: ");
    Serial.println(runtime);
    delay(2000);
}

void LED_toggle_30(int pin){
    Serial.println("\nStopping until next hour...");
    digitalWrite(pin, LOW);
    delay(15000);
    digitalWrite(pin, HIGH);
    delay(15000);
}

void counter(){
    CounterMIN++;
    if(CounterMIN == 120){  //120 due to 30 seconds for 2 rotations (CCW and CW), 30seconds delay on LED_toggle
        CounterMIN = 0;
        CounterHR++;
    }
    else if(CounterHR == 12){
        CounterHR = 0;
        flipAMPM();
    }
}

void flipAMPM(){
    CounterAMPM = !CounterAMPM;
}

void wait12hrs(){
    Serial.println("\nEntering Sleep Mode...");
    delay(720000);
     Serial.println("\nWake from Sleep Mode...");
}

void setup(){
    Serial.begin(9600); //9600 BPS
    Serial.println("\nMotor Initilising...");
    pinMode(LED_PIN, OUTPUT);
    Serial.println("\nLED Pin Assigned...");
}

void loop(){
    digitalWrite(5, HIGH);
    delay(100);

    Serial.println("\nMotor is now energising... ");
    Serial.println(CounterMIN);
    small_stepper.setSpeed(200); //300 is the max speed supported by driver/stepper combo
    if(CounterAMPM == true){
        if(CounterMIN <= 20){
            time_measure_for_steps(stepUnit);
            time_measure_for_steps(-stepUnit); //~30 seconds, +1 on counter, 10 minutes is 20 on the counter
            counter();
        }
        else if(CounterMIN <= 120){
            LED_toggle_30(LED_PIN); //30 seconds, 15 on, 15 off on LED
            counter();
        }
    }
    else if(CounterAMPM == false){
        wait12hrs();
        flipAMPM();
    }
    else{
        Serial.println("\nCode should never get to here...You dummy");
        //Serial.println("\n counterMIN = %d" ,CounterMIN);
        //Serial.println("\n counterMIN = %d" ,CounterHR);
        //Serial.println("\n counterMIN = %d" ,CounterAMPM);
    }
}