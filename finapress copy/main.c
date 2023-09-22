#include "gd32vf103.h"
#include "usb_serial_if.h"
#include "usb_delay.h"
#include "usart.h"
#include <stdlib.h>
#include <string.h>
#include "rn4871.h"
#include "radioHead.h"
#include "systick.h"
#include "gd32v_tf_card_if.h"
#include "ff.h"
#include "dma.h"
#include "queue.h"
#include "tf_card.h"
#include "state.h"
#include "adc.h"

#define BUFFER_SIZE 512

#define BLUETOOTH_START_MESSAGE "%WV,0072,7374617274%"      //phone send string "start"
#define BLUETOOTH_STOP_MESSAGE  "%WV,0072,73746F70%"        //phone send string "stop"

void ledInit (void);
void led_on_B1();
void led_off_B1();
void led_on_B0();
void led_off_B0();
void timer_interrupt_config();
void led_on_b2();
void led_off_b2();

volatile int start_time = 0;
volatile int millisec = 0;
volatile int seconds = 0;
volatile int minutes = 0;    

const char *commands[] = {
    "start0",
    "start1",
    "start2",
    "stopp0",
    "stopp1",
    "stopp2"
};

const int delays[] = {
    300,
    200,
    100,
    300,
    200,
    100
};

const int states[] = {
    START,
    START,
    START,
    STOP,
    STOP,
    STOP
};

int write = 0;
uint8_t txbuffer[8];
uint8_t txCount;

void formatTime(char *output, int minutes, int seconds, int milliseconds) {
    // Format minutes
    output[0] = (minutes / 10) + '0';
    output[1] = (minutes % 10) + '0';

    // Add a period
    output[2] = '.';

    // Format seconds
    output[3] = (seconds / 10) + '0';
    output[4] = (seconds % 10) + '0';

    // Add a period
    output[5] = '.';

    // Format milliseconds
    output[6] = ((milliseconds / 100) % 10) + '0';
    output[7] = ((milliseconds / 10) % 10) + '0';
    output[8] = (milliseconds % 10) + '0';

    // Add a semicolon and null terminator
    output[9] = ';';
    output[10] = '\0';
}

int int_to_string(char *string, int integer){
    int i = 0;
    int divider = 1000;
    if(integer < 0){
        string[i++] = '-';
        integer = -integer;
    }else if(integer == 0){
        string[i++] = '0';
        string[i++] = ';';
        string[i] = '\0';
        return i;
    }
    int og_num = integer;
    while (integer >= divider * 10) {
        divider *= 10; 
    }

    while(divider){
        char digit = (integer/divider)+ '0';
        if(digit != '0' || og_num > integer){
            string[i++] = digit; 
        }
        
        integer %= divider;
        divider /= 10;
    }
    string[i++] = ';';
    string[i] = '\0';
    return i;
}

int append_int_to_string(char *string, int integer){
    char temp[20] = {'\0'};
    int_to_string(temp, integer);
    int len = fuseStrings(string, temp);
    return len;
}

void append_newLine(char *string, int length) {
    string[length-1] = '\n';
    string[length] = '\0'; // Add a null terminator at the new end
}

int millis(void) {
    uint64_t mtime = get_timer_value();
    return ((mtime * 4000.0) / SystemCoreClock); // Adjusted for seconds and milliseconds
}

void TIMER1_IRQHandler(void){
    //led_on();
    if(!adc_flag_get(ADC0,ADC_FLAG_EOC)){
        uint16_t data_Buffer[8] = {0};
        char str_buf[256] = {'\0'};
        for (int i = 0; i < 8; i++){
            data_Buffer[i] = sample_buffer[i];
        }
        adc_flag_clear(ADC0, ADC_FLAG_EOC); 
        adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
        /* Check which channel triggered the interrupt */
        if (SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_CH0)){
            millisec++; // Add one each time an interrupt happens
            if (millisec == 1000){
                millisec = 0;
                seconds++;
            }

            if (seconds == 60){
                seconds = 0;
                minutes++;
            }

            formatTime(str_buf, minutes, seconds, millisec);
            append_int_to_string(str_buf, data_Buffer[0]);
            append_int_to_string(str_buf, data_Buffer[1]);
            append_int_to_string(str_buf, data_Buffer[2]);
            append_int_to_string(str_buf, data_Buffer[3]);
            append_int_to_string(str_buf, data_Buffer[4]);
            append_int_to_string(str_buf, data_Buffer[5]);
            append_int_to_string(str_buf, data_Buffer[6]);
            append_int_to_string(str_buf, data_Buffer[7]);
            append_newLine(str_buf, strlen(str_buf));

            enqueue_string(str_buf);
            timer_interrupt_flag_clear(TIMER1, TIMER_INT_CH0);
        }
    }
    //led_off();
}



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
	lio_send_bt(string_buffer, strlen(string_buffer));
}



int main(void){
    init_q();
    

    /* Handle for the mounted filesystem */
    FATFS fs;

    /* FatFs return code */
    volatile FRESULT fr;

    /* File handle */
    FIL fil;

    /* Used for bytes written */
    UINT bw = 0;
    
    /* Sets a valid date for when writing to file */
    set_fattime(1980, 1, 1, 0, 0, 0); // 1980 Jan 1st, 00:00:00
    delay_1ms(100);

    /* This function "mounts" the SD-card which makes the filesystem available */
    fr = f_mount(&fs, "test", 1); // Mount storage device
    f_sync(&fil);

    /* This function opens a file. In this case, we are creating a file which we want to write to */
    /* Filename should be at most 8 chars long and the extension should be at most 3 */
    
    delay_1ms(400);

    int ind_ble = 0;
    char read_buf[80] = {'\0'};
    char read_buf_ble[80] = {'\0'};
    //configure_usb_serial(); //måste vara före lio_init_bt
    lio_init_bt();
    init_radio();
    dma_config();
    init_ADC_A0_A7();
    ledInit();
    usb_delay_1ms(1);

    char write_to_sd[4096] = {'\0'};
    char buf[BUFFER_SIZE] = {'\0'};
    int buf_size = 0;
    char usb_data_buffer[4096] = {'\0'};
    int dequeued_str_len = 0;

    write = 0;

    int blink = 0;
    int onOff = 0;
    
    int file_Nr = 0;
    
    while(1){
        switch (get_state()){
            case START:
                eclic_global_interrupt_disable();    
                fr = f_mount(&fs, "test", 1); // Mount storage device
                f_sync(&fil);
                fr = f_open(&fil, "FINAP.CSV", FA_WRITE | FA_CREATE_ALWAYS);
                clear_queues();
                /* Write the header to the file */
                char header[] = "Time;A0;A1;A2;A3;A4;A5;A6;A7\n";
                //clear queue
                enqueue_string(header);
            
                millisec = 0;
                seconds = 0;
                minutes = 0;
                    
                //ENABLE interrupts for data collection
                set_state(RUNNING);
                write = 1;
                while(millis() < start_time);

                timer_interrupt_config();
                //SKicka till telefon "JAg startar nu"
                eclic_global_interrupt_enable();  
                break;

            case STOP:
                //STOP CONTROL
                timer_interrupt_disable(TIMER1, TIMER_INT_CH0);
                eclic_global_interrupt_disable();
                //Skicka till telefon jag stoppar nu
                write_to_sd[0] = '\0';
                while(empty_string_queue(write_to_sd, 8)){
                    f_write(&fil, write_to_sd, strlen(write_to_sd), &bw); // testa byt strlen till buf_size för bättre prestanda
                    f_sync(&fil);
                    write_to_sd[0] = '\0';
                }    

                f_close(&fil);
                write = 0;
                set_state(RUNNING);
                break;

            case RUNNING:
                //RUNNING CONTROL
                lio_read_bt(read_buf, 20);
                ind_ble = fuseStrings(read_buf_ble, read_buf);
                memset(read_buf, '\0', sizeof(read_buf));
                //me no likey code below
                blink++;
                if (blink == 100000){
                    blink = 0;
                    
                    if(onOff){
                        led_on_B0();
                        onOff = 0;
                    }else{
                        led_off_B0();
                        onOff = 1;
                    }
                }
  
                if(!strcmp(read_buf_ble, BLUETOOTH_START_MESSAGE)){
                    send_radio("empty",5);
                    delay_1ms(50);
                    send_radio("start0", 6);
                    memset(read_buf_ble, '\0', sizeof(read_buf_ble));
                    start_time = millis() + 1051;
                    led_on_B1();
                    set_state(START);
                    write = 1;
                    led_on_B1();
                }else if(!strcmp(read_buf_ble, BLUETOOTH_STOP_MESSAGE)){
                    send_radio("empty",5);
                    delay_1ms(50);
                    send_radio("stopp0", 6);
                    memset(read_buf_ble, '\0', sizeof(read_buf_ble));
                    set_state(STOP);
                    write = 0;
                    led_off_B1();
                }//Vi kan koll vilket kommand vi får när vi connectar för att ta bort det. Just nu så memsettar vi aldrig read_buf_ble så den kommer alltid ligga kvar där i buffern, vilket gör att disconnectar telefonen och sen connectar så kan vi ej starta mätningen

                //BREAK IF PROGRAM SHOULDNT WRITE
                if(!write){
                    break;
                }

                //WRITE TO FILE
                if (queue_str_len() > 8){
                    write_to_sd[0] = '\0';
                    buf_size = dequeue_string(write_to_sd, 8);
                    if(buf_size > 0){
                        //led_on();
                        f_write(&fil, write_to_sd, strlen(write_to_sd), &bw); // testa byt strlen till buf_size för bättre prestanda
                        f_sync(&fil);
                        write_to_sd[0] = '\0';
                        buf_size = 0;
                        //led_off();
                    }
                } 
                break;
            default:
                break;
        }
    }
}

void ledInit (void){
    rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
}

void led_on_B1(){
    gpio_bit_write(GPIOB, GPIO_PIN_1, 1);
}
void led_off_B1(){
    gpio_bit_write(GPIOB, GPIO_PIN_1, 0);
}
void led_on_b2(){
    gpio_bit_write(GPIOB, GPIO_PIN_2, 1);
}

void led_off_b2(){
    gpio_bit_write(GPIOB, GPIO_PIN_2, 0);
}

void led_on_B0(){
    gpio_bit_write(GPIOB, GPIO_PIN_0, 1);
}
void led_off_B0(){
    gpio_bit_write(GPIOB, GPIO_PIN_0, 0);
}

void timer_interrupt_config(){
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    eclic_irq_enable(TIMER1_IRQn, 1, 0);

    rcu_periph_clock_enable(RCU_TIMER1);

    timer_deinit(TIMER1);

    timer_struct_para_init(&timer_initpara);

    /* TIMER1 configuration */
    /* Frequency = core clock / ((1+prescaler)*period) = 108MHz / (2*54000) = 1KHz */
    timer_initpara.prescaler = 1;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = SystemCoreClock/2000;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_init(TIMER1, &timer_initpara);

    timer_channel_output_struct_para_init(&timer_ocinitpara);

    timer_ocinitpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocinitpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_ocinitpara);

    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, SystemCoreClock / 4000);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_TIMING);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    timer_interrupt_enable(TIMER1, TIMER_INT_CH0);
    /* Make sure interrupt flag is clear */
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_CH0);
    timer_enable(TIMER1);
}