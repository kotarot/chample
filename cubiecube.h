/**
 * cubiecube.h
 */

#ifndef __TPD_CUBIECUBE__
#define __TPD_CUBIECUBE__

// initialize to Id-Cube
typedef struct _TAG_CUBIECUBE {
    char cp[8];
    char co[8];
    char ep[12];
    char eo[12];
} CubieCube;

#include "facecube.h"

extern CubieCube move_cube[6];

// 初期化 javaでのstaticブロックに相当
extern void cubiecube_init(void);

// コンストラクタ
extern void cubiecube_construct(CubieCube *cubie);

void rotate_left(char arr[], int l, int r);
void rotate_right(char arr[], int l, int r);

extern void to_facecube(CubieCube *cubie, FaceCube *fcret);

extern void corner_multiply(CubieCube *cubie, CubieCube *b);
extern void edge_multiply(CubieCube *cubie, CubieCube *b);

extern short get_twist(CubieCube *cubie);
extern void set_twist(CubieCube *cubie, short twist);

extern short get_flip(CubieCube *cubie);
extern void set_flip(CubieCube *cubie, short flip);

extern short corner_parity(CubieCube *cubie);
extern short edge_parity(CubieCube *cubie);

extern short get_FRtoBR(CubieCube *cubie);
extern void set_FRtoBR(CubieCube *cubie, short idx);

extern short get_URFtoDLF(CubieCube *cubie);
extern void set_URFtoDLF(CubieCube *cubie, short idx);

extern int get_URtoDF(CubieCube *cubie);
extern int get_URtoDF_short(short idx1, short idx2);
extern void set_URtoDF(CubieCube *cubie, int idx);

extern short get_URtoUL(CubieCube *cubie);
extern void set_URtoUL(CubieCube *cubie, short idx);

extern short get_UBtoDF(CubieCube *cubie);
extern void set_UBtoDF(CubieCube *cubie, short idx);
extern void set_URFtoDLB(CubieCube *cubie, int idx);
extern void set_URtoBR(CubieCube *cubie, int idx);

extern int cubiecube_verify(CubieCube *cubie);

#endif


