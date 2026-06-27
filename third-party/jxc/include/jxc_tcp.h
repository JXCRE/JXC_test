// #ifndef __JXC_TCP_H
// #define __JXC_TCP_H

// /**
//  * 
// */

// #include <stdint.h>
// #include <stdbool.h>
// #include <jxc_type.h>

// typedef void* jxc_tcp_handle;

// //模式配置
// typedef enum {
//     JXC_TCP_MODE_SERVER,                //服务器模式
//     JXC_TCP_MODE_CLIENT                 //客户端模式
// }jxc_tcp_mode_t;

// //多客户端策略
// typedef enum {
//     JXC_TCP_CLIENT_POLICY_MULTI,        //多客户端模式(支持多个并发)
//     JXC_TCP_CLIENT_POLICY_SINGLE,       //单客户端模式(只保留一个连接)
//     JXC_TCP_CLIENT_POLICY_CALLBACK      //回调模式(由用户决定保留哪个)
// }jxc_tcp_client_policy_t;

// //连接保留决策(用于回调模式)
// typedef enum {
//     JXC_TCP_KEEP_NEW,                   //保留新连接，断开老连接
//     JXC_TCP_KEEP_OLD,                   //保留老连接，拒绝新连接
//     JXC_TCP_KEEP_BOTH                   //同时保留(自动转为多客户端模式)
// }jxc_tcp_keep_decision_t;

// typedef struct{
//     jxc_tcp_mode_t mode;                        //模式
//     jxc_tcp_client_policy_t client_policy;      //客户端策略
//     const char* address;                        //IP地址(服务器监听地址或客户端目标地址)
//     uint16_t port;                              //端口
//     uint16_t* multi_ports;                      //多端口数组(服务器用)
//     int port_count;                             //端口数量
//     tcp_port_handler_t port_handler;            //端口处理回调
//     tcp_decision_callback_t decision_cb;        //连接决策回调
//     tcp_data_callback_t data_cb;                //数据接收回调
//     tcp_connect_callback_t connect_cb;          //连接事件回调
//     void* user_data;                            //用户数据
//     int recv_buffer_size;                       //接收缓冲区大小(默认8192)
//     int send_buffer_size;                       //发送缓冲区大小(默认8192)
//     bool non_blocking;                          //非阻塞模式(默认true)
// }jxc_tcp_cfg_t;

// //端口处理回调(用于多客户端模式下对不同端口的处理)
// typedef void (*tcp_port_handler_t)(void* handle, int client_fd, 
//                                      uint16_t port, const char* data, 
//                                      size_t len, void* user_data);

// //连接决策回调(当单客户端模式下有新连接时调用)
// typedef jxc_tcp_keep_decision_t (*tcp_decision_callback_t)(void* handle, 
//                                         int new_fd, uint16_t new_port,
//                                         int old_fd, uint16_t old_port,
//                                         void* user_data);

// //数据接收回调
// typedef void (*tcp_data_callback_t)(void* handle, int client_id, 
//                                      const char* data, size_t len, 
//                                      void* user_data);

// //连接事件回调
// typedef void (*tcp_connect_callback_t)(void* handle, int client_id, 
//                                         uint16_t port, bool is_connected,
//                                         void* user_data);

// #ifdef __cplusplus
// extern "C" {
// #endif

// jxc_tcp_handle jxc_tcp_create(jxc_tcp_cfg_t *cfg);
// void jxc_tcp_destroy(jxc_tcp_handle handle);

// // 初始化TCP库
// jxc_tcp_handle* tcp_init(const jxc_tcp_cfg_t* config);
// // 销毁TCP库
// void tcp_destroy(jxc_tcp_cfg_t* handle);
// // 发送数据
// int tcp_send(jxc_tcp_cfg_t* handle, int client_id, const char* data, size_t len);
// // 广播数据（多客户端模式）
// int tcp_broadcast(jxc_tcp_cfg_t* handle, const char* data, size_t len);
// // 主动断开连接
// int tcp_disconnect(jxc_tcp_cfg_t* handle, int client_id);
// // 获取客户端数量
// int tcp_get_client_count(jxc_tcp_cfg_t* handle);
// // 运行事件循环（阻塞）
// int tcp_run(jxc_tcp_cfg_t* handle);
// // 停止事件循环
// void tcp_stop(jxc_tcp_cfg_t* handle);
// // 设置非阻塞发送
// int tcp_send_nonblock(jxc_tcp_cfg_t* handle, int client_id, const char* data, size_t len);

// #ifdef __cplusplus
// }
// #endif



// #endif
