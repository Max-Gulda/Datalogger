#pragma once
#include "stdlib.h"

enum state_enum{
    RUNNING,
    START,
    STOP
};

int get_state();
void set_state(int state);