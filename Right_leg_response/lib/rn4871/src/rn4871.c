#include "rn4871.h"
#include "usb_delay.h"
#include "string.h"

char rx_dma_buffer[LIO_BT_BUFFER_SIZE] = {'\0'};
char tx_dma_buffer[LIO_BT_BUFFER_SIZE] = {'\0'}; 



static void UUID_parser(char *UUID) {
    if (UUID == NULL) {
        return; // Handle invalid input
    }

    char *readPtr = UUID; // Read pointer
    char *writePtr = UUID; // Write pointer

    while (*readPtr != '\0') {
        if (*readPtr != '-') {
            *writePtr = *readPtr;
            writePtr++;
        }
        readPtr++;
    }

    *writePtr = '\0'; // Null-terminate the modified string
}

static void UUID_Flipper(char *input, char *output) {
    int len = strlen(input);
    for (int i = 0; i < len; i += 2) {
        output[len - i - 2] = input[i];
        output[len - i - 1] = input[i + 1];
    }
    output[len] = '\0';
}

void enter_command_mode(void){
    usb_delay_1ms(1000);
    char command_mode[] = "$$$";
    lio_send_bt(command_mode, 3);
    usb_delay_1ms(1000);
    command_mode[0] = '\r';
    lio_send_bt(command_mode, 1);
}

void lio_init_bt(){
    dma_parameter_struct dma_init_struct;
    /* enable DMA0 */
    rcu_periph_clock_enable(RCU_DMA0);
    /* initialize USART */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);

    usart_dma_transmit_config(USART0, USART_DENT_ENABLE);
    usart_dma_receive_config(USART0, USART_DENR_ENABLE);

   

    dma_deinit(DMA0, DMA_CH4);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)rx_dma_buffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = LIO_BT_BUFFER_SIZE;
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH4, &dma_init_struct);
        /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH4);
        /* enable DMA channel4 */
    dma_channel_enable(DMA0, DMA_CH4);
}

size_t lio_read_bt(char* buffer, uint32_t size){
    if(rx_dma_buffer[0] != 0)
    {
        int i = 0;
        for(; i < LIO_BT_BUFFER_SIZE && rx_dma_buffer[i] != 0 && i < size - 1; i++){
            buffer[i] = rx_dma_buffer[i];
            rx_dma_buffer[i] = 0;
        }
        buffer[i] = '\0';

        dma_parameter_struct dma_init_struct;

        dma_deinit(DMA0, DMA_CH4);
        dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
        dma_init_struct.memory_addr = (uint32_t)rx_dma_buffer;
        dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
        dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
        dma_init_struct.number = LIO_BT_BUFFER_SIZE;
        dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
        dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
        dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
        dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
        dma_init(DMA0, DMA_CH4, &dma_init_struct);
        /* configure DMA mode */
        dma_circulation_disable(DMA0, DMA_CH4);
        /* enable DMA channel4 */
        dma_channel_enable(DMA0, DMA_CH4);
        return i;
    }
    return 0;
}

size_t lio_send_bt(char* message, uint32_t size){
    for(int i = 0; i < size && i < LIO_BT_BUFFER_SIZE; i++) {
        tx_dma_buffer[i] = message[i];
    }

    dma_parameter_struct dma_init_struct;
    dma_deinit(DMA0, DMA_CH3);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)tx_dma_buffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = size;
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH3, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH3);
    /* enable DMA channel3 */
    dma_channel_enable(DMA0, DMA_CH3);
}

static int fuseStrings(char *str1, const char *str2) {
    char *start = str1;  // Save the starting position of str1

    // Find the end of the first string
    while (*str1 != '\0') {
        str1++;
    }
    // Copy characters from the second string to the end of the first string
    while (*str2 != '\0') {
        *str1 = *str2;
        str1++;
        str2++;
    }
    // Add the null terminator to mark the end of the concatenated string
    *str1 = '\0';
    // Calculate and return the length of the concatenated string
    return str1 - start;
}

void init_bluetooth(init_ble_struct init_struct){
    UUID_parser(init_struct.service_uuid);
    UUID_parser(init_struct.tx_characteristic_uuid);
    UUID_parser(init_struct.start_stop_characteristic_uuid);
    UUID_parser(init_struct.value_service_data_UUID);
    char flipped_uuid[64];
    UUID_Flipper(init_struct.service_uuid, flipped_uuid);

    char sendBuf[64] = {'\0'};
    int str_len = 0;

    enter_command_mode();
    usb_delay_1ms(400);
    printf("Starting init\r\n");
    /************* Complete factory reset ****************/
    sendBuf[0] = '\0';
    strcpy(sendBuf, "SF,1\r");

    lio_send_bt(sendBuf, 5);
    usb_delay_1ms(1000);
    /************* Complete factory reset ****************/

    enter_command_mode();
    usb_delay_1ms(400);

    



    /************* Sets name ****************/
    sendBuf[0] = '\0';
    strcpy(sendBuf, "SN,");
    str_len = fuseStrings(sendBuf,init_struct.name);
    str_len = fuseStrings(sendBuf, "\r");
    lio_send_bt(sendBuf, str_len);
    usb_delay_1ms(400);
    /************* Sets name ****************/
    
    /************* Disable low power mode ****************/
    sendBuf[0] = '\0';
    strcpy(sendBuf, "S0,0\r");
    lio_send_bt(sendBuf, 5);
    usb_delay_1ms(400);
    /************* Disable low power mode ****************/


    /************* Remove all Services and Characteristics ****************/
    sendBuf[0]= '\0';
    strcpy(sendBuf,"PZ\r");
    lio_send_bt(sendBuf,3);
    usb_delay_1ms(400);
    /************* Remove all Services and Characteristics ****************/    

    /************* Reboot ****************/
    sendBuf[0]= '\0';
    strcpy(sendBuf,"R,1\r");
    lio_send_bt(sendBuf,4);
    usb_delay_1ms(1000);
    /************* Reboot ****************/

    enter_command_mode();
    usb_delay_1ms(400);

    /************* Create service ****************/
    sendBuf[0] = '\0';
    strcpy(sendBuf, "PS,");
    str_len = fuseStrings(sendBuf, init_struct.service_uuid);
    str_len = fuseStrings(sendBuf, "\r");
    lio_send_bt(sendBuf, str_len);
    usb_delay_1ms(400);
    /************* Create service ****************/

    /************* Create characteristic  TX ****************/
    sendBuf[0] = '\0';
    strcpy(sendBuf, "PC,");
    str_len = fuseStrings(sendBuf, init_struct.tx_characteristic_uuid);
    str_len = fuseStrings(sendBuf, ",");
    str_len = fuseStrings(sendBuf, init_struct.tx_properties);
    str_len = fuseStrings(sendBuf, ",");
    str_len = fuseStrings(sendBuf, init_struct.message_size);
    str_len = fuseStrings(sendBuf, "\r");
    lio_send_bt(sendBuf, str_len);
    usb_delay_1ms(400);
    /************* Create characteristic TX ****************/

    /************* Create characteristic START-STOP ****************/
    sendBuf[0]= '\0';
    strcpy(sendBuf,"PC,");
    str_len = fuseStrings(sendBuf, init_struct.start_stop_characteristic_uuid);
    str_len = fuseStrings(sendBuf, ",");
    str_len = fuseStrings(sendBuf, init_struct.start_stop_properties);
    str_len = fuseStrings(sendBuf, ",");
    str_len = fuseStrings(sendBuf, init_struct.message_size);
    str_len = fuseStrings(sendBuf, "\r");
    lio_send_bt(sendBuf, str_len);
    usb_delay_1ms(400);
    /************* Create characteristic START-STOP ****************/

    /************* Create data service 1****************/
    sendBuf[0] = '\0';
    strcpy(sendBuf, "PS,");
    str_len = fuseStrings(sendBuf, init_struct.value_service_data_UUID);
    str_len = fuseStrings(sendBuf, "\r");
    lio_send_bt(sendBuf, str_len);
    usb_delay_1ms(400);
    /************* Create data service 1****************/

    /******************** Create caracteristic values ************************/
    for(int i = 0 ; i < init_struct.number_of_values; i++){
        UUID_parser(init_struct.value_characteristic_uuid[i]);
        sendBuf[0]= '\0';
        strcpy(sendBuf,"PC,");
        str_len = fuseStrings(sendBuf, init_struct.value_characteristic_uuid[i]);
        str_len = fuseStrings(sendBuf, ",");
        str_len = fuseStrings(sendBuf, init_struct.value_properties);
        str_len = fuseStrings(sendBuf, ",");
        str_len = fuseStrings(sendBuf, init_struct.message_size);
        str_len = fuseStrings(sendBuf, "\r");
        lio_send_bt(sendBuf, str_len);
        usb_delay_1ms(1000);
    }
    
    /******************** Create caracteristic values ************************/

    /************* Set Authentication ****************/
    sendBuf[0]= '\0';
    strcpy(sendBuf,"SA,2\r");
    lio_send_bt(sendBuf, 5);
    usb_delay_1ms(400);
    /************* Set Authentication ****************/

    /************* Set pin ****************/
    sendBuf[0]= '\0';
    strcpy(sendBuf,"SP,");
    str_len = fuseStrings(sendBuf, init_struct.pin_code);
    str_len = fuseStrings(sendBuf, "\r");
    lio_send_bt(sendBuf, str_len);
    usb_delay_1ms(400);
    /************* Set pin ****************/

    /************* Clear Advertise ****************/
    sendBuf[0] = '\0';
    strcpy(sendBuf, "IA,Z");
    str_len = fuseStrings(sendBuf, "\r");
    lio_send_bt(sendBuf, str_len);
    usb_delay_1ms(400);
    /************* Clear Advertise ****************/

    
    /************* Advertise UUID ****************/
    sendBuf[0] = '\0';
    strcpy(sendBuf, "NA,07,");
    str_len = fuseStrings(sendBuf, flipped_uuid);
    str_len = fuseStrings(sendBuf, "\r");
    //printf("This is the flipped uuid cmd: %s\n", flipped_uuid);
    lio_send_bt(sendBuf, str_len);
    usb_delay_1ms(400);
    /************* Advertise UUID ****************/

    /************* Reboot ****************/
    sendBuf[0]= '\0';
    strcpy(sendBuf,"R,1\r");
    lio_send_bt(sendBuf,4);
    usb_delay_1ms(1000);
    /************* Reboot ****************/
    return;
}

/* #include <stdio.h>
#include <stdint.h>

void write_handle_raw(uint16_t handle, uint8_t* byte_data, uint8_t size){
	uint8_t string_buffer[128] = {'\0'};
	uint8_t data_buffer[(20*2)+15] = {'\0'};
	const uint8_t lut_hex[] = "0123456789ABCDEF";
	for(int i = 0; i < size; i++){
		data_buffer[i*2] = lut_hex[byte_data[i] / 16];
		data_buffer[(i*2)+1] = lut_hex[byte_data[i] % 16];
	}
	data_buffer[size*2] = '\0';
	sprintf(string_buffer, "%s%.4x,%s\r\n", "SHW,", handle, data_buffer);
	printf("%s\n", string_buffer);
}

int main()
{
    uint8_t array[3] = {1,2,83};
    printf("Hello World\n");
    write_handle_raw(0x74,array,3);

    return 0;
} */