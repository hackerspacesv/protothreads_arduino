// Include to use LCD display
//#include <LiquidCrystal.h>
// Include to show
#include <stdio.h>

//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char stringTime[9] = "00:00:00";

void setup() {
  //lcd.begin(16, 2);
  Serial.begin(9600);
  //lcd.print(stringTime);
  Serial.println(stringTime);
}

long elapsedSeconds = 0;

void loop() {
  // Calculate the elapsed time based on total seconds;
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

  // Increase second counter
  elapsedSeconds++;
  delay(1000);
}
