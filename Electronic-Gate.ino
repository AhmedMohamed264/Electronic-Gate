#include <Servo.h>
Servo door_motor;
int enteranceLdr = A5;
int exitLdr = A3;
int button = 4;
int greenLed = 2;
int redLed = 9;
int buzzer = 7;
int enteranceLdrValue = 0;
int exitLdrValue = 0;
int number_of_persons = 0;
int power = 0;
int num = 0;
void setup() {
  pinMode(enteranceLdr, INPUT);
  pinMode(exitLdr, INPUT);
  pinMode(button, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  door_motor.attach(5);
  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(button) == HIGH && power == 0)
  {
    while (digitalRead(button) == HIGH);
    digitalWrite(greenLed, HIGH);
    power = 1;
  }
  else if (digitalRead(button) == HIGH && power == 1)
  {
    while (digitalRead(button) == HIGH);
    digitalWrite(greenLed, LOW);
    power = 0;
  }
  delay(50);
  if (power == 1)
  {
    if ((analogRead(enteranceLdr) < 300) && (analogRead(exitLdr) > 170) && (enteranceLdrValue == 0) && (exitLdrValue == 0))
      enteranceLdrValue = 1;
    delay(100);
    if ((analogRead(enteranceLdr) > 300) && (analogRead(exitLdr) < 170) && (enteranceLdrValue == 1) &&  (exitLdrValue == 0))
    {
      while (analogRead(exitLdr) < 170);
      number_of_persons++;
      Serial.print("Total of persons inside the room: ");
      Serial.print(number_of_persons);
      Serial.print("\n");
      enteranceLdrValue = 0;
      exitLdrValue = 0;
      door_motor.write(90);
      delay(20);
    }
    if ((analogRead(exitLdr) < 170) && (analogRead(enteranceLdr) > 300) && (enteranceLdrValue == 0) && (exitLdrValue == 0))
      exitLdrValue = 1;
    delay(100);
    if ((analogRead(exitLdr) > 170) && (analogRead(enteranceLdr) < 300) && (exitLdrValue == 1) &&  (enteranceLdrValue == 0))
    {
      while (analogRead(enteranceLdr) < 300);
      if (number_of_persons > 0)
      {
        number_of_persons--;
        Serial.print("Total of persons inside the room: ");
        Serial.print(number_of_persons);
        Serial.print("\n");
      }
      enteranceLdrValue = 0;
      exitLdrValue = 0;
      door_motor.write(90);
      delay(20);
    }
    if (number_of_persons > 5)
    {
      Serial.print("ERROR: Number of persons exceeded 5 persons in the room!\n");
      tone(buzzer, 1000);
      digitalWrite(redLed, HIGH);
    }
    else
    {
      noTone(buzzer);
      digitalWrite(redLed, LOW);
    }
    door_motor.write(180);
    delay(20);
  }
  else
  {
    noTone(buzzer);
    digitalWrite(redLed, LOW);
    digitalWrite(redLed, LOW);
    number_of_persons = 0;
    enteranceLdrValue = 0;
    exitLdrValue = 0;
    door_motor.write(180);
    delay(20);
  }
  delay(50);
}
