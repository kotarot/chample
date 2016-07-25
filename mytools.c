/**
 * mytools.c
 *
 * My tools for using Two-Phase Algorithms
 * @author KT
 */
#include <stdlib.h>
#include <string.h>
#include "mytools.h"
#include "coordcube.h"
#include "cubiecube.h"
#include "facecube.h"
#include "mt19937ar.c"

// メルセンヌツイスタの初期化
void mt_init(unsigned long s)
{
	init_genrand(s);
}

// 0以上(max-1)の乱数を返す
int next_int(int max)
{
	return (int)( ((double)(genrand_int32() & 0x7fffffff) / (unsigned long)0x80000000) * max );
}

// Check if the cube string s represents a solvable cube.
// 0: Cube is solvable
// -1: There is not exactly one facelet of each colour
// -2: Not all 12 edges exist exactly once
// -3: Flip error: One edge has to be flipped
// -4: Not all corners exist exactly once
// -5: Twist error: One corner has to be twisted
// -6: Parity error: Two corners or two edges have to be exchanged
// 
/**
 * Check if the cube definition string s represents a solvable cube.
 * 
 * @param s is the cube definition string , see {@link Facelet}
 * @return 0: Cube is solvable<br>
 *         -1: There is not exactly one facelet of each colour<br>
 *         -2: Not all 12 edges exist exactly once<br>
 *         -3: Flip error: One edge has to be flipped<br>
 *         -4: Not all 8 corners exist exactly once<br>
 *         -5: Twist error: One corner has to be twisted<br>
 *         -6: Parity error: Two corners or two edges have to be exchanged
 */
int mytools_verify(char *s)
{
	int count[6] = {0};
	int i;
	FaceCube fc;
	CubieCube cc;

	for (i = 0; i < 54; i++) {
		switch (s[i]) {
			case 'U':
				count[0]++; break;
			case 'R':
				count[1]++; break;
			case 'F':
				count[2]++; break;
			case 'D':
				count[3]++; break;
			case 'L':
				count[4]++; break;
			case 'B':
				count[5]++; break;
			default:
				return -1;
		}
	}
	for (i = 0; i < 6; i++) {
		if (count[i] != 9) {
			return -1;
		}
	}

	facecube_construct_str(&fc, s);
	to_cubiecube(&fc, &cc);
	return cubiecube_verify(&cc);
}

/**
 * Generates a random cube.
 * @return A random cube in the string representation. Each cube of the cube space has the same probability.
 */
void random_cube(char *des) {
	CubieCube cc;
	FaceCube fc;

	cubiecube_construct(&cc);
	set_flip(&cc, next_int(N_FLIP));
	set_twist(&cc, next_int(N_TWIST));
	do {
		set_URFtoDLB(&cc, next_int(N_URFtoDLB));
		set_URtoBR(&cc, next_int(N_URtoBR));
	} while ((edge_parity(&cc) ^ corner_parity(&cc)) != 0);
	to_facecube(&cc, &fc);
	to_string(&fc, des);
}

/**
 * Generates a random cube with parity.
 * @return A random cube with parity in the string representation. Each cube of the cube space has the same probability.
 */
void random_cube_with_parity(char *des) {
	CubieCube cc;
	FaceCube fc;

	cubiecube_construct(&cc);
	set_flip(&cc, next_int(N_FLIP));
	set_twist(&cc, next_int(N_TWIST));
	// パリティがそれぞれ1,1になるまで繰り返し
	do {
		set_URFtoDLB(&cc, next_int(N_URFtoDLB));
		set_URtoBR(&cc, next_int(N_URtoBR));
	} while (edge_parity(&cc) == 0 || corner_parity(&cc) == 0);
	to_facecube(&cc, &fc);
	to_string(&fc, des);
}

/**
 * Generates a random cube with no parity.
 * @return A random cube with no parity in the string representation. Each cube of the cube space has the same probability.
 */
void random_cube_with_no_parity(char *des) {
	CubieCube cc;
	FaceCube fc;

	cubiecube_construct(&cc);
	set_flip(&cc, next_int(N_FLIP));
	set_twist(&cc, next_int(N_TWIST));
	// パリティがそれぞれ0,0になるまで繰り返し
	do {
		set_URFtoDLB(&cc, next_int(N_URFtoDLB));
		set_URtoBR(&cc, next_int(N_URtoBR));
	} while (edge_parity(&cc) != 0 || corner_parity(&cc) != 0);
	to_facecube(&cc, &fc);
	to_string(&fc, des);
}

/**
 * Generates a edge-randomized cube.
 * @return A edge-randomized cube in the string representation. Each cube of the cube space has the same probability.
 */
void edge_random_cube(char *des) {
	CubieCube cc;
	FaceCube fc;

	cubiecube_construct(&cc);
	set_flip(&cc, next_int(N_FLIP));
	do {
		set_URtoBR(&cc, next_int(N_URtoBR));
	} while (edge_parity(&cc) != 0);
	to_facecube(&cc, &fc);
	to_string(&fc, des);
}

/**
 * Generates a corner-randomized cube.
 * @return A corner-randomized cube in the string representation. Each cube of the cube space has the same probability.
 */
void corner_random_cube(char *des) {
	CubieCube cc;
	FaceCube fc;

	cubiecube_construct(&cc);
	set_twist(&cc, next_int(N_TWIST));
	do {
		set_URFtoDLB(&cc, next_int(N_URFtoDLB));
	} while (corner_parity(&cc) != 0);
	to_facecube(&cc, &fc);
	to_string(&fc, des);
}

void edit_cube(char *des) {
	CubieCube cc;
	FaceCube fc;
	//int parity;

	cubiecube_construct(&cc);
	do {
		set_flip(&cc, next_int(N_FLIP));
		set_URtoBR(&cc, next_int(N_URtoBR));
		set_twist(&cc, next_int(N_TWIST));
		set_URFtoDLB(&cc, next_int(N_URFtoDLB));
	} while ((edge_parity(&cc) ^ corner_parity(&cc)) != 0 /*|| corner_roop(&cc) != 2*/ || count_co(&cc) != 4);
	to_facecube(&cc, &fc);
	to_string(&fc, des);
}

void edit_cube2(char *des) {
	CubieCube cc;
	FaceCube fc;
	int parity;

	cubiecube_construct(&cc);
	do {
		set_flip(&cc, next_int(N_FLIP));
		set_URtoBR(&cc, next_int(N_URtoBR));
		set_twist(&cc, next_int(N_TWIST));
		set_URFtoDLB(&cc, next_int(N_URFtoDLB));
		parity = corner_parity(&cc);
	} while ((edge_parity(&cc) ^ corner_parity(&cc)) != 0  || parity != 1 || corner_roop(&cc) != 1 || edge_number(&cc, parity) != 14 || count_eo(&cc, parity) != 0|| count_co(&cc) != 0);
	to_facecube(&cc, &fc);
	to_string(&fc, des);
}

/**
 * 手順の文字列を逆手順にする (switch文方式)
 * @return 逆手順にされた手順の文字列
 */
void reverse_alg(char *src, char *des)
{
	int i, j;

	// 常に1手3文字(スペース含む)になるように変更した
	for (i = 0, j = strlen(src) - 3; j >= 0; j -= 3) {
		des[i++] = src[j];
		switch (src[j + 1]) {
			case ' ':
				des[i++] = '\''; break;
			case '2':
				des[i++] = '2'; break;
		}
		if (j == 0) {
			break;
		}
		des[i++] = ' ';
	}
	des[i] = '\0';
}
