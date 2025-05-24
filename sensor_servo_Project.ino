#include <LiquidCrystal.h>
#include "SR04.h"
#include <Servo.h>

#define TRIG_PIN 12
#define ECHO_PIN 11
#define LED_PIN1 5
#define LED_PIN2 6
#define BUZZER_PIN 13
#define SERVO_PIN 9
#define TRIGGER_DISTANCE 30  // cm

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
Servo myservo;
LiquidCrystal lcd(4, 3, 7, 10, 2, 8);  // RS, E, D4, D5, D6, D7

long distance;
int pos = 0;

void setup() {
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  myservo.attach(SERVO_PIN);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  
  Serial.begin(9600);
  delay(1000);
  lcd.clear();
}

void loop() {
  // Rotate servo forward
  for (pos = 0; pos <= 180; pos += 2) {
    myservo.write(pos);
    delay(15);
    checkSensor();
  }

  // Rotate servo backward
  for (pos = 180; pos >= 0; pos -= 2) {
    myservo.write(pos);
    delay(15);
    checkSensor();
  }
}

void checkSensor() {
  distance = sr04.Distance();

  if (distance < TRIGGER_DISTANCE && distance > 2) {
    // Object detected
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, LOW);
    
    // Display distance on LCD
    lcd.setCursor(0, 0);
    lcd.print("                "); // Clear line
    lcd.setCursor(0, 0);
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print(" cm");

    // Serial and buzzer
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println(" cm");

    tone(BUZZER_PIN, 300);
    delay(300);
    noTone(BUZZER_PIN);
    delay(50);
  } else {
    // No object detected
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, HIGH);

    lcd.setCursor(0, 0);
    lcd.print("                "); // Clear line
    lcd.setCursor(0, 0);
    lcd.print("Nothing detected");
  }
}
