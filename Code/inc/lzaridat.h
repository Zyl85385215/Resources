#ifndef __LZARI_DATA_H__
#define __LZARI_DATA_H__

/********** LZSS with multiple binary trees **********/

#define N        4096   /* size of ring buffer */
#define F          60   /* upper limit for match_length */
#define THRESHOLD   2   /* encode string into position and length
                           if match_length is greater than this */
#define NIL         N   /* index for root of binary search trees */


/********** Arithmetic Compression **********/

/*  If you are not familiar with arithmetic compression, you should read
        I. E. Witten, R. M. Neal, and J. G. Cleary,
            Communications of the ACM, Vol. 30, pp. 520-540 (1987),
    from which much have been borrowed.  */

#define M   15

/*  Q1 (= 2 to the M) must be sufficiently large, but not so
    large as the unsigned long 4 * Q1 * (Q1 - 1) overflows.  */

#define Q1  (1UL << M)
#define Q2  (2 * Q1)
#define Q3  (3 * Q1)
#define Q4  (4 * Q1)
#define MAX_CUM (Q1 - 1)

#define N_CHAR  (256 - THRESHOLD + F)
    /* character code = 0, 1, ..., N_CHAR - 1 */



#endif
