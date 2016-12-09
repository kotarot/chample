/**
 * cubiecube.c
 */

#include <stdio.h>
#include <string.h>
#include "corner.h"
#include "edge.h"
#include "cubiecube.h"
#include "facecube.h"

/**** Moves on the cubie level ****/
char cpU[] = { UBR, URF, UFL, ULB, DFR, DLF, DBL, DRB };
char coU[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
char epU[] = { UB, UR, UF, UL, DR, DF, DL, DB, FR, FL, BL, BR };
char eoU[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

char cpR[] = { DFR, UFL, ULB, URF, DRB, DLF, DBL, UBR };
char coR[] = { 2, 0, 0, 1, 1, 0, 0, 2 };
char epR[] = { FR, UF, UL, UB, BR, DF, DL, DB, DR, FL, BL, UR };
char eoR[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

char cpF[] = { UFL, DLF, ULB, UBR, URF, DFR, DBL, DRB };
char coF[] = { 1, 2, 0, 0, 2, 1, 0, 0 };
char epF[] = { UR, FL, UL, UB, DR, FR, DL, DB, UF, DF, BL, BR };
char eoF[] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 };

char cpD[] = { URF, UFL, ULB, UBR, DLF, DBL, DRB, DFR };
char coD[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
char epD[] = { UR, UF, UL, UB, DF, DL, DB, DR, FR, FL, BL, BR };
char eoD[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

char cpL[] = { URF, ULB, DBL, UBR, DFR, UFL, DLF, DRB };
char coL[] = { 0, 1, 2, 0, 0, 2, 1, 0 };
char epL[] = { UR, UF, BL, UB, DR, DF, FL, DB, FR, UL, DL, BR };
char eoL[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

char cpB[] = { URF, UFL, UBR, DRB, DFR, DLF, ULB, DBL };
char coB[] = { 0, 0, 1, 2, 0, 0, 2, 1 };
char epB[] = { UR, UF, UL, BR, DR, DF, DL, BL, FR, FL, UB, DB };
char eoB[] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 };

// this CubieCube array represents the 6 basic cube moves
CubieCube move_cube[6];

// 初期化 javaでのstaticブロックに相当
void cubiecube_init() {
	//printf("cubiecube.c: cubiecube_init()\n");

	memcpy(move_cube[0].cp, cpU, sizeof(cpU));
	memcpy(move_cube[0].co, coU, sizeof(coU));
	memcpy(move_cube[0].ep, epU, sizeof(epU));
	memcpy(move_cube[0].eo, eoU, sizeof(eoU));

	memcpy(move_cube[1].cp, cpR, sizeof(cpR));
	memcpy(move_cube[1].co, coR, sizeof(coR));
	memcpy(move_cube[1].ep, epR, sizeof(epR));
	memcpy(move_cube[1].eo, eoR, sizeof(eoR));

	memcpy(move_cube[2].cp, cpF, sizeof(cpF));
	memcpy(move_cube[2].co, coF, sizeof(coF));
	memcpy(move_cube[2].ep, epF, sizeof(epF));
	memcpy(move_cube[2].eo, eoF, sizeof(eoF));

	memcpy(move_cube[3].cp, cpD, sizeof(cpD));
	memcpy(move_cube[3].co, coD, sizeof(coD));
	memcpy(move_cube[3].ep, epD, sizeof(epD));
	memcpy(move_cube[3].eo, eoD, sizeof(eoD));

	memcpy(move_cube[4].cp, cpL, sizeof(cpL));
	memcpy(move_cube[4].co, coL, sizeof(coL));
	memcpy(move_cube[4].ep, epL, sizeof(epL));
	memcpy(move_cube[4].eo, eoL, sizeof(eoL));

	memcpy(move_cube[5].cp, cpB, sizeof(cpB));
	memcpy(move_cube[5].co, coB, sizeof(coB));
	memcpy(move_cube[5].ep, epB, sizeof(epB));
	memcpy(move_cube[5].eo, eoB, sizeof(eoB));
}

// コンストラクタ
void cubiecube_construct(CubieCube *cubie) {
	// corner permutation
	char _cp[8] = { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
	// corner orientation
	char _co[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	// edge permutation
	char _ep[12] = { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };
	// edge orientation
	char _eo[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	memcpy(cubie->cp, _cp, 8);
	memcpy(cubie->co, _co, 8);
	memcpy(cubie->ep, _ep, 12);
	memcpy(cubie->eo, _eo, 12);
}

// n choose k
// 調べたら今のところ、n=11, k=6 が最大値であったからメモ化してしまう
int Cnk[12][7] = {
	{1,  0,  0,   0,   0,   0,   0},
	{1,  1,  0,   0,   0,   0,   0},
	{1,  2,  1,   0,   0,   0,   0},
	{1,  3,  3,   1,   0,   0,   0},
	{1,  4,  6,   4,   1,   0,   0},
	{1,  5, 10,   10,  5,   1,   0},
	{1,  6, 15,  20,  15,   6,   1},
	{1,  7, 21,  35,  35,  21,   7},
	{1,  8, 28,  56,  70,  56,  28},
	{1,  9, 36,  84, 126, 126,  84},
	{1, 10, 45, 120, 210, 252, 210},
	{1, 11, 55, 165, 330, 462, 462}
};

// Left rotation of all array elements between l and r
void rotate_left(char arr[], int l, int r) {
	char temp = arr[l];
	int i;

	for (i = l; i < r; i++) {
		arr[i] = arr[i + 1];
	}
	arr[r] = temp;
}

// Right rotation of all array elements between l and r
void rotate_right(char arr[], int l, int r) {
	char temp = arr[r];
	int i;

	for (i = r; i > l; i--) {
		arr[i] = arr[i - 1];
	}
	arr[l] = temp;
}

// return cube in facelet representation
void to_facecube(CubieCube *cubie, FaceCube *fcret) {
	int i, j, n;
	char ori;

	facecube_construct(fcret);
	for (i = 0; i < 8; i++) {
		j = cubie->cp[i]; // cornercubie with index j is at
		// cornerposition with index i
		ori = cubie->co[i]; // Orientation of this cubie
		for (n = 0; n < 3; n++) {
			fcret->f[(int)corner_facelet[i][(n + ori) % 3]] = corner_color[j][n];
		}
	}
	for (i = 0; i < 12; i++) {
		j = cubie->ep[i]; // edgecubie with index j is at edgeposition
		// with index i
		ori = cubie->eo[i]; // Orientation of this cubie
		for (n = 0; n < 2; n++) {
			fcret->f[(int)edge_facelet[i][(n + ori) % 2]] = edge_color[j][n];
		}
	}
}

// Multiply this CubieCube with another cubiecube b, restricted to the corners.<br>
// Because we also describe reflections of the whole cube by permutations, we get a complication with the corners. The
// orientations of mirrored corners are described by the numbers 3, 4 and 5. The composition of the orientations
// cannot
// be computed by addition modulo three in the cyclic group C3 any more. Instead the rules below give an addition in
// the dihedral group D3 with 6 elements.<br>
//	 
// NOTE: Because we do not use symmetry reductions and hence no mirrored cubes in this simple implementation of the
// Two-Phase-Algorithm, some code is not necessary here.
//	
void corner_multiply(CubieCube *cubie, CubieCube *b) {
	char cperm[8], cori[8], oriA, oriB, ori;
	int corn;

	for (corn = 0; corn < 8; corn++) {
		cperm[corn] = cubie->cp[(int)(b->cp[corn])];
		oriA = cubie->co[(int)(b->cp[corn])];
		oriB = b->co[corn];
		ori = 0;
		if (oriA < 3 && oriB < 3) { // if both cubes are regular cubes...
			ori = oriA + oriB; // just do an addition modulo 3 here
			if (ori >= 3) {
				ori -= 3; // the composition is a regular cube
			}
		// ++++ not used in this implementation ++++
		} else if (oriA < 3 && oriB >= 3) { // if cube b is in a mirrored
			// state...
			ori = oriA + oriB;
			if (ori >= 6) {
				ori -= 3; // the composition is a mirrored cube
			}
		} else if (oriA >= 3 && oriB < 3) { // if cube a is an a mirrored
			// state...
			ori = oriA - oriB;
			if (ori < 3) {
				ori += 3; // the composition is a mirrored cube
			}
		} else if (oriA >= 3 && oriB >= 3) { // if both cubes are in mirrored
			// states...
			ori = oriA - oriB;
			if (ori < 0) {
				ori += 3; // the composition is a regular cube
			}
		}
		cori[corn] = ori;
	}
	for (corn = 0; corn < 8; corn++) {
		cubie->cp[corn] = cperm[corn];
		cubie->co[corn] = cori[corn];
	}
}

// Multiply this CubieCube with another cubiecube b, restricted to the edges.
void edge_multiply(CubieCube *cubie, CubieCube *b) {
	char eperm[12], eori[12];
	int edge;

	for (edge = 0; edge < 12; edge++) {
		eperm[edge] = cubie->ep[(int)(b->ep[edge])];
		eori[edge] = (b->eo[edge] + cubie->eo[(int)(b->ep[edge])]) % 2;
	}
	for (edge = 0; edge < 12; edge++) {
		cubie->ep[edge] = eperm[edge];
		cubie->eo[edge] = eori[edge];
	}
}

// **** Get and set coordinates ****
// return the twist of the 8 corners. 0 <= twist < 3^7
short get_twist(CubieCube *cubie) {
	short ret = 0;
	int i;

	for (i = URF; i < DRB; i++) {
		ret = ret * 3 + cubie->co[i];
	}
	return ret;
}
void set_twist(CubieCube *cubie, short twist) {
	int twist_parity = 0, i;

	for (i = DRB - 1; i >= URF; i--) {
		twist_parity += (cubie->co[i] = twist % 3);
		twist /= 3;
	}
	cubie->co[DRB] = (3 - twist_parity % 3) % 3;
}

// return the flip of the 12 edges. 0<= flip < 2^11
short get_flip(CubieCube *cubie) {
	short ret = 0, i;

	for (i = UR; i < BR; i++) {
		ret = ret * 2 + cubie->eo[i];
	}
	return ret;
}

void set_flip(CubieCube *cubie, short flip) {
	int flip_parity = 0, i;

	for (i = BR - 1; i >= UR; i--) {
		flip_parity += cubie->eo[i] = flip % 2;
		flip /= 2;
	}
	cubie->eo[BR] = (2 - flip_parity % 2) % 2;
}

// Parity of the corner permutation
short corner_parity(CubieCube *cubie) {
	int s = 0, i, j;

	for (i = DRB; i >= URF + 1; i--) {
		for (j = i - 1; j >= URF; j--) {
			if (cubie->cp[j] > cubie->cp[i]) {
				s++;
			}
		}
	}
	return s % 2;
}

// Parity of the edges permutation. Parity of corners and edges are the same if the cube is solvable.
short edge_parity(CubieCube *cubie) {
	int s = 0, i, j;

	for (i = BR; i >= UR + 1; i--) {
		for (j = i - 1; j >= UR; j--) {
			if (cubie->ep[j] > cubie->ep[i]) {
				s++;
			}
		}
	}
	return s % 2;
}

/**
 * @author: YY
 */
short corner_roop(CubieCube *cubie) {
	int fill[8];
	int i, j;
	for(i = URF; i <= DRB; i++) {
		fill[i] = 0;
	}
	
	int roop=0;
	int buffer = ULB;//buffer指定
	int nowcube = buffer;
	int nextcube = buffer;
	int roopfirst = buffer;
	for (i = 0; i < 8; i++) {
		nowcube = nextcube;//更新
		nextcube = cubie->cp[nowcube];
		fill[nowcube] = 1;
		if (nextcube == roopfirst) {//ループが終わる場合
			//co,cs判定
			if(nowcube == nextcube && nowcube != buffer) {}
			else {roop++;}

			//新規ループ作成
			for(j = URF; j<= DRB; j++){
				if(fill[j] == 0){
					roopfirst = j;
					nextcube = roopfirst;
					break;
				}
			}
		}
	}
	return roop;
}

/**
 * @author: YY
 */
short edge_roop(CubieCube *cubie, int parity) {
	int fill[12];
	int ep[12];
	int i, j;
	for(i = UR; i <= BR; i++) {
		if(!(cubie->ep[i] == UB || cubie->ep[i] == UL)||parity == 0){
			ep[i] = cubie->ep[i];
		}
		else{//交換分析法
			if(cubie->ep[i] == UB) { ep[i] = UL; }
			else { ep[i] = UB; }
		}
	}
	
	for(i = UR; i <= BR; i++) {
		fill[i] = 0;
	}
	
	int roop=0;
	int buffer = DF;//buffer指定
	int nowcube = buffer;
	int nextcube = buffer;
	int roopfirst = buffer;
	for (i = 0; i < 12; i++) {
		nowcube = nextcube;//更新
		nextcube = ep[nowcube];
		fill[nowcube] = 1;
		if (nextcube == roopfirst) {//ループが終わる場合
			//co,cs判定
			if(nowcube == nextcube && nowcube != buffer) {}
			else {roop++;}

			//新規ループ作成
			for(j = UR; j<= BR; j++){
				if(fill[j] == 0){
					roopfirst = j;
					nextcube = roopfirst;
					break;
				}
			}
		}
	}
	return roop;
}

/**
 * @author: YY
 */
short count_co(CubieCube *cubie) {
	int i;
	int co = 0;
	for(i = URF; i <= DRB; i++) {
		if (i == cubie->cp[i]) {
			if(cubie->co[i] != 0 && i != ULB) co++;
		}
	}
	return co;
}

/**
 * @author: Kotaro
 */
short count_naive_co(CubieCube *cubie) {
	int i;
	int co = 0;
	for (i = URF; i <= DRB; i++) {
		if (i == cubie->cp[i]) {
			if (cubie->co[i] != 0) co++;
		}
	}
	return co;
}

/**
 * @author: YY
 */
short count_eo(CubieCube *cubie, int parity) {
	int i;
	int ep[12];
	for(i = UR; i <= BR; i++) {
		if(!(cubie->ep[i] == UB || cubie->ep[i] == UL)||parity == 0){
			ep[i] = cubie->ep[i];
		}
		else{//交換分析法 epのみ入れ替え
			if(cubie->ep[i] == UB) { ep[i] = UL;}
			else  { ep[i] = UB;}
		}
	}
	int n_eo = 0;
	for(i = UR; i <= BR; i++) {
		if (i == ep[i]) {
			if (cubie->eo[i] != 0 && i != DF) n_eo++;
		}
	}
	return n_eo;
}

/**
 * @author: Kotaro
 */
short count_naive_eo(CubieCube *cubie) {
	int i;
	int eo = 0;
	for (i = UR; i <= BR; i++) {
		if (i == cubie->ep[i]) {
			if (cubie->eo[i] != 0) eo++;
		}
	}
	return eo;
}

/**
 * @author: Kotaro
 */
short count_cf(CubieCube *cubie) {
	int i;
	int cf = 0;
	for (i = URF; i <= DRB; i++) {
		if (i == cubie->cp[i]) {
			if (cubie->co[i] == 0) cf++;
		}
	}
	return cf;
}

/**
 * @author: Kotaro
 */
short count_ef(CubieCube *cubie) {
	int i;
	int ef = 0;
	for (i = UR; i <= BR; i++) {
		if (i == cubie->ep[i]) {
			if (cubie->eo[i] == 0) ef++;
		}
	}
	return ef;
}

/**
 * @author: YY
 */
short corner_number(CubieCube *cubie){
	int fill[8];
	int i, j;
	for(i = URF; i <= DRB; i++) {
		fill[i] = 0;
	}
	
	int roop=0;
	int co = 0;
	int buffer = ULB;//buffer指定
	int nowcube = buffer;
	int nextcube = buffer;
	int roopfirst = buffer;
	for (i = 0; i < 8; i++) {
		nowcube = nextcube;//更新
		nextcube = cubie->cp[nowcube];
		fill[nowcube] = 1;
		if (nextcube == roopfirst) {//ループが終わる場合
			//co,cs判定
			if(nowcube == nextcube && nowcube != buffer) {co++;}
			else {roop++;}

			//新規ループ作成
			for(j = URF; j<= DRB; j++){
				if(fill[j] == 0){
					roopfirst = j;
					nextcube = roopfirst;
					break;
				}
			}
		}
	}
	return 8-1-co+roop-1;
}

/**
 * @author: YY
 */
short edge_number(CubieCube *cubie, int parity){
	int fill[12];
	int ep[12];
	int i, j;
	for(i = UR; i <= BR; i++) {
		if(!(cubie->ep[i] == UB || cubie->ep[i] == UL)||parity == 0){
			ep[i] = cubie->ep[i];
		}
		else{//交換分析法
			if(cubie->ep[i] == UB) { ep[i] = UL; }
			else { ep[i] = UB; }
		}
	}
	for(i = UR; i <= BR; i++) {
		fill[i] = 0;
	}
	
	int roop=0;
	int eo = 0;
	int buffer = DF;//buffer指定
	int nowcube = buffer;
	int nextcube = buffer;
	int roopfirst = buffer;
	for (i = 0; i < 12; i++) {
		nowcube = nextcube;//更新
		nextcube = ep[nowcube];
		fill[nowcube] = 1;
		if (nextcube == roopfirst) {//ループが終わる場合
			//co,cs判定
			if(nowcube == nextcube && nowcube != buffer) {eo++;}//厳密にはeoじゃない
			else {roop++;}

			//新規ループ作成
			for(j = UR; j<= BR; j++){
				if(fill[j] == 0){
					roopfirst = j;
					nextcube = roopfirst;
					break;
				}
			}
		}
	}
	return 12-1-eo+roop-1;
}

// permutation of the UD-slice edges FR,FL,BL and BR
short get_FRtoBR(CubieCube *cubie) {
	int a, b, x, j, k;
	char edge4[4];

	// compute the index a < (12 choose 4) and the permutation array perm.
	for (a = 0, x = 0, j = BR; j >= UR; j--) {
		if (FR <= cubie->ep[j] && cubie->ep[j] <= BR) {
			a += Cnk[11 - j][x + 1];
			edge4[3 - x++] = cubie->ep[j];
		}
	}
	for (b = 0, j = 3; j > 0; j--) { // compute the index b < 4! for the
		// permutation in perm
		for (k = 0; edge4[j] != j + 8; k++) {

			rotate_left(edge4, 0, j);
		}
		b = (j + 1) * b + k;
	}
	return 24 * a + b;
}

void set_FRtoBR(CubieCube *cubie, short idx) {
	int x, e, j, k;
	char slice_edge[] = { FR, FL, BL, BR };
	char other_edge[] = { UR, UF, UL, UB, DR, DF, DL, DB };
	int b = idx % 24; // Permutation
	int a = idx / 24; // Combination

	for (e = 0; e < 12; e++) {
		cubie->ep[e] = DB; // Use UR to invalidate all edges
	}
	for (j = 1; j < 4; j++) { // generate permutation from index b
		k = b % (j + 1);
		b /= j + 1;
		while (k-- > 0) {
			rotate_right(slice_edge, 0, j);
		}
	}
	// generate combination and set slice edges
	for (x = 3, j = UR; j <= BR; j++) {
		if (a - Cnk[11 - j][x + 1] >= 0) {
			cubie->ep[j] = slice_edge[3 - x];
			a -= Cnk[11 - j][x-- + 1];
		}
	}
	// set the remaining edges UR..DB
	for (x = 0, j = UR; j <= BR; j++) {
		if (cubie->ep[j] == DB) {
			cubie->ep[j] = other_edge[x++];
		}
	}
}

// Permutation of all corners except DBL and DRB
short get_URFtoDLF(CubieCube *cubie) {
	int a = 0, b, x = 0, j, k;
	char corner6[6];

	// compute the index a < (8 choose 6) and the corner permutation.
	for (j = URF; j <= DRB; j++) {
		if (cubie->cp[j] <= DLF) {
			a += Cnk[j][x + 1];
			corner6[x++] = cubie->cp[j];
		}
	}
	for (b = 0, j = 5; j > 0; j--) { // compute the index b < 6! for the
		// permutation in corner6
		for (k = 0; corner6[j] != j; k++) {
			rotate_left(corner6, 0, j);
		}
		b = (j + 1) * b + k;
	}
	return 720 * a + b;
}

void set_URFtoDLF(CubieCube *cubie, short idx) {
	int x, j, k;
	char corner6[] = { URF, UFL, ULB, UBR, DFR, DLF };
	char other_corner[] = { DBL, DRB };
	int b = idx % 720; // Permutation
	int a = idx / 720; // Combination

	for (j = 0; j < 8; j++) {
		cubie->cp[j] = DRB; // Use DRB to invalidate all corners
	}
	for (j = 1; j < 6; j++) { // generate permutation from index b
		k = b % (j + 1);
		b /= j + 1;
		while (k-- > 0) {
			rotate_right(corner6, 0, j);
		}
	}
	// generate combination and set corners
	for (x = 5, j = DRB; j >= 0; j--) {
		if (a - Cnk[j][x + 1] >= 0) {
			cubie->cp[j] = corner6[x];
			a -= Cnk[j][x-- + 1];
		}
	}
	for (x = 0, j = URF; j <= DRB; j++) {
		if (cubie->cp[j] == DRB) {
			cubie->cp[j] = other_corner[x++];
		}
	}
}

// Permutation of the six edges UR,UF,UL,UB,DR,DF.
int get_URtoDF(CubieCube *cubie) {
	int a, b, x = 0, j, k;
	char edge6[6];

	// compute the index a < (12 choose 6) and the edge permutation.
	for (a = 0, j = UR; j <= BR; j++) {
		if (cubie->ep[j] <= DF) {
			a += Cnk[j][x + 1];
			edge6[x++] = cubie->ep[j];
		}
	}
	for (b = 0, j = 5; j > 0; j--) { // compute the index b < 6! for the
		// permutation in edge6
		for (k = 0; edge6[j] != j; k++) {
			rotate_left(edge6, 0, j);
		}
		b = (j + 1) * b + k;
	}
	return 720 * a + b;
}

// Permutation of the six edges UR,UF,UL,UB,DR,DF
int get_URtoDF_short(short idx1, short idx2) {
	CubieCube a;
	CubieCube b;
	int i;

	cubiecube_construct(&a);
	cubiecube_construct(&b);
	set_URtoUL(&a, idx1);
	set_UBtoDF(&b, idx2);
	for (i = 0; i < 8; i++) {
		if (a.ep[i] != BR) {
			if (b.ep[i] != BR) { // collision
				return -1;
			} else {
				b.ep[i] = a.ep[i];
			}
		}
	}
	return get_URtoDF(&b);
}

void set_URtoDF(CubieCube *cubie, int idx) {
	int x, e, j, k;
	char edge6[] = { UR, UF, UL, UB, DR, DF };
	char other_edge[] = { DL, DB, FR, FL, BL, BR };
	int b = idx % 720; // Permutation
	int a = idx / 720; // Combination

	for (e = 0; e < 12; e++) {
		cubie->ep[e] = BR;// Use BR to invalidate all edges
	}
	for (j = 1; j < 6; j++) { // generate permutation from index b
		k = b % (j + 1);
		b /= j + 1;
		while (k-- > 0) {
			rotate_right(edge6, 0, j);
		}
	}
	// generate combination and set edges
	for (x = 5, j = BR; j >= 0; j--) {
		if (a - Cnk[j][x + 1] >= 0) {
			cubie->ep[j] = edge6[x];
			a -= Cnk[j][x-- + 1];
		}
	}
	// set the remaining edges DL..BR
	for (x = 0, j = UR; j <= BR; j++) {
		if (cubie->ep[j] == BR) {
			cubie->ep[j] = other_edge[x++];
		}
	}
}

// Permutation of the three edges UR,UF,UL
short get_URtoUL(CubieCube *cubie) {
	int a = 0, b, j, k, x = 0;
	char edge3[3];

	// compute the index a < (12 choose 3) and the edge permutation.
	for (j = UR; j <= BR; j++) {
		if (cubie->ep[j] <= UL) {
			a += Cnk[j][x + 1];
			edge3[x++] = cubie->ep[j];
		}
	}
	for (b = 0, j = 2; j > 0; j--) { // compute the index b < 3! for the
		// permutation in edge3
		for (k = 0; edge3[j] != j; k++) {
			rotate_left(edge3, 0, j);
		}
		b = (j + 1) * b + k;
	}
	return 6 * a + b;
}

void set_URtoUL(CubieCube *cubie, short idx) {
	int e, j, x, k;
	char edge3[] = { UR, UF, UL };
	int b = idx % 6; // Permutation
	int a = idx / 6; // Combination

	for (e = 0; e < 12; e++) {
		cubie->ep[e] = BR; // Use BR to invalidate all edges
	}
	for (j = 1; j < 3; j++) { // generate permutation from index b
		k = b % (j + 1);
		b /= j + 1;
		while (k-- > 0) {
			rotate_right(edge3, 0, j);
		}
	}
	// generate combination and set edges
	for (x = 2, j = BR; j >= 0; j--) {
		if (a - Cnk[j][x + 1] >= 0) {
			cubie->ep[j] = edge3[x];
			a -= Cnk[j][x-- + 1];
		}
	}
}

// Permutation of the three edges UB,DR,DF
short get_UBtoDF(CubieCube *cubie) {
	int a = 0, b, x = 0, j, k;
	char edge3[3];

	// compute the index a < (12 choose 3) and the edge permutation.
	for (j = UR; j <= BR; j++) {
		if (UB <= cubie->ep[j] && cubie->ep[j] <= DF) {
			a += Cnk[j][x + 1];
			edge3[x++] = cubie->ep[j];
		}
	}
	for (b = 0, j = 2; j > 0; j--) { // compute the index b < 3! for the
		// permutation in edge3
		for (k = 0; edge3[j] != UB + j; k++) {
			rotate_left(edge3, 0, j);
		}
		b = (j + 1) * b + k;
	}
	return 6 * a + b;
}

void set_UBtoDF(CubieCube *cubie, short idx) {
	int e, j, x, k;
	char edge3[] = { UB, DR, DF };
	int b = idx % 6; // Permutation
	int a = idx / 6; // Combination

	for (e = 0; e < 12; e++) {
		cubie->ep[e] = BR;// Use BR to invalidate all edges
	}
	for (j = 1; j < 3; j++) { // generate permutation from index b
		k = b % (j + 1);
		b /= j + 1;
		while (k-- > 0) {
			rotate_right(edge3, 0, j);
		}
	}
	// generate combination and set edges
	for (x = 2, j = BR; j >= 0; j--) {
		if (a - Cnk[j][x + 1] >= 0) {
			cubie->ep[j] = edge3[x];
			a -= Cnk[j][x-- + 1];
		}
	}
}

void set_URFtoDLB(CubieCube *cubie, int idx) {
	char perm[] = { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
	int j, k;

	for (j = 1; j < 8; j++) {
		k = idx % (j + 1);
		idx /= j + 1;
		while (k-- > 0) {
			rotate_right(perm, 0, j);
		}
	}
	// set corners
	for (j = 7; j >= 0; j--) {
		cubie->cp[j] = perm[j];
	}
}

/**
 * @author: Kotaro
 */
void set_URFtoDLB_with_fixed(CubieCube *cubie, int idx, char *fixed_c) {
	int i, j;

	// シャッフルした後，
	// 固定コーナーを正しいものと交換することで元に戻す．
	// 数学的には正しくないかもしれない．
	set_URFtoDLB(cubie, idx);
	for (i = 0; i < 8; i++) {
		if (fixed_c[i] == 1 && cubie->cp[i] != i) {
			for (j = 0; j < 8; j++) {
				if (cubie->cp[j] == i) {
					cubie->cp[j] = cubie->cp[i];
					cubie->cp[i] = i;
					break;
				}
			}
		}
	}
}

void set_URtoBR(CubieCube *cubie, int idx) {
	char perm[] = { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };
	int j, k;

	for (j = 1; j < 12; j++) {
		k = idx % (j + 1);
		idx /= j + 1;
		while (k-- > 0) {
			rotate_right(perm, 0, j);
		}
	}
	// set edges
	for (j = 11; j >= 0; j--) {
		cubie->ep[j] = perm[j];
	}
}

/**
 * @author: Kotaro
 */
void set_URtoBR_with_fixed(CubieCube *cubie, int idx, char *fixed_e) {
	int i, j;

	// シャッフルした後，
	// 固定エッジを正しいものと交換することで元に戻す．
	// 数学的には正しくないかもしれない．
	set_URtoBR(cubie, idx);
	for (i = 0; i < 12; i++) {
		if (fixed_e[i] == 1 && cubie->ep[i] != i) {
			for (j = 0; j < 12; j++) {
				if (cubie->ep[j] == i) {
					cubie->ep[j] = cubie->ep[i];
					cubie->ep[i] = i;
					break;
				}
			}
		}
	}
}

// Check a cubiecube for solvability. Return the error code.
// 0: Cube is solvable
// -2: Not all 12 edges exist exactly once
// -3: Flip error: One edge has to be flipped
// -4: Not all corners exist exactly once
// -5: Twist error: One corner has to be twisted
// -6: Parity error: Two corners ore two edges have to be exchanged
int cubiecube_verify(CubieCube *cubie) {
	int edge_count[12] = {0};
	int corner_count[8] = {0};
	int sum, c, e, i;

	// edge
	for (e = 0; e < 12; e++) {
		edge_count[(int)(cubie->ep[e])]++;
	}
	for (i = 0; i < 12; i++) {
		if (edge_count[i] != 1) {
			return -2; // missing edges
		}
	}
	for (sum = 0, i = 0; i < 12; i++) {
		sum += cubie->eo[i];
	}
	if (sum % 2 != 0) {
		return -3; // fliped edge
	}

	// corner
	for (c = 0; c < 8; c++) {
		corner_count[(int)(cubie->cp[c])]++;
	}
	for (i = 0; i < 8; i++) {
		if (corner_count[i] != 1) {
			return -4; // missing corners
		}
	}
	for (sum = 0, i = 0; i < 8; i++) {
		sum += cubie->co[i];
	}
	if (sum % 3 != 0) {
		return -5; // twisted corner
	}

	// edges and corners parity check
	if ((edge_parity(cubie) ^ corner_parity(cubie)) != 0) {
		return -6; // parity error
	}
	return 0; // cube ok
}
