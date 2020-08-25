/*
Author: Kay
Date: 2020/8/18 11:59 上午
Usage: 
*/


#ifndef NGINX_PUBLIC_H
#define NGINX_PUBLIC_H

#include <ngx_config.h>
#include <stdio.h>
#include <ngx_conf_file.h>
#include <nginx.h>
#include <ngx_core.h>
#include <ngx_string.h>
#include <ngx_palloc.h>
#include <ngx_array.h>
#include <ngx_queue.h>
#include <ngx_list.h>


volatile ngx_cycle_t  *ngx_cycle;

void ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
                        const char *fmt, ...)
{
}


#endif //NGINX_PUBLIC_H
