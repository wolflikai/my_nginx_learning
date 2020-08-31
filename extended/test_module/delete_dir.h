#include <ngx_config.h>
#include <ngx_conf_file.h>
#include <ngx_core.h>
#include <nginx.h>
#include <ngx_http.h>
#include <stdio.h>



/*test delete dir 只能删除空文件夹 */
static ngx_int_t
my_delete_dir(ngx_str_t* path) {
    ngx_dir_t       dir;
    u_char          *p;
    u_char           spath[NGX_MAX_PATH + 1];

    printf("path: %s len=%d\n", path->data, (int)path->len);
    if (ngx_open_dir(path, &dir) != NGX_OK) {
        printf("ngx open dir error\n");
        return NGX_ERROR;
    }

    ngx_err_t err;
    // 读出所有的文件
    ngx_str_t name;
    for(;;) {
        ngx_set_errno(0);
        if (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;
            if (err == NGX_ENOMOREFILES) {
                goto done;
            }
        }
        if (!ngx_de_is_file(&dir)) {
            continue;
        }
        name.data = ngx_de_name(&dir);
        if (name.data[0] == '.') {
            continue;
        }
        name.len = ngx_de_namelen(&dir);
        printf("name: %s\n", name.data);

        p = ngx_snprintf(spath, sizeof(spath) - 1, "%V/%V", path, &name);
        *p = 0;
        
        if (ngx_delete_file(spath) == NGX_FILE_ERROR) {
            printf("delete file error\n");
        }
    }

done:
    // try to delete dir
    if (ngx_delete_dir(path->data) == NGX_ERROR) {
        printf("delete dir error\n");
    }
   
    printf("return ok\n");
    return NGX_OK;
}



static void
my_delete_dir_ev(ngx_event_t *ev) {
    ngx_str_t *path = ev->data;
    my_delete_dir(path);
}


void 
test_mem(ngx_event_t* ev) {
    ngx_str_t *p = ev->data;
    ngx_free(p->data);
    ngx_free(p);
}


static ngx_int_t
remove_timer(ngx_cycle_t *cycle, ngx_str_t path, ngx_uint_t ms) {
    // static ngx_event_t ev;
    // static ngx_str_t p2;
    // p2.data = path.data;
    // p2.len = path.len;
    // ngx_memzero(&ev, sizeof(ngx_event_t));
    // ev.handler = my_delete_dir_ev;
    // ev.data = &p2;
    // ngx_add_timer(&ev, ms);
    ngx_event_t* ev = ngx_calloc(sizeof(ngx_event_t), cycle->log);
    const char hello[1024*1024*5];
    ngx_str_t* p = ngx_calloc(sizeof(ngx_str_t), cycle->log);
    printf("remove timer\n");
    p->data = ngx_calloc(5 * 1024 * 1024, cycle->log);

    *ngx_cpymem(p->data, hello, 5 * 1024 * 1024) = 0;
    p->len = 5 * 1024 * 1024;

    ev->handler = test_mem;
    ev->data = p;
    ngx_add_timer(ev, 20000);

    return NGX_OK;

}