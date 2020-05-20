#ifndef MACROS_H
#define MACROS_H

#ifndef __INTELLISENSE__
#define SDCC_FIXED_ADDRESS(_address) __at (_address)
#else
#define SDCC_FIXED_ADDRESS(_address)
#endif //__INTELLISENSE__

#define MIN(_x,_y) ((_x<_y)?_x:_y)
#define MAX(_x,_y) ((_x<_y)?_y:_x)

#define CLAMP(_v,_min,_max) MIN(MAX(_v,_min),_max)

#define VIEW_ROTATION_Z_MAX (8)

#define UNUSED(_v) (void)(_v)
#endif //MACROS_H

