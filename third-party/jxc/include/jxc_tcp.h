#ifndef __JXC_TCP_H
#define __JXC_TCP_H

/**
 * TCP:
 *  支持客户端和服务器模式
 *  服务器支持单客户模式和多客户epoll模式
 * SUGGESTION:
 *  不支持多线程同时使用同一个handle
 *  客户端模式下client_id参数会被忽略
 * DEFECT:
 *  多客户模式一次wait只返回一个就绪连接
 */

#include <stdint.h>
#include <jxc_type.h>

typedef void* jxc_tcp_handle;

typedef enum {
    JXC_TCP_MODE_SERVER = 0,
    JXC_TCP_MODE_CLIENT = 1,
}jxc_tcp_mode_t;

typedef enum {
    JXC_TCP_SERVER_SINGLE = 0,
    JXC_TCP_SERVER_MULTI  = 1,
}jxc_tcp_server_mode_t;

typedef struct{
    jxc_tcp_mode_t        mode;                 //客户端/服务器
    jxc_tcp_server_mode_t server_mode;          //服务器单客户/多客户
    char                 *local_ip;             //服务器绑定IP(NULL=INADDR_ANY)
    uint16_t              local_port;           //服务器绑定端口
    char                 *server_ip;            //客户端连接的服务器IP
    uint16_t              server_port;          //客户端连接的服务器端口
    int                   connect_timeout_ms;   //客户端连接超时(-1=阻塞)
    int                   io_timeout_ms;        //收发超时(-1=阻塞)
    int                   backlog;              //listen backlog(<=0使用默认值)
    int                   max_clients;          //多客户最大数量(<=0使用默认值)
}jxc_tcp_cfg_t;

#ifdef __cplusplus
extern "C" {
#endif

jxc_tcp_handle jxc_tcp_create(jxc_tcp_cfg_t *cfg);
void jxc_tcp_destroy(jxc_tcp_handle handle);
jxc_status jxc_tcp_connect(jxc_tcp_handle handle);
jxc_status jxc_tcp_reconnect(jxc_tcp_handle handle, int retry_times, int interval_ms);
int jxc_tcp_accept(jxc_tcp_handle handle, int timeout_ms);
int jxc_tcp_wait(jxc_tcp_handle handle, int *client_id, int timeout_ms);
int jxc_tcp_send(jxc_tcp_handle handle, int client_id, uint8_t *data, uint32_t len);
int jxc_tcp_recv(jxc_tcp_handle handle, int client_id, uint8_t *buf, uint32_t buf_len);
jxc_status jxc_tcp_close_client(jxc_tcp_handle handle, int client_id);

#ifdef __cplusplus
}
#endif

#endif
