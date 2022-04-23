#include <Arduino.h>
#include "DigiKeyboard.h"
#include "Credential\Credential.h"
#include "Keypad\Keypad.h"

#define RED_PIN 0
#define GREEN_PIN 1
#define KEYPAD_PIN A1

void setup()
{
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(KEYPAD_PIN, INPUT);
}

void loop()
{
  bool isKeyDown = Keypad_IsKeyDown();
  uint8_t key = Keypad_GetKey();

  if (true == Credential_IsLogged())
  {
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);
    if (true == isKeyDown)
    {
      DigiKeyboard.sendKeyStroke(0);
      DigiKeyboard.print(Credential_GetName(key));
      DigiKeyboard.print('\t');
      DigiKeyboard.print(Credential_GetPassword(key));
      digitalWrite(RED_PIN, HIGH);
    }
  }
  else
  {
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(RED_PIN, HIGH);
    if (true == isKeyDown)
    {
      Credential_PutKey(key);
      digitalWrite(GREEN_PIN, HIGH);
    }
  }
  
  Credential_Task();
  Keypad_Load(analogRead(KEYPAD_PIN));
  DigiKeyboard.delay(50);
}