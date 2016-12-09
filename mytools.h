/**
 * mytools.h
 */

#ifndef __TPD_MYTOOLS__
#define __TPD_MYTOOLS__

void mt_init(unsigned long s);
int next_int(int max);

extern int mytools_verify(char *s);

extern void random_cube(char *des);
extern void random_cube_with_parity(char *des);
extern void random_cube_with_no_parity(char *des);
extern void edge_random_cube(char *des);
extern void corner_random_cube(char *des);
extern void yy_cube_test1(char *des);
extern void yy_cube_test2(char *des);

extern void reverse_alg(char *src, char *des);

#endif


