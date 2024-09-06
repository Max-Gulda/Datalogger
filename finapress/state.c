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