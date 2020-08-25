/*
Author: Kay
Date: 2020/8/19 4:04 下午
Usage: 
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef unsigned char u_char;

#define hash(key, c) ((uint32_t)key * 31 + c)

static uint32_t ngx_hash(u_char* data, int32_t len) {
    uint32_t key = 0;
    int i;
    for (i = 0; i < len; i++) {
        key = hash(key, data[i]);
    }
    return key;
}


int main() {
    int hash_val = ngx_hash("likai", strlen("likai"));
    printf("hash val: %d\n", hash_val);
}