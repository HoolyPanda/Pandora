#include <Arduino.h>
#include <Servo.h>

Servo myservo;

void setup() 
{
    myservo.attach(6);
    Serial.begin(4800);
}
void loop() 
{
    for ( int i = 0; i <= 360; i += 10)
    {
        myservo.write(i);
        Serial.println(i);
        delay(500);
    }
}