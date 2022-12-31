/**
 * Perilib Peripheral Device Library Example Code
 * Copyright (c) 2020 Jeff Rowberg
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <Perilib.h>
#include "madgwick_6dof.h"

#define MPU6050_DEVADDR_DEFAULT         0x68
#define MPU6050_REGADDR_ACCEL_XOUT_H    0x3B
#define MPU6050_REGADDR_PWR_MGMT_1      0x6B
#define MPU6060_REGADDR_WHO_AM_I        0x75

// create dedicated interface object for MPU-6050 peripheral using Wire object
PerilibTwiRegisterInterface_ArduinoWire mpu6050(MPU6050_DEVADDR_DEFAULT, &Wire);

// handy union/struct for working with 7-register I2C read blobs from MPU-6050
// ----------------------------------------------------------------------------
// NOTE: This is in reverse order because the actual register map structure has
// MSB first, but RegisterInterface conveniently allows us to store incoming
// bytes in reverse order in the destination buffer. Since Arduino platforms are
// almost universally (?) little-endian, storing big-endian data backwards into
// a reverse-mapped structure results in valid data with no extra byte-swapping
// required.
typedef union {
  uint8_t buf[14];
  struct {
    int16_t gz;
    int16_t gy;
    int16_t gx;
    int16_t temp;
    int16_t az;
    int16_t ay;
    int16_t ax;
  } __attribute__((packed));
} imu_data_t;

// generic reusable container for incoming data
imu_data_t imuData;

// timestamp for providing accurate deltaT values to fusion algorithm
uint32_t dTMicros;
uint32_t lastMicros;

void setup() {
  // initialize host serial interface for monitoring
  Serial.begin(9600);

  // wait for enumeration with USB-based serial interface (optional)
  //while (!Serial);

  // initialize Wire interface
  Wire.begin();

  // confirm presence of MPU-6050
  mpu6050.read8_reg8(MPU6060_REGADDR_WHO_AM_I, imuData.buf);
  if (imuData.buf[0] != 0x68) {
    Serial.println("Did not receive expected 0x68 response from MPU-6050. Verify connections:");
    Serial.println("- VCC is 3.3v");
    Serial.println("- GND is common between MCU and IMU");
    Serial.println("- SDA/SCL are not swapped");
    Serial.println("- SDA/SCL have independent ~4.7k pull-ups to VCC");
    Serial.println("- AD0 slave address pin is tied or pulled to GND for default 0x68 slave address");
    while (1);
  }

  // prime timestamp to first reading
  lastMicros = micros();

  // disable sleep on MPU
  mpu6050.write8_reg8(MPU6050_REGADDR_PWR_MGMT_1, 0x00);
}

void loop() {
  // read raw IMU data (datalength is "-14" to store 14 incoming bytes in reverse order)
  int rxResult = mpu6050.readBuf_reg8(MPU6050_REGADDR_ACCEL_XOUT_H, imuData.buf, -14);
  if (rxResult != 14) {
    Serial.println("Error reading data from IMU");
  } else {
#if 0
    // send raw data to host
    Serial.print("ax,ay,az,gx,gy,gz=");
    Serial.print(imuData.ax); Serial.write(',');
    Serial.print(imuData.ay); Serial.write(',');
    Serial.print(imuData.az); Serial.write(',');
    Serial.print(imuData.gx); Serial.write(',');
    Serial.print(imuData.gy); Serial.write(',');
    Serial.println(imuData.gz);
#endif

    // run Madgwick IMU fusion algorithm iteration
    dTMicros = micros() - lastMicros;
    lastMicros += dTMicros;
    Quaternion q = update_madgwick_6dof_filter(
      imuData.ax, imuData.ay, imuData.az, // raw accelerometer values
      imuData.gx, imuData.gy, imuData.gz, // raw gyroscope values
      dTMicros/1000000.0f,                // time (sec) since last iteration
      (1.0f/16384.0f),                    // scale factor to convert raw accel -> g (1.0 = 9.8 m/s^2)
      PI/(131.0f*180.0f));                // scale factor to convert raw gyro -> rad/sec

    // send quaternion data to host
    Serial.print("qw,qx,qy,qz=");
    Serial.print(q.w, 6); Serial.write(',');
    Serial.print(q.x, 6); Serial.write(',');
    Serial.print(q.y, 6); Serial.write(',');
    Serial.println(q.z, 6);
  }

  // wait 10ms between iterations for ~100Hz output
  delay(10);
}
