/*
Author: Kay
Date: 2020/8/18 3:08 下午
Usage: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef unsigned char u_char;

typedef struct link {
    link* next;
}link;


typedef struct node_data {
    int      data1;
    int      data2;
    link     lk;
}node_data;



int main() {

}


