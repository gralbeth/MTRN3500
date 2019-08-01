#pragma once 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <structs.h>
#include <stdint.h>

void* SMCreate(key_t key, size_t size);

