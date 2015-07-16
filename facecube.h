/**
 * facecube.h
 */

#ifndef __TPD_FACECUBE__
#define __TPD_FACECUBE__

typedef struct _TAG_FACECUBE {
	char f[54];
} FaceCube;

#include "cubiecube.h"

extern char corner_facelet[8][3];
extern char edge_facelet[12][2];
extern char corner_color[8][3];
extern char edge_color[12][2];

extern void facecube_construct(FaceCube *fc);
extern void facecube_construct_str(FaceCube *fc, char *cube_str);
extern void to_string(FaceCube *fc, char *s);
extern void to_cubiecube(FaceCube *fc, CubieCube *ccret);

#endif


