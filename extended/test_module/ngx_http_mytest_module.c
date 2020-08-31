/*
Author: Kay
Date: 2020/8/18 6:48 下午
Usage: 
*/



/*
module_s 调用顺序：
1. create_main_conf
2. create_srv_conf
3. create_loc_conf
4. precofiguration
5. init_main_conf
6. merge_srv_conf
7. merge_loc_conf
8. postconfiguration

*/


#include "delete_dir.h"


static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r) {
    if ( !(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }
    ngx_int_t rc = ngx_http_discard_request_body(r);
    if (rc != NGX_OK) {
        return rc;
    }

    ngx_str_t type = ngx_string("text/plain");
    ngx_str_t res = ngx_string("Hello World");
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = res.len;
    r->headers_out.content_type = type;

    rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }
    // 临时的内存 把数据写进去
    // ngx_buf_t *b;
    // b = ngx_create_temp_buf(r->pool, res.len);
    // if (b == NULL) {
    //     return NGX_HTTP_INTERNAL_SERVER_ERROR;
    // }
    // ngx_memcpy(b->pos, res.data, res.len);
    // b->last = b->pos + res.len;
    // b->last_buf = 1;
    

    ngx_buf_t *b;
    b = ngx_palloc(r->pool, sizeof(ngx_buf_t));
    u_char* filename = "/home/reading/reading/extended/test_module/hello.txt";
    b->in_file = 1;
    b->file = ngx_palloc(r->pool, sizeof(ngx_file_t));
    b->file->fd = ngx_open_file(filename, NGX_FILE_RDONLY, NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);
    b->file->name.len = strlen(filename);
    if (b->file->fd <= 0) {
        return NGX_HTTP_NOT_FOUND;
    }
    if (ngx_file_info(filename, &b->file->info) == NGX_FILE_ERROR) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    r->headers_out.content_length_n = b->file->info.st_size;
    b->file_pos = 0;
    b->file_last = b->file->info.st_size;
    printf("\n");
    printf("st size: %u\n", b->file_last);
    // close the file
    ngx_pool_cleanup_t* cln = ngx_pool_cleanup_add(r->pool, sizeof(ngx_pool_cleanup_file_t));
    if (cln == NULL) {
        return NGX_ERROR;
    }
    cln->handler = ngx_pool_cleanup_file;
    ngx_pool_cleanup_file_t* clnf = cln->data;
    clnf->fd = b->file->fd;
    clnf->name = b->file->name.data;
    clnf->log = r->pool->log;
    
    // ngx_str_t path = ngx_string("/dev/shm/fmp4/2");
    // remove_timer((ngx_cycle_t *)ngx_cycle, path, 5000);

    ngx_chain_t out;
    out.buf = b;
    out.next = NULL;

    r->allow_ranges = 1;
    return ngx_http_output_filter(r, &out);
}


static char*
ngx_http_mytest(ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_mytest_handler;   // NGX_HTTP_CONTENT_PHASE阶段就会调用咱们的函数
    return NGX_OK;
}


static ngx_command_t ngx_http_mytest_commands[] = {
        {
            ngx_string("mytest"),
            NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF  | NGX_CONF_NOARGS,
            ngx_http_mytest,  // char* (*set) (ngx_conf_t*, ngx_command_t*, void*)
            NGX_HTTP_LOC_CONF_OFFSET,
            0,
            NULL,
        },
        ngx_null_command
};

static ngx_http_module_t ngx_http_mytest_module_ctx = {
    NULL,  // pre configuration
    NULL,  // post configuration
    NULL,  // create main configuration
    NULL,  // init main configuration
    NULL,  // create server configuration
    NULL,  // merge server configuration
    NULL,  // create location configuration
    NULL  // merge location configuration
};

ngx_module_t ngx_http_mytest_module = {
    NGX_MODULE_V1,
    &ngx_http_mytest_module_ctx,
    ngx_http_mytest_commands,
    NGX_HTTP_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    
    NGX_MODULE_V1_PADDING
};