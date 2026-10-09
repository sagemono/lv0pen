#include "component.h"
struct component_ztv { long offset_to_top; const void *type_info; void *slots[3]; };
extern const struct component_ztv component__ztv __asm__("_ZTV9component");
long **component__base_dtor(long **self) { *self = (long *)&component__ztv.slots; return self; }
CXX_ALIAS(component__base_dtor, _ZN9componentD2Ev);
