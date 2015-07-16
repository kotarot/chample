/**
 * coordcube.h
 * 
 * coordcube.cのヘッダファイル
 */

#ifndef __TPD_COORDCUBE__
#define __TPD_COORDCUBE__

#define N_TWIST 2187     // 3^7 possible corner orientations
#define N_FLIP 2048      // 2^11 possible edge flips
#define N_SLICE1 495     // 12 choose 4 possible positions of FR,FL,BL,BR edges
#define N_SLICE2 24      // 4! permutations of FR,FL,BL,BR edges in phase2
#define N_PARITY 2       // 2 possible corner parities
#define N_URFtoDLF 20160 // 8!/(8-6)! permutation of URF,UFL,ULB,UBR,DFR,DLF corners
#define N_FRtoBR 11880   // 12!/(12-4)! permutation of FR,FL,BL,BR edges
#define N_URtoUL 1320    // 12!/(12-3)! permutation of UR,UF,UL edges
#define N_UBtoDF 1320    // 12!/(12-3)! permutation of UB,DR,DF edges
#define N_URtoDF 20160   // 8!/(8-6)! permutation of UR,UF,UL,UB,DR,DF edges in phase2

#define N_URFtoDLB 40320   // 8! permutations of the corners
#define N_URtoBR 479001600 // 8! permutations of the corners

#define N_MOVE 18

// 後で計算してるから追加した
#define N_SLICE2_2 48
#define N_SLICE2_URFtoDLF_PARITY 967680
#define N_SLICE2_URFtoDLF_PARITY_2 483840
#define N_SLICE2_URtoDF_PARITY 967680
#define N_SLICE2_URtoDF_PARITY_2 483840
#define N_SLICE1_TWIST 1082565
#define N_SLICE1_TWIST_2 541282
#define N_SLICE1_FLIP 1013760
#define N_SLICE1_FLIP_2 506880

typedef struct _TAG_COORDCUBE {
    short twist;
    short flip;
    short parity;
    short FRtoBR;
    short URFtoDLF;
    short URtoUL;
    short UBtoDF;
    int URtoDF;
} CoordCube;

#include "cubiecube.h"

extern void coordcube_init(void);

extern void coordcube_construct(CoordCube *coord, CubieCube *cubie);

extern short twist_move[N_TWIST][N_MOVE];
extern short flip_move[N_FLIP][N_MOVE];
extern short parity_move[2][18];
extern short FRtoBR_move[N_FRtoBR][N_MOVE];
extern short URFtoDLF_move[N_URFtoDLF][N_MOVE];
extern short URtoDF_move[N_URtoDF][N_MOVE];
extern short URtoUL_move[N_URtoUL][N_MOVE];
extern short UBtoDF_move[N_UBtoDF][N_MOVE];
extern short merge_URtoULandUBtoDF[336][336];
extern char slice_URFtoDLF_parity_prun[N_SLICE2_URFtoDLF_PARITY_2];
extern char slice_URtoDF_parity_prun[N_SLICE2_URtoDF_PARITY_2];
extern char slice_twist_prun[N_SLICE1_TWIST_2 + 1];
extern char slice_flip_prun[N_SLICE1_FLIP_2];

void set_pruning(char *table, int index, char value);
extern char get_pruning(char *table, int index);

#endif


