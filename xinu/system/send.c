/* send.c - send */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  _send_msg  -  Helper function to put (msg) in the message queue of the
 *  process pointed to by prptr. It waits on send semaphore before sending
 *  and signals receive semaphore of the process before returning
 *------------------------------------------------------------------------
 */
static uint32 _send_msg(struct procent *prptr, umsg32 msg)
{
	struct mesgnode *node, *tailnode;

	/* we return if we can no longer put messges into the queue
	 * because it is full */
	if (prptr == NULL || wait(prptr->msgq.msgssem) == SYSERR) {
		return SYSERR;
	}

	/* lock the message queue before initiating any operation on it  */
	mutex_acquire(prptr->msgq.mqlock);
	/* get a free node by unlinking */
	node = prptr->msgq.mfree;       /* Point to first free node      */
	if (node == NULL) {
		panic("_send_msg: no free message nodes in process's message queue\n");
	}
	prptr->msgq.mfree = node->mnext; /* Unlink from free list         */
	node->mnext = NULL;              /* Set fields in the node        */
	node->msg  = msg;

	/* Link into queue */
	tailnode = prptr->msgq.mtail;
	if (tailnode == NULL) {        /* if the message queue was empty */
		prptr->msgq.mhead = prptr->msgq.mtail = node;
	}
	else {                         /* insert new node at tail        */
		tailnode->mnext = node;
		prptr->msgq.mtail = node;
	}

	/* unlock message queue */
	mutex_release(prptr->msgq.mqlock);

	signal(prptr->msgq.msgrsem);

	return OK;
}

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

/*-------------------------------------------------------------------
 * sendMsg - Send a message (msg) to process (pid). In case a message
 * or a number of messages are already waiting on pid to receive them,
 * then new msg will be queued. It will return OK on success or SYSERR
 * on error.	
 *-------------------------------------------------------------------   
 */
syscall	sendMsg	(
		pid32 pid,      /* ID of receipient process             */
	       	umsg32 msg      /* Contents of message                  */
		)
{
	intmask mask; 		/* saved interrupt mask                 */
	struct procent *prptr;  /* Ptr to process's table entry         */

	mask = disable();

	/* kprintf(" sendMsg:>p:%d->p:%d m:%u ", getpid(), pid, msg); */

	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];

	if (_send_msg(prptr, msg) == SYSERR) {
		restore(mask);
		return SYSERR;
	}

	restore(mask); /* restore mask */
	return OK;
}

/*-------------------------------------------------- 
 * Sending a group (msg_count) of messages (msgs) to
 * process (pid). It will return the number of msgs
 * successfully sent or SYSERR on error
 * -------------------------------------------------
 */
uint32 sendMsgs ( 	
        	pid32 pid,	     /* ID of receipient process             */
	        umsg32* msgs,	     /* Message array                        */
		uint32 msg_count     /* No of messages                       */	
                )
{
	intmask mask; 		     /* saved interrupt mask                 */
	uint32 count = 0;            /* message iterator                     */
	struct procent *prptr;       /* ptr to process's table entry         */
	uint32 result = SYSERR;      /* return result                        */

	mask = disable();


	if (isbadpid(pid)) {
		restore(mask);
		return result;
	}

	if (msg_count == 0 || msgs == NULL) {
		restore(mask);
		return result;
	}

	kprintf(" sendMsgs:>");
	for (count = 0; count < msg_count; count++) {
		kprintf("  m%u:%u", count + 1, msgs[count]);
	}
	kprintf(" to p:%d ", pid);
	count = 0;

	prptr = &proctab[pid];

	while (count < msg_count) {
		if(_send_msg(prptr, *msgs) == SYSERR) {
			break;
		}

		count++;
		msgs++;
	}

	if (count != 0) {
		result = count;
	}
	else {
		kprintf("sendMsgs: Failed to send any msg\n");
	}
	

	restore(mask);
	return result;
}

/*--------------------------------------------------------- 
 * sendnMsg - Sending the message (msg) to the number (pid-
 * _count) of processes (pids). It will return the number
 * of pids successfully sent to or SYSERR on error
 * --------------------------------------------------------
 */
uint32	sendnMsg (
		 uint32 pid_count, /* number of processes to send the message to */
		 pid32* pids,      /* process id array */
		 umsg32 msg        /* message to send  */
		 )
{
	intmask mask; 		    	    /* saved interrupt mask                 */
	uint32 count = 0, pidcount = 0;     /* process iterator and process count   */
	pid32 pid;                          /* next process id in array             */
	struct procent *prptr;              /* ptr to process's table entry         */
	uint32 result = SYSERR;             /* return result                        */

	mask = disable();

	if (pid_count == 0 || pids == NULL) {
		restore(mask);
		return result;
	}

	kprintf(" sendnMsg:> m:%u to ", msg);
	for (count = 0; count < pid_count; count++) {
		kprintf(" p%u:%d", count + 1, pids[count]);
	}
	kprintf(" ");
	count = 0;

	while (count < pid_count) {
		pid = pids[count];

		if (isbadpid(pid)) {
			kprintf("sendnMsg: pid bad: %d ", pid);
			count++;
			continue;
		}

		prptr = &proctab[pid];
		if (_send_msg(prptr, msg) == SYSERR) {
			kprintf("sendnMsg: failed to send msg to pid: %d ", pid);
			count++;
			continue;
		}

		pidcount++;
		count++;
	}

	if (pidcount != 0) {
		result = pidcount;
	}

	restore(mask);
	return result;
}
