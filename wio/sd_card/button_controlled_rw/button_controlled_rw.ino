/*
SD card read/write
button a write
button b read
buttin c close files and stops

*/
#include <Seeed_FS.h>

#define SERIAL Serial

#define DEV SD
#include "SD/Seeed_SD.h"

#ifdef _SAMD21_
#define SDCARD_SS_PIN 1
#define SDCARD_SPI SPI
#endif 

void setup() {
  SERIAL.begin(115200);
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
  while (!Serial); //Wait for user to open terminal
  
  SERIAL.println("not SFUD_USING_QSPI");
  while (!DEV.begin(SDCARD_SS_PIN,SDCARD_SPI,4000000UL)) {
    SERIAL.println("Card Mount Failed");
    return;
  }
  SERIAL.println("initialization done.");
}

void loop() {
  if (digitalRead(WIO_KEY_A) == LOW) {
    // open file for writing
    File RootWrite = DEV.open("test.txt", "a"); // a,r,w : append, read, write 
    if (RootWrite) {
      SERIAL.print("Writing to test.txt...");
      RootWrite.println("hello SD world.");
      // close the file:
      RootWrite.close();
      SERIAL.println("done.");
    } else {
      // if the file didn't open, print an error:
      SERIAL.println("error opening test.txt");
    }
  }
    else if (digitalRead(WIO_KEY_B) == LOW) {
    // open the file for reading:
    File RootRead= DEV.open("test.txt");
    if (RootRead) {
      SERIAL.println("test.txt:");
      
      // read from the file until there's nothing else in it:
      while (RootRead.available()) {
        SERIAL.write(RootRead.read());
      }
      // close the file:
      RootRead.close();
    } else {
    // if the file didn't open, print an error:
      SERIAL.println("error opening test.txt");
    }
  }
  else if (digitalRead(WIO_KEY_C) == LOW) {
    Serial.println("Device halted");
    while(1);
  }
  
  delay(200);
}
