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




typedef struct {
    ngx_str_t           my_str;
    ngx_int_t           my_num;
    ngx_flag_t          my_flag;
    size_t              my_size;
    ngx_array_t*        my_str_array;
    ngx_array_t*        my_keyval;
    off_t               my_off;
    ngx_msec_t          my_msec;
    time_t              my_sec;
    ngx_bufs_t          my_bufs;
    ngx_uint_t          my_enum_seq;
    ngx_uint_t          my_bitmask;
    ngx_uint_t          my_access;
    ngx_path_t*         my_path;
}ngx_http_mytest_conf_t;

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
    ngx_buf_t *b;
    b = ngx_create_temp_buf(r->pool, res.len);
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    ngx_memcpy(b->pos, res.data, res.len);
    b->last = b->pos + res.len;
    b->last_buf = 1;
    

    // ngx_buf_t *b;
    // b = ngx_palloc(r->pool, sizeof(ngx_buf_t));
    // u_char* filename = "/home/reading/reading/extended/test_module/hello.txt";
    // b->in_file = 1;
    // b->file = ngx_palloc(r->pool, sizeof(ngx_file_t));
    // b->file->fd = ngx_open_file(filename, NGX_FILE_RDONLY, NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);
    // b->file->name.len = strlen(filename);
    // if (b->file->fd <= 0) {
    //     return NGX_HTTP_NOT_FOUND;
    // }
    // if (ngx_file_info(filename, &b->file->info) == NGX_FILE_ERROR) {
    //     return NGX_HTTP_INTERNAL_SERVER_ERROR;
    // }
    // r->headers_out.content_length_n = b->file->info.st_size;
    // b->file_pos = 0;
    // b->file_last = b->file->info.st_size;
    // printf("\n");
    // printf("st size: %u\n", b->file_last);
    // // close the file
    // ngx_pool_cleanup_t* cln = ngx_pool_cleanup_add(r->pool, sizeof(ngx_pool_cleanup_file_t));
    // if (cln == NULL) {
    //     return NGX_ERROR;
    // }
    // cln->handler = ngx_pool_cleanup_file;
    // ngx_pool_cleanup_file_t* clnf = cln->data;
    // clnf->fd = b->file->fd;
    // clnf->name = b->file->name.data;
    // clnf->log = r->pool->log;
    
    // ngx_str_t path = ngx_string("/dev/shm/fmp4/2");
    // remove_timer((ngx_cycle_t *)ngx_cycle, path, 5000);

    ngx_chain_t out;
    out.buf = b;
    out.next = NULL;

    r->allow_ranges = 1;
    return ngx_http_output_filter(r, &out);
}


static void* ngx_http_mytest_create_loc_conf(ngx_conf_t* cf) {
    ngx_http_mytest_conf_t* mycf;
    mycf = (ngx_http_mytest_conf_t*) ngx_palloc(cf->pool, sizeof(ngx_http_mytest_conf_t));
    if (mycf == NULL) {
        return NULL;
    }
    mycf->my_flag = NGX_CONF_UNSET;
    mycf->my_num = NGX_CONF_UNSET;
    mycf->my_str_array = NGX_CONF_UNSET_PTR;
    mycf->my_keyval = NULL;
    mycf->my_off = NGX_CONF_UNSET;
    mycf->my_msec = NGX_CONF_UNSET_MSET;
    mycf->my_sec = NGX_CONF_UNSET;
    mycf->my_size = NGX_CONF_UNSET_SIZE;
    return mycf;
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
            NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
            ngx_http_mytest,  // char* (*set) (ngx_conf_t*, ngx_command_t*, void*)
            NGX_HTTP_LOC_CONF_OFFSET,  // 指示配置项所处内存相对偏移位置
            0,
            NULL,  
        },
        ngx_null_command
};

// 对于配置项的的解析设置的回调函数 nginx有预设的14个方法 跟上面配置选项的14个属性一一对应

/*
    ngx_conf_set_flag_slot   设置ngx_flag_t 为1或0
    ngx_conf_set_str_slot    str 只有一个参数 
    ngx_conf_set_str_array_slot  这个配置项多次出现 我们希望用一个数组cover
    ngx_conf_set_keyval_slot  配置参数为2个 kay val 这种
    ngx_conf_set_num_slot     携带一个参数 只能是数字 必须是整形
    ngx_conf_set_size_slot    设置size k/K  m/M  KB MB 这种 解析出来会自动转换为B
    ngx_conf_set_off_slot      kK mM gG 同上
    ngx_conf_set_msec_slot     m s h d w M  y 这是分秒时年月日 最终会转换成毫秒
    ngx_conf_set_sec_slot      同上 但是会转换为秒
    ngx_conf_set_bufs_slot     两个参数 4 8k  指的是多少个ngx_buf_t缓冲区  第二个可以带单位 这会被解析为
                                ngx_bufs_t 结构体的两个成员
    ngx_conf_set_enum_slot      
    ngx_conf_set_bitmask_slot
    ngx_conf_set_access_slot    1-3个参数 设置文件权限 user:rw group:rw all:rw
    ngx_conf_set_path_slot      1个参数 最终转换为ngx_path_t

*/

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