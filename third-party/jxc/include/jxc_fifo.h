#ifndef __JXC_FIFO_H
#define __JXC_FIFO_H

/**
 * FIFO:
 *  
 * SUGGESTION:
 *  多线程使用时，不建议在使用过程中destory
 * DEFECT:
 *  读写被虚假唤醒后，且条件不满足，会重新计算超时时间，会导致总等待时间会超过设定的等待时间
 *  使用pthread_condattr_setclock，可能在某些精简的平台不适配
 * 
*/

#include <stdint.h>
#include "jxc_type.h"

typedef void* jxc_fifo_handle;

#ifdef __cplusplus
extern "C" {
#endif

jxc_fifo_handle jxc_fifo_create(uint32_t maxLen);
void jxc_fifo_destroy(jxc_fifo_handle *handle);
jxc_status jxc_fifo_write(jxc_fifo_handle handle, uint8_t *data, uint32_t len, int timeout);
jxc_status jxc_fifo_read(jxc_fifo_handle handle, uint8_t *data, uint32_t len, int timeout);
jxc_status jxc_fifo_clear(jxc_fifo_handle handle);
uint32_t jxc_fifo_get_count(jxc_fifo_handle handle);


#ifdef __cplusplus
}
#endif


#endif
