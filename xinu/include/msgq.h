/* msgq.h */

#ifdef __cplusplus
extern "C" {
#endif


struct mesgnode {
	uint32 msg;               /* a one work message         */
	struct mesgnode *mnext;   /* pointer next node in msg q */
};

typedef struct mesgq {
	sid32 msgssem;            /* send semaphore              */
	sid32 msgrsem;            /* receive semaphore           */
	uint16 msgmaxcnt;         /* max no of msg allowed in q  */
	mutx32 mqlock;            /* message queue lock          */
	struct mesgnode *mqueueh; /*  pointer to allocated list
				      required to free           */
	struct mesgnode *mfree;   /* list of free nodes          */
	struct mesgnode *mhead;   /* head of message queue       */
	struct mesgnode *mtail;   /* tail of message queue       */
} mesgq;

#ifdef __cplusplus
} //end extern "C"
#endif

