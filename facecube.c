/**
 * facecube.c
 * 
 * Cube on the facelet level
 */

#include <string.h>
#include "facecube.h"
#include "cubiecube.h"
#include "facelet.h"
#include "color.h"
#include "corner.h"
#include "edge.h"

// Map the corner positions to facelet positions. cornerFacelet[URF.ordinal()][0] e.g. gives the position of the
// facelet in the URF corner position, which defines the orientation.<br>
// cornerFacelet[URF.ordinal()][1] and cornerFacelet[URF.ordinal()][2] give the position of the other two facelets
// of the URF corner (clockwise).
char corner_facelet[8][3] = {
	{ U9, R1, F3 }, { U7, F1, L3 }, { U1, L1, B3 }, { U3, B1, R3 },
	{ D3, F9, R7 }, { D1, L9, F7 }, { D7, B9, L7 }, { D9, R9, B7 }
};
// Map the edge positions to facelet positions. edgeFacelet[UR.ordinal()][0] e.g. gives the position of the facelet in
// the UR edge position, which defines the orientation.<br>
// edgeFacelet[UR.ordinal()][1] gives the position of the other facelet
char edge_facelet[12][2] = {
	{ U6, R2 }, { U8, F2 }, { U4, L2 }, { U2, B2 }, { D6, R8 }, { D2, F8 },
	{ D4, L8 }, { D8, B8 }, { F6, R4 }, { F4, L6 }, { B6, L4 }, { B4, R6 }
};
// Map the corner positions to facelet colors.
char corner_color[8][3] = {
	{ U, R, F }, { U, F, L }, { U, L, B }, { U, B, R },
	{ D, F, R }, { D, L, F }, { D, B, L }, { D, R, B }
};
// Map the edge positions to facelet colors.
char edge_color[12][2] = {
	{ U, R }, { U, F }, { U, L }, { U, B }, { D, R }, { D, F },
	{ D, L }, { D, B }, { F, R }, { F, L }, { B, L }, { B, R }
};

// コンストラクタ
void facecube_construct(FaceCube *fc) {
	char str[] = "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB";
	int i;

	for (i = 0; i < 54; i++) {
		switch (str[i]) {
			case 'U':
				fc->f[i] = 0; break;
			case 'R':
				fc->f[i] = 1; break;
			case 'F':
				fc->f[i] = 2; break;
			case 'D':
				fc->f[i] = 3; break;
			case 'L':
				fc->f[i] = 4; break;
			case 'B':
				fc->f[i] = 5; break;
		}
	}
}
// Construct a facelet cube from a string
void facecube_construct_str(FaceCube *fc, char cube_str[]) {
	int i, len;

	for (i = 0, len = strlen(cube_str); i < len; i++) {
		switch (cube_str[i]) {
			case 'U':
				fc->f[i] = 0; break;
			case 'R':
				fc->f[i] = 1; break;
			case 'F':
				fc->f[i] = 2; break;
			case 'D':
				fc->f[i] = 3; break;
			case 'L':
				fc->f[i] = 4; break;
			case 'B':
				fc->f[i] = 5; break;
		}
	}
}

// Gives string representation of a facelet cube
void to_string(FaceCube *fc, char *s) {
	char to[6] = {'U', 'R', 'F', 'D', 'L', 'B'};
	int i;

	for (i = 0; i < 54; i++) {
		s[i] = to[(int)(fc->f[i])];
	}
	s[54] = '\0';
}

// Gives CubieCube representation of a faceletcube
void to_cubiecube(FaceCube *fc, CubieCube *ccret) {
	char col1, col2;
	int ori, i, j;

	cubiecube_construct(ccret);
	for (i = 0; i < 8; i++) {
		ccret->cp[i] = URF; // invalidate corners
	}
	for (i = 0; i < 12; i++) {
		ccret->ep[i] = UR; // and edges
	}
	for (i = 0; i < 8; i++) {
		// get the colors of the cubie at corner i, starting with U/D
		for (ori = 0; ori < 3; ori++) {
			if (fc->f[(int)corner_facelet[i][ori]] == U || fc->f[(int)corner_facelet[i][ori]] == D) {
				break;
			}
		}
		col1 = fc->f[(int)corner_facelet[i][(ori + 1) % 3]];
		col2 = fc->f[(int)corner_facelet[i][(ori + 2) % 3]];
		for (j = 0; j < 8; j++) {
			if (col1 == corner_color[j][1] && col2 == corner_color[j][2]) {
				// in cornerposition i we have cornercubie j
				ccret->cp[i] = j;
				ccret->co[i] = ori % 3;
				break;
			}
		}
	}
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 12; j++) {
			if (fc->f[(int)edge_facelet[i][0]] == edge_color[j][0] && fc->f[(int)edge_facelet[i][1]] == edge_color[j][1]) {
				ccret->ep[i] = j;
				ccret->eo[i] = 0;
				break;
			}
			if (fc->f[(int)edge_facelet[i][0]] == edge_color[j][1] && fc->f[(int)edge_facelet[i][1]] == edge_color[j][0]) {
				ccret->ep[i] = j;
				ccret->eo[i] = 1;
				break;
			}
		}
	}
}


