/**
 * search.c
 * 
 * Class Search implements the Two-Phase-Algorithm.
 * non-static ver.
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"
#include "coordcube.h"

// generate the solution string from the array data
void solution_tostring(Search *sc, int length, char *des)
{
	int i, j;

	for (i = 0, j = 0; j < length; j++) {
		switch (sc->ax[j]) {
			case 0:
				des[i++] = 'U'; break;
			case 1:
				des[i++] = 'R'; break;
			case 2:
				des[i++] = 'F'; break;
			case 3:
				des[i++] = 'D'; break;
			case 4:
				des[i++] = 'L'; break;
			case 5:
				des[i++] = 'B'; break;
		}
		switch (sc->po[j]) {
			case 1:
				des[i++] = ' '; break;
			case 2:
				des[i++] = '2'; break;
			case 3:
				des[i++] = '\''; break;
		}
		des[i++] = ' ';
	}
	des[i] = '\0';
}

/**
 * Computes the solver string for a given cube.
 * 
 * @param facelets
 *          is the cube definition string, see {@link Facelet} for the format.
 * 
 * @param maxDepth
 *          defines the maximal allowed maneuver length. For random cubes, a maxDepth of 21 usually will return a
 *          solution in less than 0.5 seconds. With a maxDepth of 20 it takes a few seconds on average to find a
 *          solution, but it may take much longer for specific cubes.
 * 
 *@param timeOut
 *          defines the maximum computing time of the method in seconds. If it does not return with a solution, it returns with
 *          an error code.
 * 
 * @param useSeparator
 *          determines if a " . " separates the phase1 and phase2 parts of the solver string like in F' R B R L2 F .
 *          U2 U D for example.<br>
 * @return The solution string or an error code:<br>
 *         Error 1: There is not exactly one facelet of each colour<br>
 *         Error 2: Not all 12 edges exist exactly once<br>
 *         Error 3: Flip error: One edge has to be flipped<br>
 *         Error 4: Not all corners exist exactly once<br>
 *         Error 5: Twist error: One corner has to be twisted<br>
 *         Error 6: Parity error: Two corners or two edges have to be exchanged<br>
 *         Error 7: No solution exists for the given maxDepth<br>
 *         Error 8: Timeout, no solution within given time
 */
extern void solution(Search *sc, char *facelets, int max_depth, clock_t timeout, int use_separator, char *des)
{
	int s, i;
	int count[6] = {0};
	FaceCube fc;
	CubieCube cc;
	CoordCube c;
	time_t tstart;
	int mv = 0, n = 0;
	int busy = 0;
	int depth_phase1 = 1;

	// ++++ check for wrong input ++++
	for (i = 0; i < 54; i++) {
		switch (facelets[i]) {
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
				strcpy(des, "Error 1");				
				return;
		}
	}
	for (i = 0; i < 6; i++) {
		if (count[i] != 9) {
			strcpy(des, "Error 1");
			return;
		}
	}

	facecube_construct_str(&fc, facelets);
	to_cubiecube(&fc, &cc);
	if ((s = cubiecube_verify(&cc)) != 0) {
		sprintf(des, "Error %d", abs(s));
		return;
	}

	// ++++ initialization ++++
	coordcube_construct(&c, &cc);

	sc->po[0] = 0;
	sc->ax[0] = 0;
	sc->flip[0] = c.flip;
	sc->twist[0] = c.twist;
	sc->parity[0] = c.parity;
	sc->slice[0] = c.FRtoBR / 24;
	sc->URFtoDLF[0] = c.URFtoDLF;
	sc->FRtoBR[0] = c.FRtoBR;
	sc->URtoUL[0] = c.URtoUL;
	sc->UBtoDF[0] = c.UBtoDF;
	sc->min_distphase1[1] = 1; // else failure for depth=1, n=0

	tstart = clock();

	// ++++ Main loop ++++
	do {
		do {
			if ((depth_phase1 - n > sc->min_distphase1[n + 1]) && !busy) {
				if (sc->ax[n] == 0 || sc->ax[n] == 3) { // Initialize next move
					sc->ax[++n] = 1;
				} else {
					sc->ax[++n] = 0;
				}
				sc->po[n] = 1;
			} else if (++(sc->po[n]) > 3) {
				do { // increment axis
					if (++(sc->ax[n]) > 5) {
						if (clock() - tstart > (timeout << 10)) {
							strcpy(des, "Error 8");
							return;
						}
						if (n == 0) {
							if (depth_phase1 >= max_depth) {
								strcpy(des, "Error 7");
								return;
							} else {
								depth_phase1++;
								sc->ax[n] = 0;
								sc->po[n] = 1;
								busy = 0;
								break;
							}
						} else {
							n--;
							busy = 1;
							break;
						}
					} else {
						sc->po[n] = 1;
						busy = 0;
					}
				} while (n != 0 && (sc->ax[n - 1] == sc->ax[n] || sc->ax[n - 1] - 3 == sc->ax[n]));
			} else {
				busy = 0;
			}
		} while (busy);

		// ++++ compute new coordinates and new minDistPhase1 ++++
		// if min_distphase1 = 0, the H subgroup is reached
		mv = 3 * sc->ax[n] + sc->po[n] - 1;
		sc->flip[n + 1] = flip_move[sc->flip[n]][mv];
		sc->twist[n + 1] = twist_move[sc->twist[n]][mv];
		sc->slice[n + 1] = FRtoBR_move[sc->slice[n] * 24][mv] / 24;
		sc->min_distphase1[n + 1] = MAX(
			get_pruning(slice_flip_prun, N_SLICE1 * sc->flip[n + 1] + sc->slice[n + 1]),
			get_pruning(slice_twist_prun, N_SLICE1 * sc->twist[n + 1] + sc->slice[n + 1])
		);

		if (sc->min_distphase1[n + 1] == 0 && n >= depth_phase1 - 5) {
			sc->min_distphase1[n + 1] = 10; // instead of 10 any value >5 is possible
			if (n == depth_phase1 - 1 && (s = total_depth(sc, depth_phase1, max_depth)) >= 0) {
				if (s == depth_phase1 || (sc->ax[depth_phase1 - 1] != sc->ax[depth_phase1] &&
                    sc->ax[depth_phase1 - 1] != sc->ax[depth_phase1] + 3)) {
					solution_tostring(sc, s, des);
					return;
				}
			}
		}
	} while (1);
}


// Apply phase2 of algorithm and return the combined phase1 and phase2 depth. In phase2, only the moves
// U,D,R2,F2,L2 and B2 are allowed.
int total_depth(Search *sc, int depth_phase1, int max_depth)
{
	int mv = 0, d1 = 0, d2 = 0, i;
	int max_depthphase2 = MIN(10, max_depth - depth_phase1); // Allow only max 10 moves in phase2
	int depth_phase2, n, busy;

	for (i = 0; i < depth_phase1; i++) {
		mv = 3 * sc->ax[i] + sc->po[i] - 1;
		sc->URFtoDLF[i + 1] = URFtoDLF_move[sc->URFtoDLF[i]][mv];
		sc->FRtoBR[i + 1] = FRtoBR_move[sc->FRtoBR[i]][mv];
		sc->parity[i + 1] = parity_move[sc->parity[i]][mv];
	}

	if ((d1 = get_pruning(slice_URFtoDLF_parity_prun, (N_SLICE2 * sc->URFtoDLF[depth_phase1] + sc->FRtoBR[depth_phase1]) * 2 + sc->parity[depth_phase1])) > max_depthphase2) {
		return -1;
	}

	for (i = 0; i < depth_phase1; i++) {
		mv = 3 * sc->ax[i] + sc->po[i] - 1;
		sc->URtoUL[i + 1] = URtoUL_move[sc->URtoUL[i]][mv];
		sc->UBtoDF[i + 1] = UBtoDF_move[sc->UBtoDF[i]][mv];
	}
	sc->URtoDF[depth_phase1] = merge_URtoULandUBtoDF[sc->URtoUL[depth_phase1]][sc->UBtoDF[depth_phase1]];

	if ((d2 = get_pruning(slice_URtoDF_parity_prun, (N_SLICE2 * sc->URtoDF[depth_phase1] + sc->FRtoBR[depth_phase1]) * 2 + sc->parity[depth_phase1])) > max_depthphase2) {
		return -1;
	}

	if ((sc->min_distphase2[depth_phase1] = MAX(d1, d2)) == 0) { // already solved
		return depth_phase1;
	}

	// now set up search
	depth_phase2 = 1;
	n = depth_phase1;
	busy = 0;
	sc->po[depth_phase1] = 0;
	sc->ax[depth_phase1] = 0;
	sc->min_distphase2[n + 1] = 1; // else failure for depthPhase2=1, n=0
	// ++++ end initialization ++++
	do {
		do {
			if ((depth_phase1 + depth_phase2 - n > sc->min_distphase2[n + 1]) && !busy) {
				if (sc->ax[n] == 0 || sc->ax[n] == 3) { // Initialize next move
					sc->ax[++n] = 1;
					sc->po[n] = 2;
				} else {
					sc->ax[++n] = 0;
					sc->po[n] = 1;
				}
			} else if ((sc->ax[n] == 0 || sc->ax[n] == 3) ? (++(sc->po[n]) > 3) : ((sc->po[n] = sc->po[n] + 2) > 3)) {
				do { // increment axis
					if (++(sc->ax[n]) > 5) {
						if (n == depth_phase1) {
							if (depth_phase2 >= max_depthphase2) {
								return -1;
							} else {
								depth_phase2++;
								sc->ax[n] = 0;
								sc->po[n] = 1;
								busy = 0;
								break;
							}
						} else {
							n--;
							busy = 1;
							break;
						}
					} else {
						if (sc->ax[n] == 0 || sc->ax[n] == 3) {
							sc->po[n] = 1;
						} else {
							sc->po[n] = 2;
						}
						busy = 0;
					}
				} while (n != depth_phase1 && (sc->ax[n - 1] == sc->ax[n] || sc->ax[n - 1] - 3 == sc->ax[n]));
			} else {
				busy = 0;
			}
		} while (busy);
		// +++++++++++++ compute new coordinates and new minDist ++++++++++
		mv = 3 * sc->ax[n] + sc->po[n] - 1;

		sc->URFtoDLF[n + 1] = URFtoDLF_move[sc->URFtoDLF[n]][mv];
		sc->FRtoBR[n + 1] = FRtoBR_move[sc->FRtoBR[n]][mv];
		sc->parity[n + 1] = parity_move[sc->parity[n]][mv];
		sc->URtoDF[n + 1] = URtoDF_move[sc->URtoDF[n]][mv];
		sc->min_distphase2[n + 1] = MAX(
			get_pruning(slice_URtoDF_parity_prun, (N_SLICE2 * sc->URtoDF[n + 1] + sc->FRtoBR[n + 1]) * 2 + sc->parity[n + 1]),
			get_pruning(slice_URFtoDLF_parity_prun, (N_SLICE2 * sc->URFtoDLF[n + 1] + sc->FRtoBR[n + 1]) * 2 + sc->parity[n + 1])
		);
	} while (sc->min_distphase2[n + 1] != 0);

	return depth_phase1 + depth_phase2;
}


