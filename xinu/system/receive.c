/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	restore(mask);
	return msg;
}

/*-----------------------------------------------------------------------
 * receiveMsg - Receive a message from any sender. It causes the calling
 * process to wait for a message to be sent (by any process). When a me-
 * ssage is sent, it is received and returned.	
 *-----------------------------------------------------------------------
 */
umsg32 receiveMsg (void)
{
	intmask mask;                  /* saved interrupt mask           */
	umsg32 msg;                    /* msg to return                  */
	char buf[100];                 /* log buf                        */
	struct procent *prptr;         /* ID of receipient process       */
	struct mesgnode *node, *mhead; /* pointer to msgnodes            */

	mask  = disable();
	prptr = &proctab[currpid];

	if (wait(prptr->msgq.msgrsem) == SYSERR) {
		restore(mask);
		return SYSERR;
	}

	/* deque message from queue head */
	mhead = prptr->msgq.mhead;
	if (mhead == NULL) {
		sprintf(buf, "Process %d is attempting to deque message from empty message queue\n", currpid);
		panic(buf);
	}
	node = mhead;
	if (node->mnext == NULL) {
		prptr->msgq.mhead = prptr->msgq.mtail = NULL;
	}
	else {
		prptr->msgq.mhead = node->mnext;
	}

	/* copy msg to return */
	msg = node->msg;
	/* return msg node to free list */
	node->mnext   = prptr->msgq.mfree;
	prptr->msgq.mfree = node;
	signal(prptr->msgq.msgssem);

	/* kprintf(" receiveMsg:> p:%d m:%u ", getpid(), msg); */

	restore(mask);
	return msg;	
}

/*-------------------------------------------------------------------
 * receiveMsgs - Receiving a group (msg_count) of messages (msgs). It
 * causes the calling process to wait for msg_count messages to be s-
 * ent. When all messages are in the queue, they are then all togeth-
 * er immediately received.	
 *-------------------------------------------------------------------
 */
syscall receiveMsgs(umsg32* msgs, uint32 msg_count)
{
	intmask mask;                  /* saved interrupt mask           */
	uint32 count = 0;              /* message iterator               */
	char buf[100];                 /* log buf                        */
	struct procent *prptr;         /* ID of receipient process       */
	struct mesgnode *node, *mhead; /* pointer to msgnodes            */

	mask  = disable();
	prptr = &proctab[currpid];

	while(count < msg_count) {
		if (wait(prptr->msgq.msgrsem) == SYSERR) {
			restore(mask);
			return SYSERR;
		}

		/* deque message from queue head */
		mhead = prptr->msgq.mhead;
		if (mhead == NULL) {
			sprintf(buf, "Process %d is attempting to deque message from empty message queue\n", currpid);
			panic(buf);
		}
		node = mhead;
		if (node->mnext == NULL) {
			prptr->msgq.mhead = prptr->msgq.mtail = NULL;
		}
		else {
			prptr->msgq.mhead = node->mnext;
		}

		/* copy msg to return */
		msgs[count]  = node->msg;
		/* return msg node to free list */
		node->mnext   = prptr->msgq.mfree;
		prptr->msgq.mfree = node;
		signal(prptr->msgq.msgssem);
		count++;
	}

	/* kprintf(" receiveMsgs:> p:%d", getpid());
	for (count = 0; count < msg_count; count++) {
		kprintf(" m%u:%u", count + 1, msgs[count]);
	} */

	restore(mask);
	return OK;	
}
