#include <stdlib.h>
#include <stdio.h>
#include "state.h"

static int current_state = RUNNING;

int get_state(){
    return current_state;
};

void set_state(int state){
    current_state = state;
}

/* int main(void){
    while (1)
    {
        switch (current_state){
            case START:
                printf("START STATE IS: %d\n", get_state());
                set_state(RUNNING);     
                break;
            case STOP:
                printf("STOP STATE IS: %d\n", get_state());
                set_state(RUNNING);     
                break;
            case RUNNING:
                printf("%d\n", get_state());
                printf("What state next?\n");
                scanf(" %d", &current_state);       
                break;
            default:
                break;
        }
    }
} */