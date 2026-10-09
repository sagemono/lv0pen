#include "storage.h"
extern const struct storage_manager_ztv storage_manager__ztv __asm__("_ZTV15storage_manager");
long **storage_manager__base_dtor(long **self) { *self = (long *)&storage_manager__ztv.slots; return self; }
CXX_ALIAS(storage_manager__base_dtor, _ZN15storage_managerD2Ev);
