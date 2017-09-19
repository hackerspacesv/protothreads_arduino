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
}
