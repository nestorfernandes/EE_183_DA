#include "user_interface.h"

os_timer_t myTimer;

bool tickOccured;

const short int inPin = D3;        // the number of the input pin
const short int outPin1 = D5;     // the number of the output pin
const short int outPin2 = D6;
const short int outPin3 = D7;
 
int LEDstate1 = LOW;// the current state of the output pin
int LEDstate2 = LOW;
int LEDstate3 = LOW;

int reading;           // the current reading from the input pin
int previous;   // the previous reading from the input pin
const int delayBetweenReadings = 2;
int delayCounter;
bool delaying;

int currentSeq;
int currentPos;
const int numOfSeq = 3;
const int numOfLED = 3;
const int sequenceLength = 16;
int seq[numOfSeq][numOfLED][sequenceLength] = {{{1,0,0,1,1,0,1,1,1,0,0,0,1,0,1,1},
                                                {0,0,1,0,1,1,0,0,0,0,1,1,0,1,1,0},
                                                {1,1,0,0,0,1,0,1,0,0,1,1,0,1,0,0}},
                                                
                                               {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
                                                
                                               {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}};

 void timerCallback(void *pArg) {
      tickOccured = true;
      
      if(delaying){
        delayCounter += 1;
        if (delayCounter == delayBetweenReadings){
          delayCounter = 0;
          delaying = false;
        }
      }

      currentPos = (currentPos+1)%sequenceLength;
      digitalWrite(outPin1,seq[currentSeq][0][currentPos]);
      digitalWrite(outPin2,seq[currentSeq][1][currentPos]);
      digitalWrite(outPin3,seq[currentSeq][2][currentPos]);

      return;
}

void interrupt_init(void) {
        os_timer_setfn(&myTimer, timerCallback, NULL);
        os_timer_arm(&myTimer, 1000, true);
}

void state_init(void){
        delayCounter = 0;
        delaying = false;
        currentSeq = 0;
        currentPos = 0;
        previous = HIGH;
}

void setup()
{
pinMode(inPin, INPUT);
digitalWrite(inPin, LOW); // turn on the built in pull-up resistor
pinMode(outPin1, OUTPUT);
pinMode(outPin2, OUTPUT);
pinMode(outPin3, OUTPUT);

 Serial.begin(115200);
 Serial.println();
 Serial.println();
 Serial.println("--------------------------");
 Serial.println("ESP8266 Timer Test With LED");
 Serial.println("--------------------------");
 tickOccured = false;
 interrupt_init();
 state_init();
 
}

void loop()
{

 if (tickOccured == true)
 {
    //Serial.println("Tick Occurred");
    tickOccured = false;
 }

if (delaying){
  previous = digitalRead(inPin);
  return;
}else{
  reading = digitalRead(inPin);
  if (reading != previous){
    delaying = true;
    currentSeq = (currentSeq+1)%numOfSeq;
  }
}

}
