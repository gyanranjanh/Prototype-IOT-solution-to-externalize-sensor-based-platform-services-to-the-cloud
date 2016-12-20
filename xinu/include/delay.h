/* delay.h - DELAY */

/*
 * Delay units are in microseconds.
 */
 #ifdef __cplusplus
extern "C" {
#endif

#define	DELAY(n)                                	\
{                                               	\
	volatile long N = 2*n;				\
							\
	while(N > 0) {					\
		N--;					\
	}						\
}

/*
 * Delay units are in milli-seconds.
 */
#define	MDELAY(n)					\
{							\
	register long i;				\
							\
	for (i=n;i>0;i--) {				\
		DELAY(1000);				\
	}						\
}

#ifdef __cplusplus
} //end extern "C"
#endif

