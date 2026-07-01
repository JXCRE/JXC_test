#ifndef __JXC_QUEUE_H
#define __JXC_QUEUE_H

/**
 * QUEUE:
 *  
 * SUGGESTION:
 *  多线程使用时，不建议在使用过程中destory
 * DEFECT:
 *  读写被虚假唤醒后，且条件不满足，会重新计算超时时间，会导致总等待时间会超过设定的等待时间
 *  使用pthread_condattr_setclock，可能在某些精简的平台不适配
 * 
*/

#include <stdint.h>
#include <jxc_type.h>

typedef void* jxc_queue_handle;

#ifdef __cplusplus
extern "C" {
#endif

jxc_queue_handle jxc_queue_create(uint32_t max_num, uint32_t elem_size);
void jxc_queue_destroy(jxc_queue_handle handle);
jxc_status jxc_queue_enqueue(jxc_queue_handle handle, uint8_t *data, int timeout_ms);
jxc_status jxc_queue_dequeue(jxc_queue_handle handle, uint8_t *data, int timeout_ms);
//jxc_queue_get_latest会获取最新的一个数据，并丢弃所有旧数据(直接清空队列)
jxc_status jxc_queue_get_latest(jxc_queue_handle handle, uint8_t *data, int timeout_ms);
int jxc_queue_get_cur_num(jxc_queue_handle handle);
jxc_status jxc_queue_clear(jxc_queue_handle handle);

#ifdef __cplusplus
}
#endif


#endif
