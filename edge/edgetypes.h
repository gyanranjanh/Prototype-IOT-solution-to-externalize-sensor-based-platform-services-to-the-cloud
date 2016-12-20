#ifndef __EDGETYPES_h__
#define __EDGETYPES_h__

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>	/* defines inet_ntoa */
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <semaphore.h> 

#define MAX_NO_TOPICS 256
#define MAX_NO_SUBSCRIBER 8

/* General type declarations used throughout the kernel */
typedef	unsigned char	byte;
typedef	byte	    bool;
typedef	uint8_t	    uint8;
typedef	int8_t		int32;
typedef	int16_t		int16;
typedef	uint32_t	uint32;
typedef	uint16_t	uint16;
typedef uint64_t    uint64;

/* Xinu-specific types */
typedef	int32	sid32;		/* semaphore ID				*/
typedef	int16	qid16;		/* jobqueue ID				*/
typedef	int32	pid32;		/* process ID				*/
typedef	int32	did32;		/* device ID				*/
typedef	int16	pri16;		/* process priority			*/
typedef	uint32	umsg32;		/* message passed among processes	*/
typedef	int32	bpid32;		/* buffer pool ID			*/
typedef	byte	bool8;		/* Boolean type				*/
typedef	uint32	intmask;	/* saved interrupt mask			*/
typedef	int32	ibid32;		/* index block ID (used in file system)	*/
typedef	int32	dbid32;		/* data block ID (used in file system)	*/
typedef	int32	uid32;		/* ID for UDP table descriptor		*/
typedef uint16  topic16;	/*type definition for topics*/

#define local	static		/* Local procedure or variable declar.	*/
typedef	int32	status;		/* returned status value (OK/SYSERR)	*/


/* Boolean constants */

#define FALSE	0		/* Boolean False			*/
#define TRUE	1		/* Boolean True				*/


/* Universal return constants */

#define OK	( 0)		/* normal system call return		*/
#define	SYSERR	(-1)		/* system call failed			*/
#define	TIMEOUT	(-3)		/* system call timed out		*/

extern	qid16	readylist;	/* global ID for list of ready processes*/

/* OS type */
typedef pthread_t mqttThread;
typedef pthread_mutex_t* mqttMutexHandle;
typedef sem_t mqttSemaHandle;

#define CREATE_THREAD pthread_create
#define DESTORY_THREAD pthread_cancel
#define EXIT_THREAD(p) pthread_exit(p)


/* Prototypes of I/O functions used throughout the kernel */
/*--------------------------------Local data---------------------------*/
#define INVALID_HANDLER   (NULL)
#define INVALID_BBBID     (2147483647)
#define WILDCARD_GP       (0)

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE 
#define FALSE 0
#endif

#define MQTT_ASSERT assert

#endif
