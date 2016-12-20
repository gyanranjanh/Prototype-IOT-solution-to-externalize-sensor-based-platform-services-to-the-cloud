/* ramdisk.h - definitions for a ram disk (for testing) */
#ifdef __cplusplus
extern "C" {
#endif

/* Ram disk block size */

#define	RM_BLKSIZ	512		/* block size			*/
#define	RM_BLKS		200		/* number of blocks		*/

struct	ramdisk	{
	char	disk[RM_BLKSIZ * RM_BLKS];
	};

extern	struct	ramdisk	Ram;

#ifdef __cplusplus
} //end extern "C"
#endif

