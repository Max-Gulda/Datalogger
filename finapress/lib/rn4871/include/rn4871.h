#pragma once
#include "gd32vf103.h"
#include "usb_serial_if.h"

//Finapress
#define finapressServiceUUID                        "6E54EED4-9FC2-4EC4-BE2B-D6FBE2B3D9C7" //service
#define finapressCharacteristicUUID_tx              "A2D0DB30-5C84-4377-BE6D-07F9ACD958E9" //TX
#define finapressCharacteristicUUID_start_stop      "6BCBB238-6652-4894-8332-B2D0124733A5" //Start-stop
#define finapressServiceDataUUID                    "DAAB73E7-4E08-4DF9-88B5-560655A8024E" //Service data UUID1
#define finapressCharacteristicUUID_ADC_A0          "CE9D265D-FBFA-4771-BA57-986D9A55C0A3" //ADC_A0 
#define finapressCharacteristicUUID_ADC_A1          "F639D3CC-21C5-44D7-A115-BBB1731A3D86" //ADC_A1
#define finapressCharacteristicUUID_ADC_A2          "E189BE3E-355D-4FC1-A0D5-8651F600900C" //ADC_A2
#define finapressCharacteristicUUID_ADC_A3          "B723FA0C-F044-4871-843D-79A7C3201CCF" //ADC_A3
#define finapressCharacteristicUUID_ADC_A4          "1F7E8039-C922-4609-AF61-E602780396C9" //ADC_A4
#define finapressCharacteristicUUID_ADC_A5          "7BC207C6-3192-4890-A4A0-FE627630D663" //ADC_A5
#define finapressCharacteristicUUID_ADC_A6          "1088C846-5D02-431A-9C41-1E05DBC3CE2D" //ADC_A6
#define finapressCharacteristicUUID_ADC_A7          "44809D0C-28B0-45E7-9D17-026F8254C55A" //ADC_A7

#define finapressHandle_start_stop                   0x75
#define finapressHandle_ADC_A0                       0x92
#define finapressHandle_ADC_A1                       0x95
#define finapressHandle_ADC_A2                       0x98
#define finapressHandle_ADC_A3                       0x9B
#define finapressHandle_ADC_A4                       0x9E
#define finapressHandle_ADC_A5                       0xA1
#define finapressHandle_ADC_A6                       0xA4
#define finapressHandle_ADC_A7                       0xA7

//body

//Left_leg
#define leftLegServiceUUID                          "64530F27-3632-4070-82DA-3E6BA60B27AD"  //service  
#define leftLegCharacteristicUUID_tx                "9CC29B03-C3E1-421D-949B-5763479C3D33"  //TX
#define leftLegCharacteristicUUID_start_stop        "4C8C4047-7CC9-4BDF-A008-4ED0B35EF3AA"  //Start-stop send from mcu
#define leftLegServiceDataUUID                      "334F7C35-0F96-4443-A364-B7DDD07E9AE1"  //Service data UUID1
#define leftLegCharacteristicUUID_ACC_X             "07980EF5-7E56-4F40-91B0-BD312F86C62F"  //Acceleration x
#define leftLegCharacteristicUUID_ACC_Y             "808FF366-ABCD-42FE-82BC-3E0E2CA6636E"  //Acceleration y
#define leftLegCharacteristicUUID_ACC_Z             "F6DFC378-A314-448C-902C-97016FB314C9"  //Acceleration Z
#define leftLegCharacteristicUUID_GYRO_X            "5D78290C-50BF-4EEC-BF6D-50F0C42BF432"  //Gyroscope X
#define leftLegCharacteristicUUID_GYRO_Y            "55CB1DCE-706C-4992-951E-A91B24433E4F"  //Gyroscope Y
#define leftLegCharacteristicUUID_GYRO_Z            "F68CE15C-95E0-474D-90F7-D5D9137CD9B9"  //Gyroscope Z
#define leftLegCharacteristicUUID_EMG               "5DEB7A06-CA45-4EF2-9309-DE897B03D549"  //EMG

//chest
#define chestServiceUUID                            "26931357-A022-4C29-AA56-BCBBB3C72C94"  //service
#define chestCharacteristicUUID_tx                  "3DF2D329-A9D4-4D0E-9600-154072BE7544"  //TX
#define chestCharacteristicUUID_start_stop          "1FF7D19B-5B6F-4A30-81BB-C76A61D71D98"  //Start-stop send from mcu
#define chestServiceDataUUID                        "8A82BEA0-7A31-49A7-8771-2BF0E56B1955"  //Service data UUID1
#define chestCharacteristicUUID_ACC_X               "2C15828E-71D6-4792-AA29-1F96E2189E96"  //Acceleration x
#define chestCharacteristicUUID_ACC_Y               "42403077-321E-49DE-9538-0D1F7C8CE15D"  //Acceleration y
#define chestCharacteristicUUID_ACC_Z               "D2F534FF-C04F-4466-A22F-64622A47EB23"  //Acceleration Z
#define chestCharacteristicUUID_GYRO_X              "4292DC20-A07D-4993-A71F-4A9CBDC11EDB"  //Gyroscope X
#define chestCharacteristicUUID_GYRO_Y              "0F845E80-CB2D-49A0-8DEA-52A5B5C289B8"  //Gyroscope Y
#define chestCharacteristicUUID_GYRO_Z              "EF25FE39-AEA3-4079-86D6-1A5EB341EEE1"  //Gyroscope Z
#define chestCharacteristicUUID_EMG                 "3B427CE5-34B3-459D-8F0F-FE5A3506A47A"  //EMG

//Right_leg
#define rightLegServiceUUID                         "99FD4277-7073-4DDD-8486-A3EB735A832C"  //service
#define rightLegCharacteristicUUID_tx               "32489387-B321-4329-B4F3-413F7F0E5F08"  //TX
#define rightLegCharacteristicUUID_start_stop       "43AD0C7C-AAB2-42A8-B880-81457B8722BC"  //Start-stop send from mcu
#define rightLegServiceDataUUID                     "F86BA921-6AEB-4B67-BB58-668049C9E588"  //Service data UUID
#define rightLegCharacteristicUUID_ACC_X            "DD64FED8-BE2A-4052-9E84-3054FFC21058"  //Acceleration x
#define rightLegCharacteristicUUID_ACC_Y            "062B4B58-7583-469B-B579-391B70252575"  //Acceleration y
#define rightLegCharacteristicUUID_ACC_Z            "6E387753-E305-461A-9F0A-9955252AA6A9"  //Acceleration Z
#define rightLegCharacteristicUUID_GYRO_X           "D29E3D76-6220-4218-BF1C-DCC17BC967DB"  //Gyroscope X
#define rightLegCharacteristicUUID_GYRO_Y           "3600361E-FFEF-46B9-8EA7-4BA1DB5D3F2C"  //Gyroscope Y
#define rightLegCharacteristicUUID_GYRO_Z           "1920F52D-E85D-4A72-B917-B59AFA425621"  //Gyroscope Z
#define rightLegCharacteristicUUID_EMG              "B86949BA-1DE3-4044-8C26-6DD5B0B9031A"  //EMG

#define Handle_start_stop                   0x75
#define Handle_ACC_X                        0x92
#define Handle_ACC_Y                        0x95
#define Handle_ACC_Z                        0x98
#define Handle_GYRO_X                       0x9B
#define Handle_GYRO_Y                       0x9E
#define Handle_GYRO_Z                       0xA1
#define Handle_EMG                          0xA4



#define MESSAGE_SIZE_MIN                            "01"
#define MESSAGE_SIZE_ONE                            "02"
#define MESSAGE_SIZE_TWO                            "03"
#define MESSAGE_SIZE_THREE                          "04"
#define MESSAGE_SIZE_FOUR                           "05"
#define MESSAGE_SIZE_FIVE                           "06"
#define MESSAGE_SIZE_SIX                            "07"
#define MESSAGE_SIZE_SEVEN                          "08"
#define MESSAGE_SIZE_EIGHT                          "09"
#define MESSAGE_SIZE_NINE                           "10"
#define MESSAGE_SIZE_TEN                            "11"
#define MESSAGE_SIZE_ELEVEN                         "12"
#define MESSAGE_SIZE_TWELVE                         "13"
#define MESSAGE_SIZE_MAX                            "14"

#define NOTIFY                                      "1"
#define INDICATE                                    "2"
#define NOTIFY_INDICATE                             "3"

#define READ                                        "2"
#define WRITE_WITHOUT_RESPONSE                      "4"
#define READ_WRITE_WITHOUT_RESPONSE                 "6"
#define WRITE_WITH_RESPONSE                         "8"
#define READ_WRITE_WITH_RESPONSE                    "A"
#define WRITE_WITH_RESPONSE_WITHOUT_RESPONSE        "C"
#define READ_RITE_WITH_RESPONSE_WITHOUT_RESPONSE    "E"

#define LIO_BT_BUFFER_SIZE 1024

typedef struct{
    char name[21];
    char service_uuid[64];
    char tx_characteristic_uuid[64];
    char start_stop_characteristic_uuid[64];
    char value_service_data_UUID[64];
    char value_characteristic_uuid[8][64];
    char tx_properties[5];
    char start_stop_properties[5];
    char value_properties[5];
    int  number_of_values;
    char message_size[3]; //0x01 - 0x14
    char pin_code[7];
}init_ble_struct;

void enter_command_mode(void);
void lio_init_bt();
size_t lio_read_bt(char* buffer, uint32_t size);
size_t lio_send_bt(char* message, uint32_t size);
//int fuseStrings(char *str1, const char *str2);
void init_bluetooth(init_ble_struct init_struct);