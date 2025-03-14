#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <Servo.h>

const int LOADCELL_DOUT_PIN = 5;  
const int LOADCELL_SCK_PIN = 6;  

const int SERVO_PINS[7] = {7, 8, 9, 10, 11, 12, 13};

const float THRESHOLD1 = 300.0;
const float THRESHOLD2 = 700.0;

Servo servos[7];
LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 scale;

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(2280.f);
  scale.tare();

  for (int i = 0; i < 7; i++) {
    servos[i].attach(SERVO_PINS[i]);
    servos[i].write(0);
  }

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Weight:");
}

void loop() {
  if (scale.is_ready()) {
    float weight = scale.get_units(5);
    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" g");

    lcd.setCursor(7, 0);
    lcd.print("       ");
    lcd.setCursor(7, 0);
    lcd.print(weight, 2);
    lcd.setCursor(13, 0);
    lcd.print("g");

    if (weight < THRESHOLD1) {
      closeAllServos();
      lcd.setCursor(0, 1);
      lcd.print("All: Closed   ");
    } else if (weight >= THRESHOLD1 && weight < THRESHOLD2) {
      closeAllServos();
      servos[0].write(90);
      servos[1].write(90);
      lcd.setCursor(0, 1);
      lcd.print("Group 1: Open ");
    } else if (weight >= THRESHOLD2 && weight <= 1000.0) {
      closeAllServos();
      servos[2].write(90);
      servos[3].write(90);
      lcd.setCursor(0, 1);
      lcd.print("Group 2: Open ");
    } else {
      closeAllServos();
      servos[4].write(90);
      servos[5].write(90);
      servos[6].write(90);
      lcd.setCursor(0, 1);
      lcd.print("Group 3: Open ");
    }
  } else {
    Serial.println("Scale not ready!");
    lcd.setCursor(0, 0);
    lcd.print("Scale not ready!");
  }

  delay(500);
}

void closeAllServos() {
  for (int i = 0; i < 7; i++) {
    servos[i].write(0);
  }
}
