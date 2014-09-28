#include <MAX6675.h>

int CSPin = 11;         // CS pin on MAX6675
int SPin = 12;          // SO pin of MAX6675
int SCKPin = 13;         // SCK pin of MAX6675
int units = 0;        // Units to readout temp (0 = ÀöF, 1 = ÀöC)
float error = 0.0;    // Temperature compensation error
float temp_out = 0.0; // Temperature output varible

MAX6675 temp0(CSPin,SPin,SCKPin,units,error);

void setup() {
  Serial.begin(9600);
}

void loop() {
  temp_out = temp0.read_temp(5);  // Read the temp 5 times and return the average value to the var

  Serial.println( temp_out );         // Print the temperature to Serial 
  delay(1000);                    // Wait one second
}

