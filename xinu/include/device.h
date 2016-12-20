/* device.h - isbaddev */
#ifdef __cplusplus
extern "C" {
#endif

#define	DEVNAMLEN	16	/* maximum size of a device name	*/

/* Macro used to verify device ID is valid  */

#define isbaddev(f)  ( ((f) < 0) | ((f) >= NDEVS) )

#ifdef __cplusplus
} //end extern "C"
#endif

