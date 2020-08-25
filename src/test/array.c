/*
Author: Kay
Date: 2020/8/18 11:58 上午
Usage: 
*/

#include "public.h"
#include <time.h>
#include <unistd.h>

typedef struct {
    int    age;
    int    grade;
}my_data;


void test_array()
{
    ngx_pool_t *pool = ngx_create_pool(1024, NULL);
    ngx_array_t *arr = ngx_array_create(pool, 100 * 1024 * 1024, sizeof(my_data));
    int i;
    my_data *m;
    for (i = 0; i < 50 * 1024 * 1024; i++) {
        m = (my_data*)ngx_array_push(arr);
        m->age = 10;
        m->grade = i+20;
    }

    for (i = 0; i < arr->nelts; ++i) {
//        printf("grade: %d\n", ((my_data*)((u_char*)arr->elts + (i * arr->size)))->grade);
    }
    printf("nelts: %d\n", arr->nelts);
    printf("usleep:\n");
    usleep(1000*1000 * 10);
    for (i = 0; i < 50 * 1024 * 1024; i++) {
        m = (my_data*)ngx_array_push(arr);
        m->age = 10;
        m->grade = i+50;
    }

    printf("==============\n");
    for (i = 0; i < arr->nelts; ++i) {
//        printf("grade: %d\n", ((my_data*)((u_char*)arr->elts + (i * arr->size)))->grade);
    }
    printf("nelts: %d\n", arr->nelts);
    ngx_array_destroy(arr);
    ngx_array_init(arr, pool, 10, sizeof(my_data));
    usleep(1000*1000*10);
    printf("nelts: %d\n", arr->nelts);
    for (i = 0; i < 50 * 1024 * 1024; i++) {
        m = (my_data*)ngx_array_push(arr);
        m->age = 10;
        m->grade = i+50;
    }
    usleep(1000*1000*10);
    ngx_destroy_pool(pool);
    usleep(1000*1000*10);
}


int main()
{
    test_array();
    return 0;
}