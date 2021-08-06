#include <Stepper.h>
#define STEPS 100
#define LED_PIN 5
#define stepPin1 8
#define stepPin2 10
#define stepPin3 9
#define stepPin4 11


Stepper small_stepper(STEPS, stepPin1, stepPin2, stepPin3, stepPin4); //Stepper(steps, pin1, pin2, pin3, pin4) 

int stepUnit = 4096;
int Steps2Take = 0;
unsigned long runtime=0;
unsigned long Loopruntime =0;
int CounterMIN=0;
int CounterHR=0;
bool CounterAMPM = true;



void time_measure_for_steps(int steps){
    delay(660);
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

void denergiseStepper()
{
digitalWrite(stepPin1, LOW);
digitalWrite(stepPin2, LOW);
digitalWrite(stepPin3, LOW);
digitalWrite(stepPin4, LOW);
Serial.println("\nMotor is now Denergised... ");
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
    Loopruntime = millis();
    digitalWrite(5, HIGH);
    delay(99);
    Serial.println("Current CounterMIN: ");
    Serial.println(CounterMIN);
    Serial.println("Start Loopruntime: ");
    Serial.println(Loopruntime);
    if(CounterAMPM == true){
        if(CounterMIN <= 20){
            Serial.println("\nMotor is now Energising... ");
            time_measure_for_steps(stepUnit);
            time_measure_for_steps(-stepUnit); //~30 seconds, +1 on counter, 10 minutes is 20 on the counter
            counter();
        }
        else if(CounterMIN <= 120){
            denergiseStepper();
            LED_toggle_30(LED_PIN); //30 seconds, 15 on, 15 off on LED
            counter();
        }
    }
    else if(CounterAMPM == false){
        denergiseStepper();
        wait12hrs();
        flipAMPM();
    }
    else{
        Serial.println("\nCode should never get to here...You dummy");
        //Serial.println("\n counterMIN = %d" ,CounterMIN);
        //Serial.println("\n counterMIN = %d" ,CounterHR);
        //Serial.println("\n counterMIN = %d" ,CounterAMPM);
    }
    Loopruntime = millis();
    Serial.println("Loopruntime: ");
    Serial.println(Loopruntime);
}
