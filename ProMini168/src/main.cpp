#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Servo myservo;

bool clear = true;

byte masterkey[4] = {0xb7, 0x3d, 0x9e, 0x4b};
byte masterkey1[4] = {0xe7, 0x34, 0x9e, 0x4a};
byte keys[3][4] = {
   {0xb7, 0x3d, 0x9e, 0x4b},
   {0xe7, 0x34, 0x9e, 0x4a},
   {}
};



// memcpy(keys[0], masterkey, 4);
// keys[0] = masterkey;
// keys[1] = masterkey1;
// 1,2 are masterkeys, 3 unic key
void setup() {
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    myservo.attach(6);
    delay(50);
    myservo.write(1);
    delay(500);
    myservo.write(120);
    Serial.begin(4800);		// Initialize serial communications with the PC
    while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();			// Init SPI bus
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
    mfrc522.PCD_Init();		// Init MFRC522
    mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
        Serial.print(' ');
    }
    Serial.print('\n');
}

// masterkey = {0xb7, 0x3d, 0x9e, 0x4b};
boolean isOpened = false;

void loop() {
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( mfrc522.PICC_IsNewCardPresent() ) 
    {
        if ( ! mfrc522.PICC_ReadCardSerial())
        {
            Serial.println("HEHE1");
            return;
        }
        else 
        {
            Serial.println("HEHE2");
        if (clear)
        {
            for (int i = 0; i > 4; i++)
            {
                keys[2][i] = mfrc522.uid.uidByte[i];
            }
            clear = false;
            memcpy(keys[2], mfrc522.uid.uidByte,4);
            dump_byte_array(keys[2],4);
            Serial.println("Key added");
            dump_byte_array(mfrc522.uid.uidByte, 4);
        }
        else
        {
            int errors = 0;
            for ( int key = 0; key < 3; key++)
            {
                for (int pos = 0; pos < 4; pos++)
                {
                    if ((keys[key][pos] != mfrc522.uid.uidByte[pos]))
                    {
                        Serial.println("wrong key");
                        dump_byte_array(keys[key], 4);
                        dump_byte_array(mfrc522.uid.uidByte, 4);
                        errors++;
                        break;
                        // return;
                    }
                }
            }
            if ( errors < 3 )
            {
                Serial.println("OK\n==========");
                if (isOpened)
                {
                }
                else
                {
                    myservo.write(180);
                    isOpened = true; 
                    delay(2000);
                }
                
            }
            else
            {
                Serial.println("WRONG KEY\n========");
            }
        }

            return;
        }
        return;
    }
    else
    {
        if ( ! mfrc522.PICC_IsNewCardPresent())
        {
            myservo.write(1);
            isOpened = false;
            delay(500);
            Serial.println("HERE");
        }
    }
    // digitalWrite(13, LOW);
    // Select one of the cards{}
    if ( ! mfrc522.PICC_ReadCardSerial()) {
        // Serial.println("No key");
        // myservo.write(120);
        // isOpened = false;
        return;
    }

    // mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    // Dump debug info about the card; PICC_HaltA() is automatically called
    // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}
