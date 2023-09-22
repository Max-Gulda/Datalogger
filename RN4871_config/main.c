#include "gd32vf103.h"
#include "usb_serial_if.h"
#include "usb_delay.h"
#include "usart.h"
#include <stdlib.h>
#include <string.h>
#include "rn4871.h"

//extern char tx_dma_buffer[LIO_BT_BUFFER_SIZE] = {'\0'}; 
//extern char rx_dma_buffer[LIO_BT_BUFFER_SIZE] = {'\0'};

void ledInit (void){
    rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);
}
void led_on(){
    gpio_bit_write(GPIOB, GPIO_PIN_1, 1);
}
void led_off(){
    gpio_bit_write(GPIOB, GPIO_PIN_1, 0);
}

void ledOnOff(int set){
    gpio_bit_write(GPIOB, GPIO_PIN_0, set);
}

void handleBackspace(char *buffer, int *length) {
    (*length)--;
    buffer[*length] = '\0';
    printf(" \b"); // Move cursor back, erase character, move cursor back again
    fflush(0);
}

int main(void){
    uint32_t count = 0;
    char usb_data_buffer[LIO_BT_BUFFER_SIZE] = {'\0'};
    int ind_usb = 0;
    int ind_ble = 0;
    char usb_read_buf[80] = {'\0'};
    char ble_read_buf[80] = {'\0'};
    char read_buf[80] = {'\0'};
    configure_usb_serial(); //måste vara före lio_init_bt
    lio_init_bt();
    ledInit();
    usb_delay_1ms(1);

    while (!usb_serial_available()) {
        usb_delay_1ms(100);
    }

    printf("\r\nReady\r\n");
    fflush(0);

    while(1){
        count = read_usb_serial(usb_data_buffer);

        if (count > 0){
            printf("%s",usb_data_buffer);
            fflush(0);
            ind_usb = fuseStrings(usb_read_buf, usb_data_buffer);
            if (ind_usb > 0 && usb_read_buf[ind_usb - 1] == '\b') {
                handleBackspace(usb_read_buf, &ind_usb);
            }
            if(usb_read_buf[ind_usb-1] == '\n' || usb_read_buf[ind_usb-1] == '\r'){
                if(!strcmp(usb_read_buf,"$$$\r") || !strcmp(usb_read_buf,"$$$\n")){
                    enter_command_mode();
                }else if(!strcmp(usb_read_buf,"init_right\r") || !strcmp(usb_read_buf,"init_right\n")){
                    init_ble_struct ibs;
                    strcpy(ibs.name, "Right_leg");
                    strcpy(ibs.service_uuid, rightLegServiceUUID);
                    strcpy(ibs.start_stop_characteristic_uuid, rightLegCharacteristicUUID_start_stop);
                    strcpy(ibs.tx_characteristic_uuid, rightLegCharacteristicUUID_tx);
                    strcpy(ibs.value_service_data_UUID, rightLegServiceDataUUID);
                    strcpy(ibs.value_characteristic_uuid[0],rightLegCharacteristicUUID_ACC_X);
                    strcpy(ibs.value_characteristic_uuid[1],rightLegCharacteristicUUID_ACC_Y);
                    strcpy(ibs.value_characteristic_uuid[2],rightLegCharacteristicUUID_ACC_Z);
                    strcpy(ibs.value_characteristic_uuid[3],rightLegCharacteristicUUID_GYRO_X);
                    strcpy(ibs.value_characteristic_uuid[4],rightLegCharacteristicUUID_GYRO_Y);
                    strcpy(ibs.value_characteristic_uuid[5],rightLegCharacteristicUUID_GYRO_Z);
                    strcpy(ibs.value_characteristic_uuid[6],rightLegCharacteristicUUID_EMG);
                    ibs.number_of_values = 7;
                    strcpy(ibs.tx_properties, "1C");
                    strcpy(ibs.start_stop_properties, "0C");
                    strcpy(ibs.value_properties,"1C");
                    strcpy(ibs.pin_code, "1234");
                    strcpy(ibs.message_size, MESSAGE_SIZE_MAX);
                    init_bluetooth(ibs);
                    printf("\r\nInitiation Successfull!\r\n");
                }else if(!strcmp(usb_read_buf,"init_left\r") || !strcmp(usb_read_buf,"init_left\n")){
                    init_ble_struct ibs;
                    strcpy(ibs.name, "left_leg");
                    strcpy(ibs.service_uuid, leftLegServiceUUID);
                    strcpy(ibs.start_stop_characteristic_uuid, leftLegCharacteristicUUID_start_stop);
                    strcpy(ibs.tx_characteristic_uuid, leftLegCharacteristicUUID_tx);
                    strcpy(ibs.value_service_data_UUID, leftLegServiceDataUUID);
                    strcpy(ibs.value_characteristic_uuid[0],leftLegCharacteristicUUID_ACC_X);
                    strcpy(ibs.value_characteristic_uuid[1],leftLegCharacteristicUUID_ACC_Y);
                    strcpy(ibs.value_characteristic_uuid[2],leftLegCharacteristicUUID_ACC_Z);
                    strcpy(ibs.value_characteristic_uuid[3],leftLegCharacteristicUUID_GYRO_X);
                    strcpy(ibs.value_characteristic_uuid[4],leftLegCharacteristicUUID_GYRO_Y);
                    strcpy(ibs.value_characteristic_uuid[5],leftLegCharacteristicUUID_GYRO_Z);
                    strcpy(ibs.value_characteristic_uuid[6],leftLegCharacteristicUUID_EMG);
                    ibs.number_of_values = 7;
                    strcpy(ibs.tx_properties, "1C");
                    strcpy(ibs.start_stop_properties, "0C");
                    strcpy(ibs.value_properties,"1C");
                    strcpy(ibs.pin_code, "1234");
                    strcpy(ibs.message_size, MESSAGE_SIZE_MAX);
                    init_bluetooth(ibs);
                    printf("\r\nInitiation Successfull!\r\n");
                }else if(!strcmp(usb_read_buf,"init_chest\r") || !strcmp(usb_read_buf,"init_chest\n")){
                    init_ble_struct ibs;
                    strcpy(ibs.name, "chest");
                    strcpy(ibs.service_uuid, chestServiceUUID);
                    strcpy(ibs.start_stop_characteristic_uuid, chestCharacteristicUUID_start_stop);
                    strcpy(ibs.tx_characteristic_uuid, chestCharacteristicUUID_tx);
                    strcpy(ibs.value_service_data_UUID, chestServiceDataUUID);
                    strcpy(ibs.value_characteristic_uuid[0],chestCharacteristicUUID_ACC_X);
                    strcpy(ibs.value_characteristic_uuid[1],chestCharacteristicUUID_ACC_Y);
                    strcpy(ibs.value_characteristic_uuid[2],chestCharacteristicUUID_ACC_Z);
                    strcpy(ibs.value_characteristic_uuid[3],chestCharacteristicUUID_GYRO_X);
                    strcpy(ibs.value_characteristic_uuid[4],chestCharacteristicUUID_GYRO_Y);
                    strcpy(ibs.value_characteristic_uuid[5],chestCharacteristicUUID_GYRO_Z);
                    strcpy(ibs.value_characteristic_uuid[6],chestCharacteristicUUID_EMG);
                    ibs.number_of_values = 7;
                    strcpy(ibs.tx_properties, "1C");
                    strcpy(ibs.start_stop_properties, "0C");
                    strcpy(ibs.value_properties,"1C");
                    strcpy(ibs.pin_code, "1234");
                    strcpy(ibs.message_size, MESSAGE_SIZE_MAX);
                    init_bluetooth(ibs);
                    printf("\r\nInitiation Successfull!\r\n");
                }else if(!strcmp(usb_read_buf,"init_fina\r") || !strcmp(usb_read_buf,"init_fina\n")){
                    init_ble_struct ibs;
                    strcpy(ibs.name, "finapress");
                    strcpy(ibs.service_uuid, finapressServiceUUID);
                    strcpy(ibs.start_stop_characteristic_uuid, finapressCharacteristicUUID_start_stop);
                    strcpy(ibs.tx_characteristic_uuid, finapressCharacteristicUUID_tx);
                    strcpy(ibs.value_service_data_UUID, finapressServiceDataUUID);
                    strcpy(ibs.value_characteristic_uuid[0],finapressCharacteristicUUID_ADC_A0);
                    strcpy(ibs.value_characteristic_uuid[1],finapressCharacteristicUUID_ADC_A1);
                    strcpy(ibs.value_characteristic_uuid[2],finapressCharacteristicUUID_ADC_A2);
                    strcpy(ibs.value_characteristic_uuid[3],finapressCharacteristicUUID_ADC_A3);
                    strcpy(ibs.value_characteristic_uuid[4],finapressCharacteristicUUID_ADC_A4);
                    strcpy(ibs.value_characteristic_uuid[5],finapressCharacteristicUUID_ADC_A5);
                    strcpy(ibs.value_characteristic_uuid[6],finapressCharacteristicUUID_ADC_A6);
                    strcpy(ibs.value_characteristic_uuid[7],finapressCharacteristicUUID_ADC_A7);
                    ibs.number_of_values = 8;
                    strcpy(ibs.tx_properties, "1C");
                    strcpy(ibs.start_stop_properties, "0C");
                    strcpy(ibs.value_properties,"1C");
                    strcpy(ibs.pin_code, "1234");
                    strcpy(ibs.message_size, MESSAGE_SIZE_MAX);
                    init_bluetooth(ibs);
                    printf("\r\nInitiation Successfull!\r\n");
                }
                else if (usb_read_buf[ind_usb-1] == '\r' && ind_usb == 1){
                    // Null-terminate the message and send it
                    usb_read_buf[ind_usb] = '\0';
                    lio_send_bt(usb_read_buf, 1);
                    printf("\r\n");
                }else {
                    lio_send_bt(usb_read_buf, ind_usb);
                    usb_read_buf[0] = '\0';
                    printf("\r\n");
                }                
            }
        }

                // Read data from the BLE module
        if(usart_flag_get(USART0,USART_FLAG_RBNE) || 1){
            lio_read_bt(ble_read_buf, 20);

            // Fuse the received data with the existing buffer
            ind_ble = fuseStrings(read_buf, ble_read_buf);
            memset(ble_read_buf, '\0', sizeof(ble_read_buf));
            //if(!strcmp(read_buf, "CMD> ")){
            //    printf("CMD> ");
            //    fflush(0);
            //    read_buf[0] = '\0';
            //    ind_ble = 0;
            //}
            if(ind_ble>2 && read_buf[0] == '%' && read_buf[ind_ble-1] == '%'){
                printf("%s\r\n", read_buf);
                fflush(0);
                read_buf[0] = '\0';
                ind_ble = 0;
            }
            // Check if a complete line (ending with '\r' and '\n') is received
            if (read_buf[ind_ble - 2] == '\r' && read_buf[ind_ble-1] == '\n') {
                read_buf[ind_ble - 1 ] = '\0';
                // Print the received data
                printf("%s\r\n", read_buf);
                //printf("I'm here\r\n");
                fflush(0);
                
                // Process the received command
                if (!strcmp(read_buf, "start")) {
                    led_on();
                } else if (!strcmp(read_buf, "stop")) {
                    led_off();
                }
                // Reset the buffer
                //memset(read_buf, '\0', sizeof(read_buf));
                //memset(ble_read_buf, '\0', sizeof(ble_read_buf));
                read_buf[0] = '\0';
                ind_ble = 0;
                fflush(0);
            } else if (!strcmp(read_buf, "CMD> ")){
                printf("%s ", read_buf);
                fflush(0);
                read_buf[0] = '\0';
                ind_ble = 0;
            }
        }
        fflush(0);
    }
}
