#include "gd32vf103.h"
#include "radioHead.h"
#include "systick.h"


char rx_radio_buffer[1024] = {'\0'};
char tx_radio_buffer[1024] = {'\0'};

void init_radio(){
    dma_parameter_struct dma_init_struct;
    /* enable DMA0 */
    rcu_periph_clock_enable(RCU_DMA0);
    /* initialize USART */
    rcu_periph_clock_enable(RCU_GPIOB);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART2);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10); //PP for no half duplex

    /* connect port to USARTx_Rx */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    /* USART configure */
    usart_deinit(USART2);
    usart_baudrate_set(USART2, 1200);
    usart_word_length_set(USART2, USART_WL_8BIT);
    usart_stop_bit_set(USART2, USART_STB_1BIT);
    usart_parity_config(USART2, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);

    eclic_global_interrupt_enable();
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL3_PRIO1);
    eclic_irq_enable(USART2_IRQn, 1, 0);

    usart_interrupt_enable(USART2, USART_INT_RBNE); 

    usart_enable(USART2);

//     usart_dma_transmit_config(USART2, USART_DENT_ENABLE);
//    usart_dma_receive_config(USART2, USART_DENR_ENABLE);
//
//    dma_deinit(DMA0, DMA_CH2);
//    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
//    dma_init_struct.memory_addr = (uint32_t)rx_radio_buffer;
//    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
//    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
//    dma_init_struct.number = 1024;
//    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART2);
//    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
//    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
//    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
//    dma_init(DMA0, DMA_CH2, &dma_init_struct);
//        /* configure DMA mode */
//    dma_circulation_disable(DMA0, DMA_CH2);
//        /* enable DMA channel4 */
//    dma_channel_enable(DMA0, DMA_CH2); 

}

size_t read_radio(char* buffer, uint32_t size){
    if(rx_radio_buffer[0] != 0){
        int i = 0;
        for(; i < 1024 && rx_radio_buffer[i] != 0 && i < size - 1; i++){
            /* buffer[i] = rx_radio_buffer[i];
            rx_radio_buffer[i] = 0; */
        }
        buffer[i] = '\0';
    



        dma_parameter_struct dma_init_struct;

        dma_deinit(DMA0, DMA_CH2);
        dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
        dma_init_struct.memory_addr = (uint32_t)rx_radio_buffer;
        dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
        dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
        dma_init_struct.number = 1024;
        dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART2);
        dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
        dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
        dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
        dma_init(DMA0, DMA_CH2, &dma_init_struct);
        dma_circulation_disable(DMA0, DMA_CH2);
        dma_channel_enable(DMA0, DMA_CH2);
        return i;
    }
    return 0;
}

size_t send_radio(char* message, uint32_t size){
    for(int i = 0; i < size && i < 1024; i++) {
        tx_radio_buffer[i] = message[i];
    }

    dma_parameter_struct dma_init_struct;
    dma_deinit(DMA0, DMA_CH1);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)tx_radio_buffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = size;
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART2);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH1, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH1);
    /* enable DMA channel3 */
    dma_channel_enable(DMA0, DMA_CH1);
}

void init_radio_gaga(){
    USART_GP(USART2)    |= 0x01;
    USART_CTL2(USART2)  &= ~(0x04);
    USART_CTL2(USART2)  |= 0x02; 
    USART_CTL1(USART2)  &= ~(0x7800);
    USART_CTL2(USART2)  &= ~(0x28);
}