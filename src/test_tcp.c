#include "test_tcp.h"
#include "jxc_tcp.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

/**
 * test tcp lib
*/

static bool is_running = true;

//线程
static pthread_t thread_tcp_cli_id;
static pthread_t thread_tcp_single_id;
static pthread_t thread_tcp_multi_id;


static void *thread_tcp_cli(void *para)
{
    int ret = 0;
    uint8_t buf[1024] = {0};

    jxc_tcp_cfg_t cfg = {
        .mode = JXC_TCP_MODE_CLIENT,
        .server_ip = "127.0.0.1",
        .server_port = 10001,
        .connect_timeout_ms = 10000,
        .io_timeout_ms = 1000,
    };

    //创建客户端
    jxc_tcp_handle client = jxc_tcp_create(&cfg);
    if(!client){
        printf("jxc_tcp_create client failed\n");
        return NULL;
    }

    //连接到服务器,服务器必须存在，否则设置的超时时间没有，会瞬间返回报错
    ret = jxc_tcp_connect(client);
    if(ret < 0){
        printf("jxc_tcp_connect failed %d\n",ret);
        jxc_tcp_destroy(client);
        return NULL;
    }

    while(is_running)
    {   
        //这里超时读取，并在断开后，实现重连
        ret = jxc_tcp_recv(client, -1, buf, sizeof(buf));
        if(ret <= 0){
            if(ret != jxcStatusTimeout){
                if(ret == jxcStatusTcpServerAbnormal){
                        printf("client tre reconnect server\n");
                        ret = jxc_tcp_reconnect(client, 3, 1000);
                        if(ret == jxcStatusSuccess){
                            printf("reconnect successed\n");
                        }else{
                            printf("reconnect 3 times failed %d\n",ret);
                            break;
                        }
                    }else{
                        printf("jxc_tcp_recv client %d\n",ret);
                    }
            }else{
                int size = jxc_tcp_send(client, -1, "tcp client recv timeout\n", sizeof("tcp client recv timeout\n"));
                if(size < 0){
                    printf("jxc_tcp_send client %d\n",size);
                }
            }
        }else{
            printf("recv from server:%s\n",buf);
            jxc_tcp_send(client, -1, "i am tcp client\n", sizeof("i am tcp client\n"));
        }
    }

    jxc_tcp_destroy(client);
}

static void *thread_tcp_serv_single(void *para)
{
    uint8_t buf[1024] = {0};
    jxc_tcp_cfg_t cfg = {
        .mode = JXC_TCP_MODE_SERVER,
        .server_mode = JXC_TCP_SERVER_SINGLE,
        .local_ip = "127.0.0.1",
        .local_port = 19980,
        .io_timeout_ms = 1000,
    };

    jxc_tcp_handle server = jxc_tcp_create(&cfg);
    if(!server){
        printf("jxc_tcp_create server single failed\n");
        return NULL;
    }
    
    

    while(is_running)
    {
        int id = -1;
        int ret = jxc_tcp_wait(server, &id, 1000);

        if(ret == jxcStatusTimeout){
            continue;
        }
        if(ret != jxcStatusSuccess){
            continue;
        }

        ret = jxc_tcp_recv(server, id, buf, sizeof(buf));
        if(ret > 0){
            printf("tcp server single recv %d:%s\n",id,buf);
            jxc_tcp_send(server, id, "server get msg\n", sizeof("server get msg\n"));
        }else{
            if(ret == jxcStatusTcpClientAbnormal){
                jxc_tcp_close_client(server, id);
            }
            printf("jxc_tcp_recv server single falied %d\n",ret);

        }


    }

    jxc_tcp_destroy(server);
}

static void *thread_tcp_serv_multi(void *para)
{
    uint8_t buf[1024] = {0};
    jxc_tcp_cfg_t cfg = {
        .mode = JXC_TCP_MODE_SERVER,
        .server_mode = JXC_TCP_SERVER_MULTI,
        .local_ip = "127.0.0.1",
        .local_port = 19990,
        .io_timeout_ms = 1000,
    };

    jxc_tcp_handle server = jxc_tcp_create(&cfg);
    if(!server){
        printf("jxc_tcp_create server multi failed\n");
        return NULL;
    }

    while(is_running)
    {
        int id = -1;
        int ret = jxc_tcp_wait(server, &id, 1000);

        if(ret == jxcStatusTimeout){
            continue;
        }
        if(ret != jxcStatusSuccess){
            continue;
        }

        ret = jxc_tcp_recv(server, id, buf, sizeof(buf));
        if(ret > 0){
            printf("tcp server single recv %d:%s\n",id,buf);
            jxc_tcp_send(server, id, "server get msg\n", sizeof("server get msg\n"));
        }else{
            if(ret == jxcStatusTcpClientAbnormal){
                jxc_tcp_close_client(server, id);
            }
            printf("jxc_tcp_recv server single falied %d\n",ret);

        }

    }

    jxc_tcp_destroy(server);
}


void test_jxc_tcp(void)
{
    printf("--------------------TEST JXC TCP\n");

    int ret = 0;
    //客户端
    ret = pthread_create(&thread_tcp_cli_id, 0, thread_tcp_cli, NULL);
    if(ret != 0){
        printf("pthread_create thread_tcp_cli_id  failed\n");
    }
    
    //服务器-单个客户
    ret = pthread_create(&thread_tcp_single_id, 0, thread_tcp_serv_single, NULL);
    if(ret != 0){
        printf("pthread_create thread_tcp_single_id  failed\n");
    }

    //服务器-多个客户
    ret = pthread_create(&thread_tcp_multi_id, 0, thread_tcp_serv_multi, NULL);
    if(ret != 0){
        printf("pthread_create thread_tcp_multi_id  failed\n");
    }

}

void test_jxc_tcp_destory(void)
{
    is_running = false;

    pthread_join(thread_tcp_cli_id, NULL);
    pthread_join(thread_tcp_single_id, NULL);
    pthread_join(thread_tcp_multi_id, NULL);
}

