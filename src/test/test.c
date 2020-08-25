/*
Author: Kay
Date: 2020/8/17 6:22 下午
Usage: 
*/

#include "ngx_config.h"
#include <stdio.h>
#include "ngx_conf_file.h"
#include "nginx.h"
#include "ngx_core.h"
#include "ngx_string.h"
#include "ngx_palloc.h"
#include "ngx_array.h"
#include "ngx_queue.h"

volatile ngx_cycle_t  *ngx_cycle;

void ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
                        const char *fmt, ...)
{
}

typedef struct test_queue_data {
    int                 data;
    ngx_queue_t         queue;
}test_q_data;


void test_queue()
{
    /*
     *  gcc src/test/test.c src/core/ngx_array.c src/os/unix/ngx_alloc.c  src/core/ngx_list.c  src/core/ngx_palloc.c  -I src/os/unix/ -I src/core/ -I objs/

     * */

    ngx_queue_t h;

    ngx_queue_init(&h);

    test_q_data a;
    a.data = 10;
    ngx_queue_insert_tail(&h, &a.queue);

    ngx_queue_t *data_q = ngx_queue_last(&h);
    test_q_data* ret = ngx_queue_data(data_q, test_q_data, queue);
    printf("ret data: %d\n", ret->data);
}


int main()
{
    test_queue();
    return 0;
}

