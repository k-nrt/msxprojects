#ifndef KEY_MATRIX_H
#define KEY_MATRIX_H

#include <msx-types.h>

#define KM_MAKE(_c,_b) ((_c<<8)|(1<<_b))

#define KM_7 KM_MAKE(0,7)
#define KM_6 KM_MAKE(0,6)
#define KM_5 KM_MAKE(0,5)
#define KM_4 KM_MAKE(0,4)
#define KM_3 KM_MAKE(0,3)
#define KM_2 KM_MAKE(0,2)
#define KM_1 KM_MAKE(0,1)
#define KM_0 KM_MAKE(0,0)

//#define KM_7 KM_MAKE(1,7)
//#define KM_6 KM_MAKE(1,6)
//#define KM_5 KM_MAKE(1,5)
//#define KM_4 KM_MAKE(1,4)
//#define KM_3 KM_MAKE(1,2)
#define KM_9 KM_MAKE(1,1)
#define KM_8 KM_MAKE(1,0)

#define KM_B KM_MAKE(2,7)
#define KM_A KM_MAKE(2,6)
//#define KM_5 KM_MAKE(2,5)
//#define KM_4 KM_MAKE(2,4)
//#define KM_3 KM_MAKE(2,2)
//#define KM_2 KM_MAKE(2,1)
//#define KM_1 KM_MAKE(2,0)

#define KM_J KM_MAKE(3,7)
#define KM_I KM_MAKE(3,6)
#define KM_H KM_MAKE(3,5)
#define KM_G KM_MAKE(3,4)
#define KM_F KM_MAKE(3,3)
#define KM_E KM_MAKE(3,2)
#define KM_D KM_MAKE(3,1)
#define KM_C KM_MAKE(3,0)

#define KM_R KM_MAKE(4,7)
#define KM_Q KM_MAKE(4,6)
#define KM_P KM_MAKE(4,5)
#define KM_O KM_MAKE(4,4)
#define KM_N KM_MAKE(4,3)
#define KM_M KM_MAKE(4,2)
#define KM_L KM_MAKE(4,1)
#define KM_K KM_MAKE(4,0)

#define KM_Z KM_MAKE(5,7)
#define KM_Y KM_MAKE(5,6)
#define KM_X KM_MAKE(5,5)
#define KM_W KM_MAKE(5,4)
#define KM_V KM_MAKE(5,3)
#define KM_U KM_MAKE(5,2)
#define KM_T KM_MAKE(5,1)
#define KM_S KM_MAKE(5,0)

#define KM_F3F8  KM_MAKE(6,7)
#define KM_F2F7  KM_MAKE(6,6)
#define KM_F1F6  KM_MAKE(6,5)
#define KM_KANA  KM_MAKE(6,4)
#define KM_CAPS  KM_MAKE(6,3)
#define KM_GRAPH KM_MAKE(6,2)
#define KM_CTRL  KM_MAKE(6,1)
#define KM_SHIFT KM_MAKE(6,0)

#define KM_RETURN KM_MAKE(7,7)
#define KM_SELECT KM_MAKE(7,6)
#define KM_BS     KM_MAKE(7,5)
#define KM_STOP   KM_MAKE(7,4)
#define KM_TAB    KM_MAKE(7,3)
#define KM_ESC    KM_MAKE(7,2)
#define KM_F5F10  KM_MAKE(7,1)
#define KM_F4F9   KM_MAKE(7,0)

#define KM_RIGHT  KM_MAKE(8,7)
#define KM_DOWN   KM_MAKE(8,6)
#define KM_UP     KM_MAKE(8,5)
#define KM_LEFT   KM_MAKE(8,4)
#define KM_DEL    KM_MAKE(8,3)
#define KM_INS    KM_MAKE(8,2)
#define KM_HOME   KM_MAKE(8,1)
#define KM_SPACE  KM_MAKE(8,0)

u8 KeyMatrix_Press(s16 km);

#endif //KEY_MATRIX_H
