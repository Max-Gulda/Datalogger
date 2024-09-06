#pragma once

#include "gd32vf103.h"

void init_radio();
size_t read_radio(char* buffer, uint32_t size);
size_t send_radio(char* message, uint32_t size);
void init_radio_gaga();