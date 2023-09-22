//#define MPU6500_ADDR 				0x68
//#define MPU6500_ADDR_R				((MPU6500_ADDR << 1U) | 1U)
//#define MPU6500_ADDR_W				(MPU6500_ADDR << 1U)
//#define MPU6500_WHO_AM_I_ID			0x70

#define MPU6500_ADDR 				0x69
#define MPU6500_ADDR_R				((MPU6500_ADDR << 1U) | 1U)
#define MPU6500_ADDR_W				(MPU6500_ADDR << 1U)
#define MPU6500_WHO_AM_I_ID			0x11

#define MPU6500_SELF_TEST_X_GYRO	0x00

#define MPU6500_SELF_TEST_Y_GYRO	0x01

#define MPU6500_SELF_TEST_Z_GYRO	0x02

#define MPU6500_SELF_TEST_X_ACCEL	0x0D

#define MPU6500_SELF_TEST_Y_ACCEL	0x0E

#define MPU6500_SELF_TEST_Z_ACCEL	0x0F

#define MPU6500_XG_OFFSET_H			0x13

#define MPU6500_XG_OFFSET_L			0x14

#define MPU6500_YG_OFFSET_H			0x15

#define MPU6500_YG_OFFSET_L			0x16

#define MPU6500_ZG_OFFSET_H			0x17

#define MPU6500_ZG_OFFSET_L			0x18

#define MPU6500_SMPLRT_DIV			0x19

#define MPU6500_CONFIG 				0x1A
#define MPU6500_FIFO_MODE			(1U << 6)
#define MPU6500_G_DLPF_250HZ		0x00
#define MPU6500_G_DLPF_184HZ		0x01
#define MPU6500_G_DLPF_92HZ			0x02
#define MPU6500_G_DLPF_41HZ			0x03
#define MPU6500_G_DLPF_20HZ			0x04
#define MPU6500_G_DLPF_10HZ			0x05
#define MPU6500_G_DLPF_5HZ			0x06
#define MPU6500_G_DLPF_3600HZ		0x07

#define MPU6500_GYRO_CONFIG			0x1B
#define MPU6500_XG_ST 				(1U << 7)
#define MPU6500_YG_ST 				(1U << 6)
#define MPU6500_ZG_ST 				(1U << 5)
#define MPU6500_GYRO_FS_250DPS		0x00
#define MPU6500_GYRO_FS_500DPS		(0x01 << 3)
#define MPU6500_GYRO_FS_1000DPS		(0x02 << 3)
#define MPU6500_GYRO_FS_2000DPS		(0x03 << 3)
#define MPU6500_GC_RESERVED_MASK	0x0C
#define MPU6500_G_FCHOICE_8800HZ	0x03
#define MPU6500_G_FCHOICE_3600HZ	0x02

#define MPU6500_ACCEL_CONFIG 		0x1C
#define MPU6500_XA_ST 				(1 << 7)
#define MPU6500_YA_ST 				(1 << 6)
#define MPU6500_ZA_ST 				(1 << 5)
#define MPU6500_ACCEL_FS_2G			0x0
#define MPU6500_ACCEL_FS_4G			(0x01 << 3)
#define MPU6500_ACCEL_FS_8G			(0x02 << 3)
#define MPU6500_ACCEL_16G			(0x03 << 3)
#define MPU6500_AC_RESERVED_MASK	0x07

#define MPU6500_ACCEL_CONFIG2		0x1D
#define MPU6500_A_FCHOICE_B			(1U << 3)
#define MPU6500_A_DPLF_460HZ		0x00
#define MPU6500_A_DPLF_184HZ		0x01
#define MPU6500_A_DPLF_92HZ			0x02
#define MPU6500_A_DPLF_41HZ 		0x03
#define MPU6500_A_DPLF_20HZ			0x04
#define MPU6500_A_DPLF_10HZ			0x05
#define MPU6500_A_DPLF_5HZ			0x06

#define MPU6500_LP_ACCEL_ODR		0x1E
#define MPU6500_LP_ACCEL_RMAP		0XF0

#define MPU6500_WOM_THR				0x1F
#define MPU6500_FIFO_TEMP_OUT		(1 << 7)
#define MPU6500_FIFO_GYRO_XOUT		(1 << 6)
#define MPU6500_FIFO_GYRO_YOUT		(1 << 5)
#define MPU6500_FIFO_GYRO_ZOUT		(1 << 4)
#define MPU6500_FIFO_ACCEL			(1 << 3)
#define MPU6500_FIFO_SLV_2			(1 << 2)
#define MPU6500_FIFO_SLV_1			(1 << 1)
#define MPU6500_FIFO_SLV_0			1


#define MPU6500_I2C_MST_CTRL 		0x24
#define MPU6500_I2C_MULT_MST_EN		(1 << 7)
#define MPU6500_I2C_WAIT_FOR_ES		(1 << 6)
#define MPU6500_I2C_MST_P_NSR		(1 << 5)

#define MPU6500_I2C_SLV0_ADDR		0x25

#define MPU6500_I2C_SLV0_REG		0x26

#define MPU6500_I2C_SLV0_CTRL		0x27
#define MPU6500_I2C_SLV0_EN			(1 << 7)
#define MPU6500_I2C_SLV0_BYTE_SW    (1 << 6)
#define MPU6500_I2C_SLV0_REG_DIS	(1 << 5)
#define MPU6500_I2C_SLV0_GRP		(1 << 4)

#define MPU6500_I2C_SLV1_ADDR 		0x28

#define MPU6500_I2C_SLV1_REG 		0x29

#define MPU6500_I2C_SLV1_CTRL 		0x2A
#define MPU6500_I2C_SLV1_EN			(1 << 7)
#define MPU6500_I2C_SLV1_BYTE_SW    (1 << 6)
#define MPU6500_I2C_SLV1_REG_DIS	(1 << 5)
#define MPU6500_I2C_SLV1_GRP		(1 << 4)

#define MPU6500_I2C_SLV2_ADDR 		0x2B

#define MPU6500_I2C_SLV2_REG 		0x2C

#define MPU6500_I2C_SLV2_CTRL		0x2D
#define MPU6500_I2C_SLV2_EN			(1 << 7)
#define MPU6500_I2C_SLV2_BYTE_SW    (1 << 6)
#define MPU6500_I2C_SLV2_REG_DIS	(1 << 5)
#define MPU6500_I2C_SLV2_GRP		(1 << 4)

#define MPU6500_I2C_SLV3_ADDR 		0x2E

#define MPU6500_I2C_SLV3_REG 		0x2F

#define MPU6500_I2C_SLV3_CTRL 		0x30
#define MPU6500_I2C_SLV2_EN			(1 << 7)
#define MPU6500_I2C_SLV2_BYTE_SW    (1 << 6)
#define MPU6500_I2C_SLV2_REG_DIS	(1 << 5)
#define MPU6500_I2C_SLV2_GRP		(1 << 4)

#define MPU6500_I2C_SLV4_ADDR 		0x31

#define MPU6500_I2C_SLV4_REG		0x32

#define MPU6500_I2C_SLV4_DO			0x33

#define MPU6500_I2C_SLV4_CTRL		0x34
#define MPU6500_I2C_SLV4_EN			(1 << 7)
#define MPU6500_SLV4_DONE_INT_EN	(1 << 6)
#define MPU6500_I2C_SLV4_REG_DIS	(1 << 5)

#define MPU6500_I2C_SLV4_DI			0x35

#define MPU6500_I2C_MST_STATUS		0x36
#define MPU6500_I2C_PASS_THROUGH	(1 << 7)
#define MPU6500_I2C_SLV4_DONE		(1 << 6)
#define MPU6500_I2C_SLV4_NACK		(1 << 4)
#define MPU6500_I2C_SLV3_NACK		(1 << 3)
#define MPU6500_I2C_SLV2_NACK		(1 << 2)
#define MPU6500_I2C_SLV1_NACK		(1 << 1)
#define MPU6500_I2C_SLV0_NACK		(1 << 0)

#define MPU6500_INT_PIN_CFG			0x37
#define MPU6500_INT_ACTL			(1 << 7)
#define MPU6500_INT_OPEN			(1 << 6)
#define MPU6500_LATCH_INT_EN		(1 << 5)
#define MPU6500_INT_ANYRD_2CLEAR	(1 << 4)
#define MPU6500_INT_ACTL_FSYNC		(1 << 3)
#define MPU6500_FSYNC_INT_MODE_EN	(1 << 2)
#define MPU6500_INT_BYPASS_EN		(1 << 1)
#define MPU6500_INT_RESERVED		0X01

#define MPU6500_INT_ENABLE			0x38
#define MPU6500_INT_EN_RESERVED		(1 << 7)
#define MPU6500_INT_WOM_EN			(1 << 6)
#define MPU6500_FIFO_OVERFLOW_EN	(1 << 4)
#define MPU6500_FSYNC_INT_EN 		(1 << 3)
#define MPU6500_INT_RAW_RDY_EN		(1 << 0)
#define MPU6500_RESERVED_MAP		(3 << 1) | (1 << 5)

#define MPU6500_INT_STATUS			0x3A
#define MPU6500_WOM_INT				(1 << 6)
#define MPU6500_FIFO_OVERFLOW_INT	(1 << 4)
#define MPU6500_FSYNC_INT 			(1 << 3)
#define MPU6500_DMP_INT				(1 << 1)
#define MPU6500_RAW_DATA_RDY_INT	0X01

#define MPU6500_ACCEL_XOUT_H		0x3B

#define MPU6500_ACCEL_XOUT_L		0x3C

#define MPU6500_ACCEL_YOUT_H 		0x3D

#define MPU6500_ACCEL_YOUT_L		0x3E

#define MPU6500_ACCEL_ZOUT_H		0x3F

#define MPU6500_ACCEL_ZOUT_L		0x40

#define MPU6500_TEMP_OUT_H			0x41

#define MPU6500_TEMP_OUT_L			0x42

#define MPU6500_GYRO_XOUT_H			0x43

#define MPU6500_GYRO_XOUT_L			0x44

#define MPU6500_GYRO_YOUT_H			0x45

#define MPU6500_GYRO_YOUT_L			0x46

#define MPU6500_GYRO_ZOUT_H			0x47

#define MPU6500_GYRO_ZOUT_L			0x48

#define MPU6500_EXT_SENS_DATA_BASE  0x49

#define MPU6500_I2C_SLV0_DO			0x63

#define MPU6500_I2C_SLV1_DO			0x64

#define MPU6500_I2C_SLV2_DO			0x65

#define MPU6500_I2C_SLV3_DO			0x66

#define MPU6500_I2C_MST_DELAY_CTRL  0x67
#define MPU6500_DELAY_ES_SHADOW		(1 << 7)
#define MPU6500_I2C_DELAY_RESERVED 	(3 << 5)
#define MPU6500_I2C_SLV4_DLY_EN		(1 << 4)
#define MPU6500_I2C_SLV3_DLY_EN		(1 << 3)
#define MPU6500_I2C_SLV2_DLY_EN		(1 << 2)
#define MPU6500_I2C_SLV1_DLY_EN		(1 << 1)
#define MPU6500_I2C_SLV0_DLY_EN		(1 << 0)

#define MPU6500_SIGNAL_PATH_RESET   0x68
#define MPU6500_SIGNAL_RST_RESERVED	0xF8
#define MPU6500_GYRO_RST			(1 << 2)
#define MPU6500_ACCEL_RST			(1 << 1)
#define MPU6500_TEMP_RST			(1 << 0)

#define MPU6500_ACCEL_INTEL_CTRL	0x69
#define MPU6500_ACCEL_INTEL_EN		(1 << 7)
#define MPU6500_ACCEL_INTEL_MODE	(1 << 6)
#define MPU6500_ACCEL_INTEL_RMAP	0x3F

#define MPU6500_USER_CTRL			0x6A
#define MPU6500_DMP_EN				(1 << 7)
#define MPU6500_FIFO_EN				(1 << 6)
#define MPU6500_I2C_MST_EN			(1 << 5)
#define MPU6500_I2C_IF_DIS			(1 << 4)
#define MPU6500_DMP_RST				(1 << 3)
#define MPU6500_FIFO_RST			(1 << 2)
#define MPU6500_I2C_MST_RST			(1 << 1)
#define MPU6500_SIG_COND_RST		0x01

#define MPU6500_PWR_MGMT_1			0x6B
#define MPU6500_DEVICE_RESET		(1 << 7)
#define MPU6500_SLEEP				(1 << 6)
#define MPU6500_CYCLE				(1 << 5)
#define MPU6500_GYRO_STANDBY		(1 << 4)
#define MPU6500_TEMP_DIS			(1 << 3)

#define MPU6500_PWR_MGMT_2			0x6C
#define MPU6500_DISABLE_XA			(1 << 5)
#define MPU6500_DISABLE_YA			(1 << 4)
#define MPU6500_DISABLE_ZA			(1 << 3)
#define MPU6500_DISABLE_XG			(1 << 2)
#define MPU6500_DISABLE_YG			(1 << 1)
#define MPU6500_DISABLE_ZG			(1 << 0)
#define MPU6500_WAKE_1_25HZ			0X00
#define MPU6500_WAKE_5HZ 			(0X01 << 6)
#define MPU6500_WAKE_20HZ			(0X02 << 6)
#define MPU6500_WAKE_40HZ			(0x03 << 6)

#define MPU6500_FIFO_COUNT_H		0x72

#define MPU6500_FIFO_COUNT_L		0x73

#define MPU6500_FIFO_R_W 			0x74

#define MPU6500_WHO_AM_I			0x75

#define MPU6500_XA_OFFSET_H			0x77

#define MPU6500_XA_OFFSET_L			0x78

#define MPU6500_YA_OFFSET_H			0x7A

#define MPU6500_YA_OFFSET_L			0x7B

#define MPU6500_ZA_OFFSET_H			0x7D

#define MPU6500_ZA_OFFSET_L			0x7E