/*
    SD card read/write

    This example shows how to read and write data to and from an SD card file
    IT is a modified version fromt he exampled by:

    created   Nov 2010
    by David A. Mellis
    modified 9 Apr 2012
    by Tom Igoe

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
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    while (!Serial); //Wait for user to open terminal

    SERIAL.println("not SFUD_USING_QSPI");
    while (!DEV.begin(SDCARD_SS_PIN,SDCARD_SPI,4000000UL)) {
        SERIAL.println("Card Mount Failed");
        return;
    }
    SERIAL.println("initialization done.");

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.

    File RootWrite = DEV.open("test.txt", "a"); // a,r,w : append, read, write

    // if the file opened okay, write to it:
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
    
    // re-open the file for reading:
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

void loop() {
    // nothing happens after setup
}
