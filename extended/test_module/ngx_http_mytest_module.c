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

static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r);


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

typedef struct {
    ngx_uint_t my_step;
}ngx_http_mytest_ctx_t;




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
    mycf->my_msec = NGX_CONF_UNSET_MSEC;
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
            NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
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

/*
自定义配置项处理方法：
带参数的处理方式
1. 声明一个结构体存储参数
```
typedef struct {
    ngx_str_t my_config_str;
    ngx_int_t my_config_num;
}ngx_http_mytest_conf_t;
```
2. 定义set函数指针
```
static char* ngx_conf_set_myconfig(ngx_conf_t cf, ngx_command_t cmd, void* conf);
```

3. 定义ngx_command_t结构体
```
static ngx_command_t ngx_http_mytest_commands[] = {
    {
        ngx_string("test_myconfig"),
        NGX_HTTP_CONF|NGX_CONF_TAKE12,
        ngx_conf_set_myconfig,
        0,
        NULL
    },
    ngx_null_command
};
```

4. 处理参数
```
static char* ngx_conf_set_myconfig(ngx_conf_t cf, ngx_command_t cmd, void* conf) {
    ngx_http_mytest_conf_t myconf = conf;
    ngx_str_t value = cf->args->elts;

    if (cf->args->nelts > 1) {
        mycf->my_config_str = value[1];
    }
    if (cf->args->nelts > 2) {
        mycf->my_config_num = ngx_atoi(value[2].data, value[2].len);
        if (mycf->my_config_num == NGX_ERROR) {
            return "invalid number";
        }
    }
    return NGX_CONF_OK;
}
```



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

typedef struct {
    u_char* str;
    ngx_queue_t q_ele;
    int num;
} test_node;

ngx_int_t com_test_node(const ngx_queue_t* a, const ngx_queue_t* b) {
    test_node* a_node = ngx_queue_data(a, test_node, q_ele);
    test_node* b_node = ngx_queue_data(b, test_node, q_ele);
    return a_node->num > b_node->num;
}

static inline test_queue() {
    // 必须要初始化
    ngx_queue_t queue_container;
    ngx_queue_init(&queue_container);

    int i = 0;
    test_node node[5];
    for (; i < 5; i++) {
        node[i].num = i;
    }

    // 3 1 0 2 4
    ngx_queue_insert_tail(&queue_container, &node[0].q_ele);
    ngx_queue_insert_head(&queue_container, &node[1].q_ele);
    ngx_queue_insert_tail(&queue_container, &node[2].q_ele);
    ngx_queue_insert_after(&queue_container, &node[3].q_ele);
    ngx_queue_insert_tail(&queue_container, &node[4].q_ele);

    ngx_queue_t *q;
    for (q = ngx_queue_head(&queue_container); q != ngx_queue_sentinel(&queue_container);
        q = ngx_queue_next(q))
    {
        test_node* ele_node = ngx_queue_data(q, test_node, q_ele);
        printf("num: %d\n", ele_node->num);
    }

}


static inline test_array(ngx_http_request_t *r) {
    ngx_array_t* dy_array = ngx_array_create(r->pool, 1, sizeof(test_node));
    test_node* a =  ngx_array_push(dy_array);
    a->num = 1;
    a = ngx_array_push(dy_array);
    a->num = 2;
    test_node* b = ngx_array_push_n(dy_array, 3);
    b->num = 3;
    (b+1)->num = 4;
    (b+2)->num = 5;

    test_node* node_arr = dy_array->elts;
    ngx_uint_t arr_seq = 0;
    for (; arr_seq < dy_array->nelts; arr_seq++) {
        a = node_arr + arr_seq;
        printf("num=%d\n", a->num);
    }

    ngx_array_destroy(dy_array);
}

static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r) {
    if ( !(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    test_queue();
    test_array(r);

    // 获取上下文
    ngx_http_mytest_ctx_t* myctx = ngx_http_get_module_ctx(r, ngx_http_mytest_module);
    if (myctx == NULL) {
        myctx = ngx_palloc(r->pool, sizeof(ngx_http_mytest_ctx_t));
        if (myctx == NULL) {
            return NGX_ERROR;
        }
        ngx_http_set_ctx(r, myctx, ngx_http_mytest_module);
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