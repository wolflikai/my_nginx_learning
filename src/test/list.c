/*
Author: Kay
Date: 2020/8/18 4:23 下午
Usage: 
*/


#include "public.h"

typedef struct {
    int data1;
    int data2;
}my_data;

int main() {

    ngx_pool_t *pool = ngx_create_pool(1024, NULL);
    ngx_list_t* list = ngx_list_create(pool, 10, sizeof(my_data));
    int i;
    for (i = 0; i < 11; i++) {
        my_data *data = ngx_list_push(list);
        data->data1 = i + 10;
        data->data2 = i + 20;
    }
    printf("list: %0x\n", list);
    printf("list last: %x\n", list->last);
    printf("list part: %x\n", list->part);
    printf("list size: %d\n", list->size);
    printf("list nalloc: %d\n", list->nalloc);
    printf("list pool: %x\n", list->pool);
    printf("pool: %x\n", pool);

    // how to get data
    ngx_list_part_t* part = &list->part;
    while (part) {
        for (i = 0; i < part->nelts; ++i) {
            my_data* r = ( my_data*)(part->elts + (i * list->size));
            printf("\n");
            printf("data1: %d\n", r->data1);
            printf("data2: %d\n", r->data2);
        }
        part = part->next;
    }
    ngx_destroy_pool(pool);


}
