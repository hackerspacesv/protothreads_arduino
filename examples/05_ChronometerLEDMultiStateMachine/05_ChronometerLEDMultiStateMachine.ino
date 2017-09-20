// Include to use LCD display
//#include <LiquidCrystal.h>
// Include to show
#include <stdio.h>

//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int btn1 = 9;
const int btn2 = 10;

void setup() {
  //lcd.begin(16, 2);
  Serial.begin(9600);
  //lcd.print(stringTime);
  Serial.println("00:00:00");
  
  // Initialize Button Input
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
}

long lastTimestamp = 0;
long elapsedSeconds = 0;
long lastEvtBtn1 = 0;
long lastEvtBtn2 = 0;
bool clockEnabled = false;

enum valid_states {
  WAIT = 0,
  DISPLAY_TIME = 1,
  START_STOP_CLOCK = 2,
  RESET = 3
};

int machine_state = 0;

bool tick_flag = false;
bool btn1_flag = false;
bool btn2_flag = false;

const int led1 = 6, led2 = 7, led3 = 8;

enum valid_states2 {
  ALL_OFF         = 0,
  LED1ON_LED2OFF  = 1,
  LED1OFF_LED2ON  = 2,
  LED2OFF_LED3ON  = 3,
  LED3OFF         = 4
};

bool tick_flag2 = false;
long lastTimestamp2 = 0;

int machine2_state = 0;

void displayElapsedTime(long elapsedSeconds) {
  char stringTime[9];
  
  int hours = elapsedSeconds/(60*60);
  int minutes = (elapsedSeconds-(hours*60*60))/60;
  int seconds = elapsedSeconds-(hours*60*60)-(minutes*60);

  // Format time to display
  sprintf(stringTime, "%02d:%02d:%02d", hours, minutes, seconds);

  // Display on LCD
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print(stringTime);
  Serial.println(stringTime);

  // Configuration
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  // Este evento se llama cada segundo
  if((millis()-lastTimestamp)>1000) {
    tick_flag = true;
    lastTimestamp = millis();
  }

  // Eventos generados cada vez que se presiona un botón
  //btn1_flag |= !digitalRead(btn1);
  //btn2_flag |= !digitalRead(btn2);

  // Con eliminador de rebote:
  if((millis()-lastEvtBtn1)>100 && !digitalRead(btn1)) {
    btn1_flag = 1;
    lastEvtBtn1 = millis();
  }
  if((millis()-lastEvtBtn2)>100 && !digitalRead(btn2)) {
    btn2_flag = 1;
    lastEvtBtn2 = millis();
  }
  
  switch(machine_state) {
    case WAIT:
      if(btn1_flag) {
        btn1_flag = false;
        machine_state = START_STOP_CLOCK;
        break;
      }
      if(btn2_flag) {
        btn2_flag = false;
        machine_state = RESET;
        break;
      }
      if(tick_flag) {
        tick_flag = false;
        machine_state = DISPLAY_TIME;
        break;
      } 
      break;
    
    case DISPLAY_TIME:
      // Display on screen or serial
      displayElapsedTime(elapsedSeconds);
      
      // Increase clock only if clockEnabled
      if(clockEnabled)
        elapsedSeconds += 1;

      // automatically change state for next run
      machine_state = WAIT;
      break;

    case START_STOP_CLOCK:
      clockEnabled = !clockEnabled;
      machine_state = WAIT;
      break;
    
    case RESET:
      elapsedSeconds = 0;
      machine_state = WAIT;
      break;
  }

  //-- Start of second machine state.
    // Este evento se llama cada medio segundo
  if((millis()-lastTimestamp2)>500) {
    tick_flag2 = true;
    lastTimestamp2 = millis();
  }
  
  switch(machine2_state) {
    case ALL_OFF:
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        if(tick_flag2) {
          tick_flag2 = false;
          machine2_state = LED1ON_LED2OFF;
          break;
        }
      break;
    case LED1ON_LED2OFF:
        digitalWrite(led1, HIGH);
        digitalWrite(led2, LOW);
        if(tick_flag2) {
          tick_flag2 = false;
          machine2_state = LED1OFF_LED2ON;
          break;
        }
      break;
    case LED1OFF_LED2ON:
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        if(tick_flag2) {
          tick_flag2 = false;
          machine2_state = LED2OFF_LED3ON;
          break;
        }
      break;
    case LED2OFF_LED3ON:
        digitalWrite(led2, LOW);
        digitalWrite(led3, HIGH);
        if(tick_flag2) {
          tick_flag2 = false;
          machine2_state = LED3OFF;
          break;
        }
      break;
    case LED3OFF:
        digitalWrite(led3, LOW);
        if(tick_flag2) {
          tick_flag2 = false;
          machine2_state = ALL_OFF;
          break;
        }
      break;
  }
  
}
