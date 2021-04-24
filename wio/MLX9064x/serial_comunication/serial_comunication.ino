/*
    Cleaned code from the WiKi example at
    https://wiki.seeedstudio.com/Grove-Thermal-Imaging-Camera-IR-Array/
*/
 
#include <Wire.h>
#include "MLX9064X_I2C_Driver.h"

// define USE_MLX90640 or USE_MLX90641 according to what groove is attached
#define USE_MLX90640

#ifdef USE_MLX90641
    #include "MLX90641_API.h"
    #define TA_SHIFT 8 //Default shift for MLX90641 in open air
    
    const byte MLX90641_address = 0x33; //Default 7-bit unshifted address of the MLX90641
    uint16_t eeMLX90641[832];
    float MLX90641To[192];
    uint16_t MLX90641Frame[242];
    paramsMLX90641 MLX90641;
    int errorno = 0;
#else
    #include "MLX90640_API.h"
    #define TA_SHIFT 8 //Default shift for MLX90640 in open air
    
    const byte MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640
    float mlx90640To[768];
    paramsMLX90640 mlx90640;
#endif
 

void setup() {
    Wire.begin();
    Wire.setClock(400000); //Increase I2C clock speed to 400kHz
    Serial.begin(115200); //Fast Serial as possible
    while (!Serial); //Wait for user to open terminal
    Serial.println("WARNING: First two samples might be garbage\n\n");
 
#ifndef USE_MLX90641
    if (isConnected() == false) {
        Serial.println("MLX9064x not detected at default I2C address. Please check wiring. Freezing.");
        while (1);
    }
    //Get device parameters - We only have to do this once
    int status;
    uint16_t eeMLX90640[832];
    status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
    if (status != 0) {
        Serial.println("Failed to load system parameters");
    }
 
    status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
    if (status != 0) {
        Serial.println("Parameter extraction failed");
    }
 
    //Once params are extracted, we can release eeMLX90640 array
    MLX90640_SetRefreshRate(MLX90640_address, 0x05); //Set rate to 16Hz
#else
    if (isConnected() == false) {
        Serial.println("MLX90641 not detected at default I2C address. Please check wiring. Freezing.");
        while (1);
    }
    //Get device parameters - We only have to do this once
    int status;
    status = MLX90641_DumpEE(MLX90641_address, eeMLX90641);
    errorno = status;//MLX90641_CheckEEPROMValid(eeMLX90641);//eeMLX90641[10] & 0x0040;//
 
    if (status != 0) {
        Serial.println("Failed to load system parameters");
       while(1);
    }
 
    status = MLX90641_ExtractParameters(eeMLX90641, &MLX90641);
    //errorno = status;
    if (status != 0) {
        Serial.println("Parameter extraction failed");
        while(1);
    }
 
    //Once params are extracted, we can release eeMLX90641 array
    MLX90641_SetRefreshRate(MLX90641_address, 0x03); //Set rate to 16Hz
#endif 
 
}
 
void loop() {
    unsigned long timeBegin = micros();
#ifndef USE_MLX90641
    for (byte x = 0 ; x < 2 ; x++) {
        uint16_t mlx90640Frame[834];
        int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);
 
        float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
        float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);
 
        float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
        float emissivity = 0.95;
        MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);
    }
    unsigned long timeEnd = micros();
    unsigned long timeDifference = timeEnd - timeBegin;
    double duration = (double)timeDifference / 1000.0;

    char sample[2+768*7] = "[";
    for (int x = 0 ; x < 768 ; x++) {
        char buff[6];
        dtostrf(mlx90640To[x], 6, 2, buff); 
        strcat(sample, buff);
        if (x<767) {
          sample[7 + 6*x] = ',';}
         else {
          sample[7 + 6*x] = ']';
         }
    }
    Serial.print(duration,2);
    Serial.print(", ");
    Serial.println(sample);
#else 
    for (byte x = 0 ; x < 2 ; x++) {
        int status = MLX90641_GetFrameData(MLX90641_address, MLX90641Frame);
 
        float vdd = MLX90641_GetVdd(MLX90641Frame, &MLX90641);
        float Ta = MLX90641_GetTa(MLX90641Frame, &MLX90641);
 
        float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
        float emissivity = 0.95;
 
        MLX90641_CalculateTo(MLX90641Frame, &MLX90641, emissivity, tr, MLX90641To);
    }
    unsigned long stopTime = micros();
    unsigned long timeDifference = stopTime - timeBegin;
    double duration = (double)timeDifference / 1000.0;

    char sample[2+192*7] = "[";
    for (int x = 0 ; x < 192 ; x++) {
        char buff[6];
        dtostrf(MLX90641To[x], 6, 2, buff); 
        strcat(sample, buff);
        if (x<191) {
          sample[7 + 6*x] = ',';}
         else {
          sample[7 + 6*x] = ']';
         }
    }
    Serial.print(duration,2);
    Serial.print(", ");
    Serial.println(sample);
#endif
}
 
//Returns true if the MLX90640 is detected on the I2C bus
boolean isConnected() {
#ifndef USE_MLX90641
    Wire.beginTransmission((uint8_t)MLX90640_address);
#else
    Wire.beginTransmission((uint8_t)MLX90641_address);
#endif
    if (Wire.endTransmission() != 0) {
        return (false);    //Sensor did not ACK
    }
    return (true);
}

  
