#include <cmath>
#include "mariosong.h"

// #include <Timer>

// #define PI 3.1415926
// Define the pins we're going to call pinMode on

int levels[] = {LOW, LOW};
//int leds[2] ;//= {D6, D7};
int leds[2] = {D6, D7};
unsigned long t;

enum STATE { UNKNOWN, RUNNING_BLUE, RUNNING_RED, FAILED, FAILED_ACK, WIN, WIN_ACK};
enum COLOR { RED, YELLOW, BLUE, WHITE};

STATE current_state;

// This routine runs only once upon reset

void preparePins(){
    pinMode(leds[0], OUTPUT);
    pinMode(leds[1], OUTPUT);
  
    // control RGB LED colours
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
    pinMode(A6, OUTPUT);

    // turn on the RGB LED
    pinMode(D2, OUTPUT);
    digitalWrite(D2, HIGH);
  
    // button input 
    pinMode(D4, INPUT);
    
    //speaker pin
    pinMode(speaker, OUTPUT);
    pinMode(nspeaker, OUTPUT);
}

int winStart;
void setup() {
    Serial.begin(9600);
    current_state = UNKNOWN;
    preparePins();
    reset();
    Spark.function("setState", setState);
    
}

int setState(String state) {
    // Serial.println("Build state: " + state);
    // running, failed, passed
    if (state == "blue_anime") {
        change_state(RUNNING_BLUE);
    } else if (state == "red_anime") {
        change_state(RUNNING_RED);
    } else if (state == "red"){
        change_state(FAILED);
    } else if (state == "blue"){
        change_state(WIN);
    }
    return 1;
}

int scale(int value, int new_max) {
    // new_value = ( (old_value - old_min) / (old_max - old_min) ) * (new_max - new_min) + new_min
    return (value / 255) * new_max;
}

void rgb(int r, int g, int b) {
    // analogWrite values from 0 to 255.
    // analogWrite(A4, scale(r, 126));
    analogWrite(A4, 255 - r);
    analogWrite(A5, 255 - g);
    analogWrite(A6, 255 - b);
    // red 1-165
}

// This routine gets called repeatedly, like once every 5-15 milliseconds.
// Spark firmware interleaves background CPU activity associated with WiFi + Cloud activity with your code. 
// Make sure none of your code delays or blocks for too long (like more than 5 seconds), or weird things can happen.


void reset() {
    // turn off the blink and light
    digitalWrite(D2, LOW);
    digitalWrite(D7, LOW);
    noTone(speaker); 
    winStart = 0;
    // turn off the buzz
}

int lastChange;
void buzz() {
    if (millis() < lastChange) { lastChange = 0; }
    if (millis() - lastChange < 100) { 
        return;
    }
    if (digitalRead(D7) == HIGH) {
        digitalWrite(D7, LOW);
    } else {
        digitalWrite(D7, HIGH);
    }
    lastChange = millis();
}


void blink(int color, int freq=500) {
    digitalWrite(D2, HIGH);

    int *col = get_rgb(color);
    
    int max = 0;
    for(int i = 1; i<sizeof(col)/sizeof(col[0]); i++) {
       if (col[i] > max) {
           max = col[i];
       } 
    }
    
    float mult = sin(millis() / float(freq));
    if (mult < 0) {
        mult = mult * -1;
    }
    rgb(int(col[0] * mult), int(col[1] * mult), int(col[2] * mult));
    delete[] col;
}

int* get_rgb(int color) {
    int* col = new int[3];
    col[0] = 0;
    col[1] = 0;
    col[2] = 0;
    switch(color) {
        case YELLOW:
            col[0] = 126;
            col[1] = 255;
            break;
        case RED:
            col[0] = 255;
            break;
        case BLUE:
            col[2] = 255;
            break;
        case WHITE:
            col[0] = 120;
            col[1] = 255;
            col[2] = 255;
    }
    return col;
}

void light(int color) {
    digitalWrite(D2, HIGH);

    int *col = get_rgb(color);
    rgb(col[0], col[1], col[2]);
    delete[] col;
}

void change_state(int new_state) {
    reset();
    current_state = (STATE)new_state;
}


void playWin() {
    if (winStart == 0){ winStart = millis(); }
    if (millis() - winStart > 5400) {
        change_state(WIN_ACK);
    } else {
        singMarioWin();
    }
}

void playFail(){
    singMarioDeath();
}

void loop() {
    
    if (digitalRead(D4) == HIGH && current_state == FAILED) {
        // light(WHITE);
        delay(100);
        change_state(FAILED_ACK);
        // change_state((current_state + 1) % 5);
        // Serial.println("Boom, the current state is ");
        // Serial.println(current_state);
    }
    
    switch(current_state) {
        case RUNNING_BLUE:
            blink(BLUE);
        break;
        
        case RUNNING_RED:
            blink(RED);
        break;
        
        case FAILED:
            blink(RED, 100);
            buzz();
            playFail();
        break;
        
        case FAILED_ACK:
            light(RED);
        break;
        
        case WIN:
            light(BLUE);
            playWin();
        break;
        
        case WIN_ACK:
            light(BLUE);
        break;
        
        default:
        break;
    }

}

// 1 build status - BLUE, RED - pulsing
// 2 turn off red and sound -> STATE
