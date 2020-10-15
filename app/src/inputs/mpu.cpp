#include <protothreads.h>
#include <Arduino.h>
#include <Wire.h>
#include "lib/MPU6050.h"

const int mpuAddress = 0x68;  // I2C address of the MPU-6050
MPU6050 mpu(mpuAddress);

// Factores de conversion
const float accScale = 2.0 * 9.81 / 32768.0;
const float gyroScale = 250.0 / 32768.0;

void printTab(){
	Serial.print(F("\t"));
}

int printAllMPU() {
  int ax, ay, az;
  int gx, gy, gz;
	mpu.getRotation(&gx, &gy, &gz);
  mpu.getAcceleration(&ax, &ay, &az);
  Serial.print(F("DEBUG a[x y z] g[x y z]:t"));
	Serial.print(ax); printTab();
	Serial.print(ay); printTab();
	Serial.print(az); printTab();
	Serial.print(gx); printTab();
	Serial.print(gy); printTab();
	Serial.println(gz);

  Serial.print(F("a[x y z](m/s2) g[x y z](deg/s):t"));
  Serial.print(ax * accScale); printTab();
  Serial.print(ay * accScale); printTab();
  Serial.print(az * accScale); printTab();
  Serial.print(gx * gyroScale);  printTab();
  Serial.print(gy * gyroScale);  printTab();
  Serial.println(gz * gyroScale);
  float accel_ang_x = atan(ax / sqrt(pow(ay, 2) + pow(az, 2)))*(180.0 / 3.14);
  float accel_ang_y = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))*(180.0 / 3.14);

  // Mostrar resultados
  Serial.print(F("Inclinacion en X: "));
  Serial.print(accel_ang_x);
  Serial.print(F("\tInclinacion en Y:"));
  Serial.println(accel_ang_y);
}


void getMPURotation(int rotation[3]) {
  int gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  rotation[0] = gx * gyroScale;
  rotation[1] = gy * gyroScale;
  rotation[2] = gz * gyroScale;
}

void getAcceleration(int acceleration[3]) {
  int ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  acceleration[0] = ax * accScale;
  acceleration[1] = ay * accScale;
  acceleration[2] = az * accScale;
}

void getMPUSlope(int slope[2]) {
  int ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  slope[0] = atan(ax / sqrt(pow(ay, 2) + pow(az, 2)))*(180.0 / 3.14);
  slope[1] = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))*(180.0 / 3.14);
}

void setup_mpu() {
  Wire.begin();
	mpu.initialize();
}

void loop_mpu() {

}
