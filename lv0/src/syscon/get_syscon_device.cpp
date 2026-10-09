#include "cxx.h"
#include "syscon.h"

u16 g_send_and_receive_transaction_id;
u32 g_send_and_receive_communication_tag;
u16 g_sc_transaction_id;
unsigned int g_sc_swbint_pending;

inline syscon *get_syscon_device()
{
    static syscon device;
    return &device;
}

CXX_DROPPED long get_syscon_device_dropped_user()
{
    return get_syscon_device()->base;
}
