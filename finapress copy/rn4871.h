#pragma once
#include "gd32vf103.h"
#include "usb_serial_if.h"

//Right_leg
#define rightLegServiceUUID                         "80EC3A15-6948-46E4-8BF8-7C2063EFDFA6" //service
#define rightLegCharacteristicUUID_rx               "409FB59E-A14A-41EC-AB5F-CA4FD93D8AB3" //RX
#define rightLegCharacteristicUUID_tx               "C1D8328C-BD5D-4250-9E1B-14C2D3DC4110" //TX

//Left_leg
#define leftLegServiceUUID                          "2485E8EB-7239-47DA-8EAA-6B251EFBD20C"  //service  
#define leftLegCharacteristicUUID_rx                "0DBD38F8-1177-4577-BA91-37296865A474"  //RX
#define leftLegCharacteristicUUID_tx                "75ACD207-0A86-46C0-8FD1-99CB35372A48"  //TX

//Chest
#define chestServiceUUID                            "33960947-AE37-4BAA-9B94-A9A79ACF9E6C"  //service
#define chestCharacteristicUUID_rx                  "90701485-0439-4A70-BD9F-888900D26BBF"  //RX
#define chestCharacteristicUUID_tx                  "7F44469D-E7D8-41DB-B87A-FF9C549EC27B"  //TX

//Finapress
#define finapressServiceUUID                        "E290F551-39E1-480E-B624-3BCF7FA03C08"  //service
#define finapressCharacteristicUUID_rx              "74A57670-7419-4A48-B54A-2B7961A2FC7A"  //RX
#define finapressCharacteristicUUID_tx              "E3377CFC-D793-4588-8D20-10D51D390058"  //TX


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
    char rx_characteristic_uuid[64];
    char tx_properties[5];
    char rx_properties[5];
    char message_size[3]; //0x01 - 0x14
    char pin_code[7];
}init_ble_struct;

void enter_command_mode(void);
void lio_init_bt();
size_t lio_read_bt(char* buffer, uint32_t size);
size_t lio_send_bt(char* message, uint32_t size);
void init_bluetooth(init_ble_struct init_struct);