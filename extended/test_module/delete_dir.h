#include <ngx_config.h>
#include <ngx_conf_file.h>
#include <ngx_core.h>
#include <nginx.h>
#include <ngx_http.h>
#include <stdio.h>



/*test delete dir 只能删除空文件夹 */
static ngx_int_t
my_delete_dir(ngx_str_t* path) {
    ngx_dir_t dir;
    u_char          *p;
    u_char           spath[NGX_MAX_PATH + 1];

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
    ngx_delete_dir(path->data);
   
    printf("return ok\n");
    return NGX_OK;
}



static void
my_delete_dir_ev(ngx_event_t *ev) {
    ngx_str_t *path = ev->data;
    my_delete_dir(path);
}


static ngx_int_t
remove_timer(ngx_cycle_t *cycle, ngx_str_t path, ngx_uint_t ms) {
    static ngx_event_t ev;
    static ngx_str_t p2;
    p2.data = path.data;
    p2.len = path.len;
    ngx_memzero(&ev, sizeof(ngx_event_t));
    ev.handler = my_delete_dir_ev;
    ev.data = &p2;
    ngx_add_timer(&ev, ms);
    return NGX_OK;

}