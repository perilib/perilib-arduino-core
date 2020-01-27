#include <Perilib.h>

#define MPU6050_DEVADDR_DEFAULT         0x68
#define MPU6050_REGADDR_ACCEL_XOUT_H    0x3B
#define MPU6050_REGADDR_PWR_MGMT_1      0x6B
#define MPU6060_REGADDR_WHO_AM_I        0x75

// create dedicated interface object for MPU-6050 peripheral using Wire object
Perilib::TwiRegisterInterface_ArduinoWire mpu6050(MPU6050_DEVADDR_DEFAULT, &Wire);

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

void setup() {
  // initialize host serial interface for monitoring
  Serial.begin(9600);
  
  // wait for enumeration for USB-based serial interface (optional)
  while (!Serial);
  
  // initialize Wire interface
  Wire.begin();
  
  // confirm presense of MPU-6050
  mpu6050.read8_reg8(MPU6060_REGADDR_WHO_AM_I, imuData.buf);
  if (imuData.buf[0] != 0x68) {
    Serial.println("Did not receive expected 0x68 response from MPU-6050. Verify connections:");
    Serial.println("- VCC/VDD is 3.3v");
    Serial.println("- GND is common between MCU and IMU");
    Serial.println("- SDA/SCL are not swapped");
    Serial.println("- SDA/SCL have independent ~4.7k pull-ups to VCC");
    Serial.println("- AD0 slave address pin is tied or pulled to GND for default 0x68 slave address");
    while (1);
  }
  
  // disable sleep on MPU
  mpu6050.write8_reg8(MPU6050_REGADDR_PWR_MGMT_1, 0x00);
}

void loop() {
  // read raw IMU data (using "-14" to denote reverse incoming storage order)
  int rxResult = mpu6050.readBuf_reg8(MPU6050_REGADDR_ACCEL_XOUT_H, imuData.buf, -14);
  if (rxResult != 14) {
    Serial.println("Error reading data from IMU");
  } else {
    Serial.print("ax, ay, az, gx, gy, gz = ");
    Serial.print(imuData.ax); Serial.write(", ");
    Serial.print(imuData.ay); Serial.write(", ");
    Serial.print(imuData.az); Serial.write(", ");
    Serial.print(imuData.gx); Serial.write(", ");
    Serial.print(imuData.gy); Serial.write(", ");
    Serial.println(imuData.gz);
  }
  
  // wait 10ms between iterations for ~100Hz output
  delay(10);
}
