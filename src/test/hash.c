/*
Author: Kay
Date: 2020/8/19 2:53 下午
Usage: 
*/


#include "public.h"
#include <ngx_hash.h>


typedef struct{
    ngx_str_t servername;
    ngx_int_t seq;
}TestWildcardHashNode;


static ngx_int_t test_hash()
{
    ngx_pool_t* p = ngx_create_pool(NGX_DEFAULT_POOL_SIZE, NULL);
    ngx_hash_init_t hash;
    ngx_hash_keys_arrays_t ha;

    int i = 0;

    ngx_memzero(&ha, sizeof(ngx_hash_keys_arrays_t));
    ha.temp_pool = ngx_create_pool(NGX_DEFAULT_POOL_SIZE,NULL);
    ha.temp_pool = p;

    ngx_hash_keys_array_init(&ha, NGX_HASH_LARGE);


    TestWildcardHashNode testnode[3];
    testnode[0].servername.data = ngx_pcalloc(p, ngx_strlen("likai1"));
    testnode[0].servername.len = strlen("likai1");
    ngx_memcpy(testnode[0].servername.data, "likai1", testnode[0].servername.len);

    testnode[1].servername.data = ngx_pcalloc(p, ngx_strlen("likai2"));
    testnode[1].servername.len = strlen("likai2");
    ngx_memcpy(testnode[1].servername.data, "likai2", testnode[1].servername.len);

    testnode[2].servername.data = ngx_pcalloc(p, ngx_strlen("likai3"));
    testnode[2].servername.len = strlen("likai3");
    ngx_memcpy(testnode[2].servername.data, "likai3", testnode[2].servername.len);

    for(i = 0; i<3; i++)
    {
        testnode[i].seq = i;
        ngx_hash_add_key(&ha, &testnode[i].servername, &testnode[i], NGX_HASH_READONLY_KEY); //NGX_HASH_WILDCARD_KEY表明可以处理带通配符的关键字
    }

    ngx_cacheline_size=32;//必须包含这句否则会段错误
    hash.key = ngx_hash_key_lc;
    hash.max_size = 100;
    hash.bucket_size = 48;
    hash.name = "my_hash_test";
    hash.pool = p;

    ngx_hash_t my_hash;

    if(ha.keys.nelts)
    {
        hash.hash = &my_hash;
        hash.temp_pool = NULL;
        if(ngx_hash_init(&hash, ha.keys.elts, ha.keys.nelts) != NGX_OK)
            return NGX_ERROR;
    }


    ngx_destroy_pool(ha.temp_pool);

    ngx_str_t findServer;
    findServer.data=ngx_pcalloc(p,ngx_strlen("likai3"));
    findServer.len=ngx_strlen("likai3");
    ngx_memcpy(findServer.data,"likai3",ngx_strlen("likai3"));

    ngx_uint_t hashkey=ngx_hash_key_lc(findServer.data,findServer.len);
    printf("hash_key %ld\r\n",hashkey);

    ngx_strlow(findServer.data,findServer.data,findServer.len);

    TestWildcardHashNode*res=ngx_hash_find(&my_hash,
                                                    hashkey,
                                                    findServer.data,
                                                    findServer.len);
    if(NULL == res)
    {
        printf("not find\r\n");
    }
    else
    {
        printf("servername: %s ,seq: %d\r\n",res->servername.data,res->seq);
    }


}


int main() {
    test_hash();
    return 0;
}
