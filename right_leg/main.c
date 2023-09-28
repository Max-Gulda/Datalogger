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
#include "gd32v_mpu6500_if.h"
#include "dma.h"
#include "mpu6500_registers.h"
#include "mpu6500_driver.h"
#include "queue.h"
#include "tf_card.h"
#include "state.h"

#define BUFFER_SIZE 512
#define ANALOG_PORT              GPIOA
#define ANALOG_PIN               GPIO_PIN_3
#define ANALOG_CHANNEL           ADC_CHANNEL_3

/* FILENAME should be at most 8 chars long and the extension should be at most 3 */
//FILENAME INCLUDE RIGHTL.CSV, LEFTL.CSV, CHEST.CSV 
#define FILENAME_RIGHTL "RIGHTL.CSV"
//#define FILENAME_LEFTL "LEFTL.CSV"
//#define FILENAME_CHEST "CHEST.CSV"

volatile int16_t last_Sample_buffer[7] = {0};

void ledInit (void);
void led_on_B1();
void led_off_B1();
void led_on_B0();
void led_off_B0();
void timer_interrupt_config();
void init_ADC_EMG();
void init_mpu(void);
int millis(void);
void append_newLine(char *string, int length);
int append_int_to_string(char *string, int integer);
int int_to_string(char *string, int integer);
void formatTime(char *output, int minutes, int seconds, int milliseconds);

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
int ind_radio = 0;
char read_buf_radio[80] = {'\0'};
char read_buf[80] = {'\0'};

uint8_t rxbuffer[8];
uint8_t rxCount;

volatile int start_time = 0;
volatile int16_t milli = 0;
volatile int32_t seconds = 0;
volatile int16_t minutes = 0;
volatile int32_t emg = 0;              
uint8_t* pDatabuffer = (uint8_t*)&dataBuffer;

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

void send_data_to_phone(uint16_t handle, int16_t value){
    uint8_t bytes[2] = {0};
    bytes[0] = (uint8_t)((value >> 8) & 0xFF);  // Extract high byte 
    bytes[1] = (uint8_t)(value & 0xFF);   // Extract low byte
    write_handle_raw(handle,bytes,2);
}

uint16_t handles[] = {
    Handle_ACC_X,
    Handle_ACC_Y,
    Handle_ACC_Z,
    Handle_GYRO_X,
    Handle_GYRO_Y,
    Handle_GYRO_Z,
    Handle_EMG
};

void stagger_data_to_phone(int16_t seconds) {
    int16_t value = 0;
    int emg = 0;

    // Calculate the index based on seconds, cycling through handles
    int index = (seconds - 1) % 7; // Adjust the size according to the number of handles

    // Send data to the handle corresponding to the index
    value = last_Sample_buffer[index];

    send_data_to_phone(handles[index], value);
}

void reboot_BT(){
    lio_send_bt("R,1\r",4);//reboot bluetooth
    delay_1ms(50);
}

int main(void){
    init_q();
    init_ADC_EMG();
    //configure_usb_serial(); //måste vara före lio_init_bt
    lio_init_bt();
    ledInit();
    usb_delay_1ms(1);
    int rxcount=0;
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
    
    delay_1ms(400);

    init_mpu();

    rcu_periph_clock_enable(RCU_DMA0);

    delay_1ms(20);

    char write_to_sd[2560] = {'\0'};
    char buf[BUFFER_SIZE] = {'\0'};
    
/*     while (!usb_serial_available()) {
        usb_delay_1ms(100);
    } */
    init_radio();
     
    write = 0;

    int blink = 0;
    int onOff = 0;
    reboot_BT();
    enter_command_mode();
    send_data_to_phone(Handle_start_stop,0);
    while(1){
        switch (get_state()){
            case START:
                timer_interrupt_disable(TIMER1, TIMER_INT_CH0);
                eclic_global_interrupt_disable();    
                fr = f_mount(&fs, "test", 1); // Mount storage device
                f_sync(&fil);

                #ifdef FILENAME_RIGHTL
                    fr = f_open(&fil, FILENAME_RIGHTL, FA_WRITE | FA_CREATE_ALWAYS);
                #endif

                #ifdef FILENAME_LEFTL
                    fr = f_open(&fil, FILENAME_LEFTL, FA_WRITE | FA_CREATE_ALWAYS);
                #endif

                #ifdef FILENAME_CHEST
                    fr = f_open(&fil, FILENAME_CHEST, FA_WRITE | FA_CREATE_ALWAYS);
                #endif

                clear_queues();
                /* Write the header to the file */
                char header[] = "Time;X-acc;Y-acc;Z-acc;X-deg/s;Y-deg/s;Z-deg/s;EMG\n";
                //clear queue
                enqueue_string(header);

                milli = 0;
                seconds = 0;
                minutes = 0;

                while(millis() < start_time);
                //START IMU DMA
                i2c_dma_master_read_register(I2C0, MPU6500_ADDR, MPU6500_ACCEL_XOUT_H);
                //ENABLE interrupts for data collection
                set_state(RUNNING);
                write = 1;
                timer_interrupt_config();
                config_clic_irqs();
                send_data_to_phone(Handle_start_stop,1);
                eclic_global_interrupt_enable();  
                break;

            case STOP:
                //STOP CONTROL
                timer_interrupt_disable(TIMER1, TIMER_INT_CH0);
                eclic_global_interrupt_disable();
                send_data_to_phone(Handle_start_stop,0);
                write_to_sd[0] = '\0';
                while(empty_string_queue(write_to_sd, 10)){
                    f_write(&fil, write_to_sd, strlen(write_to_sd), &bw); // testa byt strlen till buf_size för bättre prestanda
                    f_sync(&fil);
                    write_to_sd[0] = '\0';
                }    

                f_close(&fil);
                set_state(RUNNING);
                write = 0;
                eclic_global_interrupt_enable();
                     
                break;

            case RUNNING:
                //RUNNING CONTROL
                blink++;
                if (blink == 1000000){
                    blink = 0;
                    if(onOff){
                        led_on_B0();
                        onOff = 0;
                    }else{
                        led_off_B0();
                        onOff = 1;
                    }
                }
                //BREAK IF PROGRAM SHOULDNT WRITE
                if(!write){
                    break;
                }
                
                if(queue_str_len() > 5){ //Behövs högre hastighet så kan vi öka denna 5:an
                    memset(write_to_sd, '\0', sizeof(write_to_sd));
                    if (dequeue_string(write_to_sd, 5)) { //Behövs högre hastighet så kan vi öka denna 5:an
                        f_write(&fil, write_to_sd, strlen(write_to_sd), &bw);
                        f_sync(&fil);
                    }
                }
                break;
            default:
                break;
        }
    }
}

void TIMER1_IRQHandler(void){
    char buf[64] = {'\0'};
    int buf_size = 0;   
    dma_vector_t vector_write;
    /* Check which channel triggered the interrupt */
    if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_CH0)){
        milli++;
        if(milli == 1000){
            milli = 0;
            seconds++;
        }
        if(seconds==60){
            seconds = 0;
            minutes++;
        }

        #ifdef FILENAME_RIGHTL
            if(milli == 0){ //Right leg
                stagger_data_to_phone(seconds);
            }
        #endif 

        #ifdef FILENAME_LEFTL
            if(milli == 200){ //Left leg
                stagger_data_to_phone(seconds);
            }
        #endif 

        #ifdef FILENAME_CHEST
            if(milli == 400){ //Chest
                stagger_data_to_phone(seconds);
            }
        #endif

        formatTime(buf,minutes,seconds,milli);
        last_Sample_buffer[0] = (int16_t)((*(pDatabuffer + 0  ) << 8) | *(pDatabuffer + 1  ));
        last_Sample_buffer[1] = (int16_t)((*(pDatabuffer + 2  ) << 8) | *(pDatabuffer + 3  ));
        last_Sample_buffer[2] = (int16_t)((*(pDatabuffer + 4  ) << 8) | *(pDatabuffer + 5  ));
        last_Sample_buffer[3] = (int16_t)((*(pDatabuffer + 8  ) << 8) | *(pDatabuffer + 9  ));
        last_Sample_buffer[4] = (int16_t)((*(pDatabuffer + 10 ) << 8) | *(pDatabuffer + 11 ));
        last_Sample_buffer[5] = (int16_t)((*(pDatabuffer + 12 ) << 8) | *(pDatabuffer + 13 ));
        last_Sample_buffer[6] = ADC_RDATA(ADC0);

        append_int_to_string(buf, last_Sample_buffer[0]);
        append_int_to_string(buf, last_Sample_buffer[1]);
        append_int_to_string(buf, last_Sample_buffer[2]);
        append_int_to_string(buf, last_Sample_buffer[3]);
        append_int_to_string(buf, last_Sample_buffer[4]);
        append_int_to_string(buf, last_Sample_buffer[5]);
        append_int_to_string(buf, last_Sample_buffer[6]);

        append_newLine(buf, strlen(buf)); //Testa byt strlen till buf_size för bättre prestanda
        enqueue_string(buf);

        i2c_dma_master_read_register(I2C0, MPU6500_ADDR, MPU6500_ACCEL_XOUT_H);
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_CH0);
    }
}

void USART2_IRQHandler(void){
    if(RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE)){
        start_time = millis() + 1000;
        rxbuffer[rxCount++] = usart_data_receive(USART2);
        if(rxbuffer[0] != 's'){
            rxCount = 0;
            memset(rxbuffer, '\0', sizeof(rxbuffer));
            usart_interrupt_flag_clear(USART2, USART_INT_FLAG_RBNE);
        }
        if(rxCount == 6){
            if(!strcmp(rxbuffer, "start0")) {
                set_state(START);
                led_on_B1();
                write = 1;
            }else if(!strcmp(rxbuffer, "stopp0")){
                //set start time millis thingy
                set_state(STOP);
                led_off_B1();
                write = 0;
            }
            rxCount = 0;
            memset(rxbuffer, '\0', sizeof(rxbuffer));
            usart_interrupt_flag_clear(USART2, USART_INT_FLAG_RBNE);
        }
    }
}

void ledInit (void){
    rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);
}

void led_on_B1(){
    gpio_bit_write(GPIOB, GPIO_PIN_1, 1);
}
void led_off_B1(){
    gpio_bit_write(GPIOB, GPIO_PIN_1, 0);
}

void led_on_B0(){
    gpio_bit_write(GPIOB, GPIO_PIN_0, 1);
}
void led_off_B0(){
    gpio_bit_write(GPIOB, GPIO_PIN_0, 0);
}

void timer_interrupt_config(){
    /* This configuration has a lot in common with the PWM configuration since both relies on a timer. 
       this time though instead of generating a puls each timer period we get an interrupt. This example
       generates an interrupt each millisecond to update a counter and when an even count of 1000 (1s)
       the interrupt will toggle an LED. */

    /* Configuration structs */ 
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    /* Enable the TIMER1 interrupt request */
    eclic_irq_enable(TIMER1_IRQn,1,0);
    
    /* Enable the peripheral clock. */
    rcu_periph_clock_enable(RCU_TIMER1);

    /* Reset the timer */
    timer_deinit(TIMER1);

    /* initialize timer configuration struct */
    timer_struct_para_init(&timer_initpara);

    /* TIMER1 configuration */
    /* Frequency = core clock / ((1+prescaler)*period) = 108MHz / (2*54000) = 1KHz */
    timer_initpara.prescaler         = 1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = SystemCoreClock/2000;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER1, &timer_initpara);

    /* initialize TIMER channel output parameter struct */
    timer_channel_output_struct_para_init(&timer_ocinitpara);

    /* Set the channel configuration */
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_ocinitpara);
    
    /* CH0 configuration in OC timing mode */
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, SystemCoreClock/4000);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_TIMING);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* Enable interrupt on the channel */
    timer_interrupt_enable(TIMER1, TIMER_INT_CH0);
    /* Make sure interrupt flag is clear */
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_CH0);

    /* Start the timer */
    timer_enable(TIMER1);
}

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

void init_mpu(void){
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_I2C0);
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7); //led?

    mpu6500_install(I2C0);

    delay_1ms(100);   
}

void init_ADC_EMG(){
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* Initialize the GPIO that will be used for ADC. A0-A7 and B0-B1 are connected to an ADC-channel each. */
    gpio_init(ANALOG_PORT, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ANALOG_PIN); //A3
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);

    /* Select the clock frequency that will be used for the ADC core. Refer to README for more info on what to select. */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);

    /* Reset ADC0 configuration. GD32VF103 has two internal ADCs (ADC0, ADC1). */
    adc_deinit(ADC0);

    /* Set the ADCs to work independently. Refer to the manual for the different parallel modes available. */
    adc_mode_config(ADC_MODE_FREE);

    /* Set the conversion mode to continuous. Continious mode lets the ADC take measurements continiously without
       an external trigger. */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);

    /* Sets where padding is applied to the measurement. Data alignment right puts padding bits above MSB */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    /* Selects how many channels to convert each time. This can be used to "queue" multiple channels. Here just one channel is selected. */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);

    /* Set the channel as the first "queued" conversion each time the ADC is activated. */
    adc_regular_channel_config(ADC0, 0, ANALOG_CHANNEL, ADC_SAMPLETIME_28POINT5);

    /* Since we are using continious conversion we do not want to use an external trigger. */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

    /* Enable ADC.*/
    adc_enable(ADC0);

    /* Let ADC stabilize */
    delay_1ms(1);

    /* Calibrates the ADC against an internal source. */
    adc_calibration_enable(ADC0);

    /* Start converting */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}