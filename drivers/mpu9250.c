#include "mpu9250.h"
extern I2C_HandleTypeDef hi2c;
// cm/s
// cm/s2

float accelBias[3], gyroBias[3];

unsigned int fifoCount = 0;

uint8_t addr = 0;
uint8_t fifoCountBytes[2];
uint16_t totalCount = 0, packetCount = 0;
uint8_t readIndex = 0;

MPU_STATE mpuState = IDLE;

bool MPU_ready = false;

uint8_t mpuBytes[14];

void I2CRead(unsigned int addr, uint8_t subAddr, uint8_t* buffer, unsigned int rlen) {
	HAL_I2C_Mem_Read_IT(&hi2c, addr<<1, subAddr, 1, buffer, rlen);
}

void I2CWrite(unsigned int addr, uint8_t subAddr, uint8_t* buffer, unsigned int wlen) {
	HAL_I2C_Mem_Write_IT(&hi2c, addr<<1, subAddr, 1, buffer, wlen);
}
inline void startFIFOread(void) {
	I2CRead(MPU_ADDRESS, FIFO_COUNTH, fifoCountBytes, 2);
}

bool MPUwriteReg(uint8_t reg, uint8_t value, bool block) {
	return HAL_I2C_Mem_Write_IT(&hi2c, MPU_ADDRESS<<1, reg, 1, &value, 1);
}

inline void MPU_startSampling(void) {
	MPUwriteReg(FIFO_EN, 0x78, false);
}

bool MPU_stopSampling(void) {
	return MPUwriteReg(FIFO_EN, 0x00, false) == HAL_OK;
}


void readAccelData(int16_t * destination)
{
  uint8_t rawData[6];  // x/y/z accel register data stored here
  I2CRead(MPU_ADDRESS, ACCEL_XOUT_H, &rawData[0], 6);  // Read the six raw data registers into data array
  destination[0] = ((int16_t)rawData[0] << 8) | rawData[1] ;  // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = ((int16_t)rawData[2] << 8) | rawData[3] ;  
  destination[2] = ((int16_t)rawData[4] << 8) | rawData[5] ; 
}

void printOffsets(void) {
	printf("ax: %f ay: %f az: %f gx: %f gy: %f gz: %f\r\n", accelBias[0], accelBias[1], accelBias[2], gyroBias[0], gyroBias[1], gyroBias[2]);
}

// Function which accumulates gyro and accelerometer data after device
// initialization. It calculates the average of the at-rest readings and then
// loads the resulting offsets into accelerometer and gyro bias registers.
bool calibrateMPU9250(float * gyroBias, float * accelBias) {
	uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
	uint16_t  accelsensitivity = 16384; // = 16384 LSB/g 
	uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
	uint16_t ii, packet_count, fifo_count;
	int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};
	int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};

	// reset device
	// Write a one to bit 7 reset bit; toggle reset device
	if (MPUwriteReg(PWR_MGMT_1, 0x80, true)) return false;
	HAL_Delay(100);

	// get stable time source; Auto select clock source to be PLL gyroscope
	// reference if ready else use the internal oscillator, bits 2:0 = 001
	MPUwriteReg(PWR_MGMT_1, 0x01, true);
	MPUwriteReg(PWR_MGMT_2, 0x00, true);
	HAL_Delay(200);

	// Configure device for bias calculation
	// Disable all interrupts
	MPUwriteReg(INT_ENABLE, 0x00, true);
	// Disable FIFO
	MPUwriteReg(FIFO_EN, 0x00, true);
	// Turn on internal clock source
	MPUwriteReg(PWR_MGMT_1, 0x00, true);
	// Disable I2C master
	MPUwriteReg(I2C_MST_CTRL, 0x00, true);
	// Disable FIFO and I2C master modes
	MPUwriteReg(USER_CTRL, 0x00, true);
	// Reset FIFO and DMP
	MPUwriteReg(USER_CTRL, 0x0C, true);
	HAL_Delay(15);

	// Configure MPU6050 gyro and accelerometer for bias calculation
	// Set low-pass filter to 188 Hz
	MPUwriteReg(CONFIG, 0x01, true);
	// Set sample rate to 1 kHz
	MPUwriteReg(SMPLRT_DIV, 0x00, true);
	// Set gyro full-scale to 250 degrees per second, maximum sensitivity
	MPUwriteReg(GYRO_CONFIG, 0x00, true);
	// Set accelerometer full-scale to 2 g, maximum sensitivity
	MPUwriteReg(ACCEL_CONFIG, 0x18, true);

	// Configure FIFO to capture accelerometer and gyro data for bias calculation
	MPUwriteReg(USER_CTRL, 0x40, true);  // Enable FIFO
	// Enable gyro and accelerometer sensors for FIFO  (max size 512 bytes in
	// MPU-9150)
	MPUwriteReg(FIFO_EN, 0x78, true);
	HAL_Delay(40);  // accumulate 40 samples in 40 milliseconds = 480 bytes

	// At end of sample accumulation, turn off FIFO sensor read
	// Disable gyro and accelerometer sensors for FIFO
	MPUwriteReg(FIFO_EN, 0x00, true);
	// Read FIFO sample count
	I2CRead(MPU_ADDRESS, FIFO_COUNTH, data, 2);
	fifo_count = ((uint16_t)data[0] << 8) | data[1];
	// How many sets of full gyro and accelerometer data for averaging
	packet_count = fifo_count/12;

	for (ii = 0; ii < packet_count; ii++) {

		// Read data for averaging
		I2CRead(MPU_ADDRESS, FIFO_R_W, data, 12);
		// Form signed 16-bit integer for each sample in FIFO
		accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  );
		accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  );
		accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  );
		gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  );
		gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  );
		gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]);

		// Sum individual signed 16-bit biases to get accumulated signed 32-bit
		// biases.
		accel_bias[0] += (int32_t) accel_temp[0];
		accel_bias[1] += (int32_t) accel_temp[1];
		accel_bias[2] += (int32_t) accel_temp[2];
		gyro_bias[0]  += (int32_t) gyro_temp[0];
		gyro_bias[1]  += (int32_t) gyro_temp[1];
		gyro_bias[2]  += (int32_t) gyro_temp[2];
	}
	// Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
	accel_bias[0] /= (int32_t) packet_count;
	accel_bias[1] /= (int32_t) packet_count;
	accel_bias[2] /= (int32_t) packet_count;
	gyro_bias[0]  /= (int32_t) packet_count;
	gyro_bias[1]  /= (int32_t) packet_count;
	gyro_bias[2]  /= (int32_t) packet_count;

	// Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
	if (accel_bias[2] > 0L) accel_bias[2] -= (int32_t) accelsensitivity;
	else accel_bias[2] += (int32_t) accelsensitivity;

	// Construct the gyro biases for push to the hardware gyro bias registers,
	// which are reset to zero upon device startup.
	// Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input
	// format.
	data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF;
	// Biases are additive, so change sign on calculated average gyro biases
	data[1] = (-gyro_bias[0]/4)       & 0xFF;
	data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
	data[3] = (-gyro_bias[1]/4)       & 0xFF;
	data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
	data[5] = (-gyro_bias[2]/4)       & 0xFF;

	// Push gyro biases to hardware registers
	MPUwriteReg(XG_OFFSET_H, data[0], true);
	MPUwriteReg(XG_OFFSET_L, data[1], true);
	MPUwriteReg(YG_OFFSET_H, data[2], true);
	MPUwriteReg(YG_OFFSET_L, data[3], true);
	MPUwriteReg(ZG_OFFSET_H, data[4], true);
	MPUwriteReg(ZG_OFFSET_L, data[5], true);

	// Output scaled gyro biases for display in the main program
	gyroBias[0] = (float) gyro_bias[0]/(float) gyrosensitivity;
	gyroBias[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
	gyroBias[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

	/* Construct the accelerometer biases for push to the hardware accelerometer
	   bias registers. These registers contain factory trim values which must be
	   added to the calculated accelerometer biases; on boot up these registers
	   will hold non-zero values. In addition, bit 0 of the lower byte must be
	   preserved since it is used for temperature compensation calculations.
	   Accelerometer bias registers expect bias input as 2048 LSB per g, so that
	   the accelerometer biases calculated above must be divided by 8.*/

	// A place to hold the factory accelerometer trim biases
	int32_t accel_bias_reg[3] = {0, 0, 0};
	// Read factory accelerometer trim values
	I2CRead(MPU_ADDRESS, XA_OFFSET_H, data, 2);
	accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
	I2CRead(MPU_ADDRESS, YA_OFFSET_H, data, 2);
	accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
	I2CRead(MPU_ADDRESS, ZA_OFFSET_H, data, 2);
	accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

	// Define mask for temperature compensation bit 0 of lower byte of
	// accelerometer bias registers
	uint32_t mask = 1uL;
	// Define array to hold mask bit for each accelerometer bias axis
	uint8_t mask_bit[3] = {0, 0, 0};

	for (ii = 0; ii < 3; ii++)
	{
		// If temperature compensation bit is set, record that fact in mask_bit
		if ((accel_bias_reg[ii] & mask))
		{
			mask_bit[ii] = 0x01;
		}
	}

	// Construct total accelerometer bias, including calculated average
	// accelerometer bias from above
	// Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g
	// (16 g full scale)
	accel_bias_reg[0] -= (accel_bias[0]/8);
	accel_bias_reg[1] -= (accel_bias[1]/8);
	accel_bias_reg[2] -= (accel_bias[2]/8);

	data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
	data[1] = (accel_bias_reg[0])      & 0xFF;
	// preserve temperature compensation bit when writing back to accelerometer
	// bias registers
	data[1] = data[1] | mask_bit[0];
	data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
	data[3] = (accel_bias_reg[1])      & 0xFF;
	// Preserve temperature compensation bit when writing back to accelerometer
	// bias registers
	data[3] = data[3] | mask_bit[1];
	data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
	data[5] = (accel_bias_reg[2])      & 0xFF;
	// Preserve temperature compensation bit when writing back to accelerometer
	// bias registers
	data[5] = data[5] | mask_bit[2];

	// Apparently this is not working for the acceleration biases in the MPU-9250
	// Are we handling the temperature correction bit properly?
	// Push accelerometer biases to hardware registers
	MPUwriteReg(XA_OFFSET_H, data[0], true);
	MPUwriteReg(XA_OFFSET_L, data[1], true);
	MPUwriteReg(YA_OFFSET_H, data[2], true);
	MPUwriteReg(YA_OFFSET_L, data[3], true);
	MPUwriteReg(ZA_OFFSET_H, data[4], true);
	MPUwriteReg(ZA_OFFSET_L, data[5], true);

	// Output scaled accelerometer biases for display in the main program
	accelBias[0] = (float)accel_bias[0]/(float)accelsensitivity;
	accelBias[1] = (float)accel_bias[1]/(float)accelsensitivity;
	accelBias[2] = (float)accel_bias[2]/(float)accelsensitivity;

	return true;
}

bool MPUinitialize(void) {
	return calibrateMPU9250(gyroBias, accelBias);
}

void MPUread(void) {
	I2CRead(MPU_ADDRESS, ACCEL_XOUT_H, mpuBytes, 14);
}
