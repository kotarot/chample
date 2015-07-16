/**
 * coordcube.c
 * 
 * Representation of the cube on the coordinate level
 */

#include <stdio.h>
#include "coordcube.h"
#include "cubiecube.h"

// Generate a CoordCube from a CubieCube
// コンストラクタ
void coordcube_construct(CoordCube *coord, CubieCube *cubie) {
    coord->twist = get_twist(cubie);
    coord->flip = get_flip(cubie);
    coord->parity = corner_parity(cubie);
    coord->FRtoBR = get_FRtoBR(cubie);
    coord->URFtoDLF = get_URFtoDLF(cubie);
    coord->URtoUL = get_URtoUL(cubie);
    coord->UBtoDF = get_UBtoDF(cubie);
    coord->URtoDF = get_URtoDF(cubie); // only needed in phase2
}


// **** Phase 1 move tables ****
// Move table for the twists of the corners
// twist < 2187 in phase 2.
// twist = 0 in phase 2.
short twist_move[N_TWIST][N_MOVE];
// Move table for the flips of the edges
// flip < 2048 in phase 1
// flip = 0 in phase 2.
short flip_move[N_FLIP][N_MOVE];
// Parity of the corner permutation. This is the same as the parity for the edge permutation of a valid cube.
// parity has values 0 and 1
short parity_move[2][18] = {
    { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
    { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 }
};

// **** Phase 1 and 2 movetable ****
// Move table for the four UD-slice edges FR, FL, Bl and BR
// FRtoBRMove < 11880 in phase 1
// FRtoBRMove < 24 in phase 2
// FRtoBRMove = 0 for solved cube
short FRtoBR_move[N_FRtoBR][N_MOVE];
// Move table for permutation of six corners. The positions of the DBL and DRB corners are determined by the parity.
// URFtoDLF < 20160 in phase 1
// URFtoDLF < 20160 in phase 2
// URFtoDLF = 0 for solved cube.
short URFtoDLF_move[N_URFtoDLF][N_MOVE];
// Move table for the permutation of six U-face and D-face edges in phase2. The positions of the DL and DB edges are
// determined by the parity.
// URtoDF < 665280 in phase 1
// URtoDF < 20160 in phase 2
// URtoDF = 0 for solved cube.
short URtoDF_move[N_URtoDF][N_MOVE];

// **** helper move tables to compute URtoDF for the beginning of phase2 ****
// Move table for the three edges UR,UF and UL in phase1.
short URtoUL_move[N_URtoUL][N_MOVE];
// Move table for the three edges UB,DR and DF in phase1.
short UBtoDF_move[N_UBtoDF][N_MOVE];
// Table to merge the coordinates of the UR,UF,UL and UB,DR,DF edges at the beginning of phase2
short merge_URtoULandUBtoDF[336][336];

// **** Pruning tables for the search ****
// Pruning table for the permutation of the corners and the UD-slice edges in phase2.
// The pruning table entries give a lower estimation for the number of moves to reach the solved cube.
char slice_URFtoDLF_parity_prun[N_SLICE2_URFtoDLF_PARITY_2];
// Pruning table for the permutation of the edges in phase2.
// The pruning table entries give a lower estimation for the number of moves to reach the solved cube.
char slice_URtoDF_parity_prun[N_SLICE2_URtoDF_PARITY_2];
// Pruning table for the twist of the corners and the position (not permutation) of the UD-slice edges in phase1
// The pruning table entries give a lower estimation for the number of moves to reach the H-subgroup.
char slice_twist_prun[N_SLICE1_TWIST_2 + 1];
// Pruning table for the flip of the edges and the position (not permutation) of the UD-slice edges in phase1
// The pruning table entries give a lower estimation for the number of moves to reach the H-subgroup.
char slice_flip_prun[N_SLICE1_FLIP_2];


// 初期化 javaでのstaticブロックに相当
void coordcube_init() {
    int i, j, k;
/*    { // TEST
        CubieCube c;
        printf("coordcube.c: TEST\n");
        cubiecube_construct(&c);
        for (i = 0; i < N_TWIST; i++) {
            set_twist(&c, i);
for (j = 0; j < 8; j++) fprintf(stderr, "%d", c.co[j]);
fprintf(stderr, "\n");
        }
exit(1);
    }*/
    {
		CubieCube c;

		//printf("coordcube.c: coordcube_init(): 1\n");
        for (i = 0; i < N_TWIST; i++) {
            set_twist(&c, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    corner_multiply(&c, &move_cube[j]);
                    twist_move[i][j * 3 + k] = get_twist(&c);
                }
                corner_multiply(&c, &move_cube[j]); // 4. faceturn restores
            }
        }
    }
    {
		CubieCube c;

		//printf("coordcube.c: coordcube_init(): 2\n");
        for (i = 0; i < N_FLIP; i++) {
            set_flip(&c, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    edge_multiply(&c, &move_cube[j]);
                    flip_move[i][j * 3 + k] = get_flip(&c);
                }
                edge_multiply(&c, &move_cube[j]);
            }
        }
    }
    {
		CubieCube c;

		//printf("coordcube.c: coordcube_init(): 3\n");
        for (i = 0; i < N_FRtoBR; i++) {
			set_FRtoBR(&c, i);
            for (j = 0; j < 6; j++) {
				for (k = 0; k < 3; k++) {
                    edge_multiply(&c, &move_cube[j]);
                    FRtoBR_move[i][j * 3 + k] = get_FRtoBR(&c);
                }
                edge_multiply(&c, &move_cube[j]);
            }
        }
    }
    {
        CubieCube c;

		//printf("coordcube.c: coordcube_init(): 4\n");
        for (i = 0; i < N_URFtoDLF; i++) {
            set_URFtoDLF(&c, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    corner_multiply(&c, &move_cube[j]);
                    URFtoDLF_move[i][j * 3 + k] = get_URFtoDLF(&c);
                }
                corner_multiply(&c, &move_cube[j]);
            }
        }
    }
	{
		CubieCube c;

		//printf("coordcube.c: coordcube_init(): 5\n");
		for (i = 0; i < N_URtoDF; i++) {
			set_URtoDF(&c, i);
			for (j = 0; j < 6; j++) {
				for (k = 0; k < 3; k++) {
					edge_multiply(&c, &move_cube[j]);
					URtoDF_move[i][j * 3 + k] = get_URtoDF(&c);
					// Table values are only valid for phase 2 moves!
					// For phase 1 moves, casting to short is not possible.
				}
				edge_multiply(&c, &move_cube[j]);
			}
		}
	}
	{
		CubieCube c;
		
		//printf("coordcube.c: coordcube_init(): 6\n");
		for (i = 0; i < N_URtoUL; i++) {
			set_URtoUL(&c, i);
			for (j = 0; j < 6; j++) {
				for (k = 0; k < 3; k++) {
					edge_multiply(&c, &move_cube[j]);
					URtoUL_move[i][j * 3 + k] = get_URtoUL(&c);
				}
				edge_multiply(&c, &move_cube[j]);
			}
		}
	}
	{
		CubieCube c;

		//printf("coordcube.c: coordcube_init(): 7\n");
		for (i = 0; i < N_UBtoDF; i++) {
			set_UBtoDF(&c, i);
			for (j = 0; j < 6; j++) {
				for (k = 0; k < 3; k++) {
					edge_multiply(&c, &move_cube[j]);
					UBtoDF_move[i][j * 3 + k] = get_UBtoDF(&c);
				}
				edge_multiply(&c, &move_cube[j]);
			}
		}
	}
	{
		//printf("coordcube.c: coordcube_init(): 8\n");
		// for i, j <336 the six edges UR,UF,UL,UB,DR,DF are not in the
		// UD-slice and the index is <20160
		for (i = 0; i < 336; i++) {
			for (j = 0; j < 336; j++) {
				merge_URtoULandUBtoDF[i][j] = get_URtoDF_short(i, j);
			}
		}
	}
	{
		int depth = 0, done = 1;
		int parity, new_parity, URFtoDLF, new_URFtoDLF, slice, new_slice;

		//printf("coordcube.c: coordcube_init(): 9\n");
		for (i = 0; i < N_SLICE2_URFtoDLF_PARITY_2; i++) {
			slice_URFtoDLF_parity_prun[i] = -1;
		}
		set_pruning(slice_URFtoDLF_parity_prun, 0, 0);
		for (; done != N_SLICE2_URFtoDLF_PARITY; depth++) {
			for (i = 0; i < N_SLICE2_URFtoDLF_PARITY; i++) {
				parity = i % 2;
				URFtoDLF = (i / 2) / N_SLICE2;
				slice = (i / 2) % N_SLICE2;
				if (get_pruning(slice_URFtoDLF_parity_prun, i) == depth) {
					for (j = 0; j < 18; j++) {
						switch (j) {
						case 3:
						case 5:
						case 6:
						case 8:
						case 12:
						case 14:
						case 15:
						case 17:
							continue;
						default:
							new_slice = FRtoBR_move[slice][j];
							new_URFtoDLF = URFtoDLF_move[URFtoDLF][j];
							new_parity = parity_move[parity][j];
							if (get_pruning(slice_URFtoDLF_parity_prun, (N_SLICE2 * new_URFtoDLF + new_slice) * 2 + new_parity) == 0x0f) {
								set_pruning(slice_URFtoDLF_parity_prun, (N_SLICE2 * new_URFtoDLF + new_slice) * 2 + new_parity, depth + 1);
								done++;
							}
						}
					}
				}
			}
		}
	}
	{
		int depth = 0, done = 1;
		int parity, new_parity, URtoDF, new_URtoDF, slice, new_slice;

		//printf("coordcube.c: coordcube_init(): 10\n");
		for (i = 0; i < N_SLICE2_URtoDF_PARITY_2; i++) {
			slice_URtoDF_parity_prun[i] = -1;
		}
		set_pruning(slice_URtoDF_parity_prun, 0, 0);
		for (; done != N_SLICE2_URtoDF_PARITY; depth++) {
			for (i = 0; i < N_SLICE2_URtoDF_PARITY; i++) {
				parity = i % 2;
				URtoDF = (i / 2) / N_SLICE2;
				slice = (i / 2) % N_SLICE2;
				if (get_pruning(slice_URtoDF_parity_prun, i) == depth) {
					for (j = 0; j < 18; j++) {
						switch (j) {
						case 3:
						case 5:
						case 6:
						case 8:
						case 12:
						case 14:
						case 15:
						case 17:
							continue;
						default:
							new_slice = FRtoBR_move[slice][j];
							new_URtoDF = URtoDF_move[URtoDF][j];
							new_parity = parity_move[parity][j];
							if (get_pruning(slice_URtoDF_parity_prun, (N_SLICE2 * new_URtoDF + new_slice) * 2 + new_parity) == 0x0f) {
								set_pruning(slice_URtoDF_parity_prun, (N_SLICE2 * new_URtoDF + new_slice) * 2 + new_parity, depth + 1);
								done++;
							}
						}
					}
				}
			}
		}
	}
	{
		int depth, done, twist, new_twist, slice, new_slice;

		//printf("coordcube.c: coordcube_init(): 11\n");
		for (i = 0; i < N_SLICE1_TWIST_2 + 1; i++) {
			slice_twist_prun[i] = -1;
		}
		set_pruning(slice_twist_prun, 0, 0);
		for (depth = 0, done = 1; done != N_SLICE1_TWIST; depth++) {
			for (i = 0; i < N_SLICE1_TWIST; i++) {
				twist = i / N_SLICE1;
				slice = i % N_SLICE1;
				if (get_pruning(slice_twist_prun, i) == depth) {
					for (j = 0; j < 18; j++) {
						new_slice = FRtoBR_move[slice * 24][j] / 24;
						new_twist = twist_move[twist][j];
						if (get_pruning(slice_twist_prun, N_SLICE1 * new_twist + new_slice) == 0x0f) {
							set_pruning(slice_twist_prun, N_SLICE1 * new_twist + new_slice, depth + 1);
							done++;
						}
					}
				}
			}
		}
	}
	{
		int depth, done, flip, new_flip, slice, new_slice;

		//printf("coordcube.c: coordcube_init(): 12\n");
		for (i = 0; i < N_SLICE1_FLIP_2; i++) {
			slice_flip_prun[i] = -1;
		}
		set_pruning(slice_flip_prun, 0, 0);
		for (depth = 0, done = 1; done != N_SLICE1_FLIP; depth++) {
			for (i = 0; i < N_SLICE1_FLIP; i++) {
				flip = i / N_SLICE1;
				slice = i % N_SLICE1;
				if (get_pruning(slice_flip_prun, i) == depth) {
					for (j = 0; j < 18; j++) {
						new_slice = FRtoBR_move[slice * 24][j] / 24;
						new_flip = flip_move[flip][j];
						if (get_pruning(slice_flip_prun, N_SLICE1 * new_flip + new_slice) == 0x0f) {
							set_pruning(slice_flip_prun, N_SLICE1 * new_flip + new_slice, depth + 1);
							done++;
						}
					}
				}
			}
		}
	}
#if 0
    { // TEST BLOCK
        FILE *fp;

        printf("coordcube.c: coordcube_init(): TEST 1\n");
        fp = fopen("test01.c.txt", "w");
        for (i = 0; i < N_TWIST; i++) { // 1
            for (j = 0; j < N_MOVE; j++) {
                fprintf(fp, "%d\n", twist_move[i][j]);
            }
        }
        fclose(fp);

        printf("coordcube.c: coordcube_init(): TEST 2\n");
        fp = fopen("test02.c.txt", "w");
        for (i = 0; i < N_FLIP; i++) { // 2
            for (j = 0; j < N_MOVE; j++) {
                fprintf(fp, "%d\n", flip_move[i][j]);
            }
        }
        fclose(fp);
        
        printf("coordcube.c: coordcube_init(): TEST 3\n");
        fp = fopen("test03.c.txt", "w");
        for (i = 0; i < 2; i++) { // 3
            for (j = 0; j < 18; j++) {
                fprintf(fp, "%d\n", parity_move[i][j]);
            }
        }
        fclose(fp);
        
        printf("coordcube.c: coordcube_init(): TEST 4\n");
        fp = fopen("test04.c.txt", "w");
        for (i = 0; i < N_FRtoBR; i++) { // 4
            for (j = 0; j < N_MOVE; j++) {
                fprintf(fp, "%d\n", FRtoBR_move[i][j]);
            }
        }
        fclose(fp);
        
        printf("coordcube.c: coordcube_init(): TEST 5\n");
        fp = fopen("test05.c.txt", "w");
        for (i = 0; i < N_URFtoDLF; i++) { // 5
            for (j = 0; j < N_MOVE; j++) {
                fprintf(fp, "%d\n", URFtoDLF_move[i][j]);
            }
        }
        fclose(fp);
        
        printf("coordcube.c: coordcube_init(): TEST 6\n");
        fp = fopen("test06.c.txt", "w");
        for (i = 0; i < N_URtoDF; i++) { // 6
            for (j = 0; j < N_MOVE; j++) {
                fprintf(fp, "%d\n", URtoDF_move[i][j]);
            }
        }
        fclose(fp);
        
        printf("coordcube.c: coordcube_init(): TEST 7\n");
        fp = fopen("test07.c.txt", "w");
        for (i = 0; i < N_URtoUL; i++) { // 7
            for (j = 0; j < N_MOVE; j++) {
                fprintf(fp, "%d\n", URtoUL_move[i][j]);
            }
        }
        fclose(fp);
        
        printf("coordcube.c: coordcube_init(): TEST 8\n");
        fp = fopen("test08.c.txt", "w");
        for (i = 0; i < N_UBtoDF; i++) { // 8
            for (j = 0; j < N_MOVE; j++) {
                fprintf(fp, "%d\n", UBtoDF_move[i][j]);
            }
        }
        fclose(fp);
        
        printf("coordcube.c: coordcube_init(): TEST 9\n");
        fp = fopen("test09.c.txt", "w");
        for (i = 0; i < 336; i++) { // 9
            for (j = 0; j < 336; j++) {
                fprintf(fp, "%d\n", merge_URtoULandUBtoDF[i][j]);
            }
        }
        fclose(fp);
        
        printf("coordcube.c: coordcube_init(): TEST 10\n");
        fp = fopen("test10.c.txt", "w");
        for (i = 0; i < N_SLICE2_URFtoDLF_PARITY_2; i++) { // 10
            fprintf(fp, "%d\n", slice_URFtoDLF_parity_prun[i]);
        }
        fclose(fp);
        
        printf("coordcube.c: coordcube_init(): TEST 11\n");
        fp = fopen("test11.c.txt", "w");
        for (i = 0; i < N_SLICE2_URtoDF_PARITY_2; i++) { // 11
            fprintf(fp, "%d\n", slice_URtoDF_parity_prun[i]);
        }
        fclose(fp);
        
        printf("coordcube.c: coordcube_init(): TEST 12\n");
        fp = fopen("test12.c.txt", "w");
        for (i = 0; i < N_SLICE1_TWIST_2 + 1; i++) { // 12
            fprintf(fp, "%d\n", slice_twist_prun[i]);
        }
        fclose(fp);
        
        printf("coordcube.c: coordcube_init(): TEST 13\n");
        fp = fopen("test13.c.txt", "w");
        for (i = 0; i < N_SLICE1_FLIP_2; i++) { // 13
            fprintf(fp, "%d\n", slice_flip_prun[i]);
        }
        fclose(fp);
        
        exit(1);
    } // END OF TEST BLOCK
#endif
} // end of function coordcube_init()

// Set pruning value in table. Two values are stored in one byte.
void set_pruning(char *table, int index, char value) {
	if ((index % 2) == 0) {
		table[index / 2] &= 0xf0 | value;
	} else {
		table[index / 2] &= 0x0f | (value << 4);
	}
}

// Extract pruning value
char get_pruning(char *table, int index) {
	if ((index % 2) == 0) {
		return table[index / 2] & 0x0f;
	} else {
		return ((table[index / 2] & 0xf0) >> 4) & 0x0f;
	}
}


