/**
 * search.h
 */

#ifndef __TPD_SEARCH__
#define __TPD_SEARCH__

#include <time.h>

#define MAX(a,b) ( ((a) > (b))? (a) : (b) )
#define MIN(a,b) ( ((a) < (b))? (a) : (b) )

typedef struct _TAG_SEARCH {
	int ax[31]; // The axis of the move
	int po[31]; // The power of the move

	int flip[31]; // phase1 coordinates
	int twist[31];
	int slice[31];

	int parity[31]; // phase2 coordinates
	int URFtoDLF[31];
	int FRtoBR[31];
	int URtoUL[31];
	int UBtoDF[31];
	int URtoDF[31];

	int min_distphase1[31]; // IDA* distance do goal estimations
	int min_distphase2[31];
} Search;

void solution_tostring(Search *sc, int length, char *des);
extern void solution(Search *sc, char *facelets, int max_depth, clock_t timeout, int use_separator, char *des);
int total_depth(Search *sc, int depth_phase1, int max_depth);

#endif


