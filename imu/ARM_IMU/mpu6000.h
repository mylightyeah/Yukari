#ifndef __MPU6000_H
#define __MPU6000_H

#include <stdint.h>

#define MPU6000_SPI SPI1

#define SPI1_PORT GPIOA

#define SPI1_NSS_PIN GPIO4
#define SPI1_SCK_PIN GPIO5
#define SPI1_MISO_PIN GPIO6
#define SPI1_MOSI_PIN GPIO7

#define MPU6000_CS_PIN SPI1_NSS_PIN

#define MPU6000_INT_PORT GPIOC
#define MPU6000_INT_PIN GPIO4

/* MPU6000 registers */
#define MPUREG_XG_OFFS_TC (0x00)
#define MPUREG_YG_OFFS_TC (0x01)
#define MPUREG_ZG_OFFS_TC (0x02)
#define MPUREG_X_FINE_GAIN (0x03)
#define MPUREG_Y_FINE_GAIN (0x04)
#define MPUREG_Z_FINE_GAIN (0x05)
#define MPUREG_XA_OFFS_H (0x06)
#define MPUREG_XA_OFFS_L (0x07)
#define MPUREG_YA_OFFS_H (0x08)
#define MPUREG_YA_OFFS_L (0x09)
#define MPUREG_ZA_OFFS_H (0x0A)
#define MPUREG_ZA_OFFS_L (0x0B)
#define MPUREG_PRODUCT_ID (0x0C)
#define MPUREG_XG_OFFS_USRH (0x13)
#define MPUREG_XG_OFFS_USRL (0x14)
#define MPUREG_YG_OFFS_USRH (0x15)
#define MPUREG_YG_OFFS_USRL (0x16)
#define MPUREG_ZG_OFFS_USRH (0x17)
#define MPUREG_ZG_OFFS_USRL (0x18)
#define MPUREG_SMPLRT_DIV (0x19)
#define MPUREG_CONFIG (0x1A)
#define MPUREG_GYRO_CONFIG (0x1B)
#define MPUREG_ACCEL_CONFIG (0x1C)
#define MPUREG_INT_PIN_CFG (0x37)
#define MPUREG_INT_ENABLE (0x38)
#define MPUREG_INT_STATUS (0x3A)
#define MPUREG_ACCEL_XOUT_H (0x3B)
#define MPUREG_ACCEL_XOUT_L (0x3C)
#define MPUREG_ACCEL_YOUT_H (0x3D)
#define MPUREG_ACCEL_YOUT_L (0x3E)
#define MPUREG_ACCEL_ZOUT_H (0x3F)
#define MPUREG_ACCEL_ZOUT_L (0x40)
#define MPUREG_TEMP_OUT_H (0x41)
#define MPUREG_TEMP_OUT_L (0x42)
#define MPUREG_GYRO_XOUT_H (0x43)
#define MPUREG_GYRO_XOUT_L (0x44)
#define MPUREG_GYRO_YOUT_H (0x45)
#define MPUREG_GYRO_YOUT_L (0x46)
#define MPUREG_GYRO_ZOUT_H (0x47)
#define MPUREG_GYRO_ZOUT_L (0x48)
#define MPUREG_SIGNAL_PATH_RESET (0x68)
#define MPUREG_USER_CTRL (0x6A)
#define MPUREG_PWR_MGMT_1 (0x6B)
#define MPUREG_PWR_MGMT_2 (0x6C)
#define MPUREG_BANK_SEL (0x6D)
#define MPUREG_MEM_START_ADDR (0x6E)
#define MPUREG_MEM_R_W (0x6F)
#define MPUREG_DMP_CFG_1 (0x70)
#define MPUREG_DMP_CFG_2 (0x71)
#define MPUREG_FIFO_COUNTH (0x72)
#define MPUREG_FIFO_COUNTL (0x73)
#define MPUREG_FIFO_R_W (0x74)
#define MPUREG_WHOAMI (0x75)

/* Configuration bits MPU6000 */
#define BIT_SLEEP (0x40)
#define BIT_H_RESET (0x80)
#define BITS_CLKSEL (0x07)
#define MPU_CLK_SEL_PLLGYROX (0x01)
#define MPU_CLK_SEL_PLLGYROZ (0x03)
#define MPU_EXT_SYNC_GYROX (0x02)
#define BITS_FS_250DPS (0x00)
#define BITS_FS_500DPS (0x08)
#define BITS_FS_1000DPS (0x10)
#define BITS_FS_2000DPS (0x18)
#define BITS_FS_2G (0x00)
#define BITS_FS_4G (0x08)
#define BITS_FS_8G (0x10)
#define BITS_FS_16G (0x18)
#define BITS_FS_MASK (0x18)
#define BITS_DLPF_CFG_256HZ_NOLPF2 (0x00)
#define BITS_DLPF_CFG_188HZ (0x01)
#define BITS_DLPF_CFG_98HZ (0x02)
#define BITS_DLPF_CFG_42HZ (0x03)
#define BITS_DLPF_CFG_20HZ (0x04)
#define BITS_DLPF_CFG_10HZ (0x05)
#define BITS_DLPF_CFG_5HZ (0x06)
#define BITS_DLPF_CFG_2100HZ_NOLPF (0x07)
#define BITS_DLPF_CFG_MASK (0x07)
#define BIT_INT_ANYRD_2CLEAR (0x10)
#define BIT_INT_LATCH_EN (0x20)
#define BIT_INT_TRIG_DATA_RDY (0x01)
#define BIT_RAW_RDY_EN (0x01)
#define BIT_I2C_IF_DIS (0x10)

/* DMP output rate constants (default=0) */
#define MPU6000_200HZ (0)
#define MPU6000_100HZ (1)
#define MPU6000_66HZ (2)
#define MPU6000_50HZ (3)

#define MPU6000_ACC_SAMPLES 50

extern volatile uint64_t mpu6000_samples;
extern volatile uint64_t mpu6000_samples_acc;
extern volatile int16_t mpu6000_acc_calib[3];
extern volatile int16_t mpu6000_gyr_calib[3];
extern volatile float mpu6000_axis[6];
extern volatile float mpu6000_gravity[3];
extern volatile float mpu6000_linear_accel[3];
extern volatile float mpu6000_world_accel[3];
extern volatile uint16_t mpu6000_world_accel_cons_zeros[3];
extern volatile float mpu6000_world_velocity[3];
extern volatile float mpu6000_world_displacement[3];
extern uint64_t mpu6000_last_integration_time;

void mpu6000_init(void);

void mpu6000_calibrate(void);

void mpu6000_get_motion_6(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy,
                          int16_t *gz);

void mpu6000_sample(void);

void mpu6000_position_update(void);

void mpu6000_reset_integrators(void);

#endif /* __MPU6000_H */
