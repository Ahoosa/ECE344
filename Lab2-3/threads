#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ucontext.h>
#include "thread.h"
#include "interrupt.h"

typedef enum states{ running, ready, exited, killed,empty,blocked,exitedSleep }states;

bool itsemp;
bool itsDone;
struct wait_queue *wait_queue_create();
struct block
{
    int myTid;
    struct block *next;
};

/* This is the wait queue structure */
struct wait_queue {
	struct block *head;
	struct block *tail;
};

/* This is the thread control block */
struct thread {
	Tid id;
	states state;
	ucontext_t* currContext;
	void* stack;
	struct wait_queue * wq;
	
};

struct thread* allThreads[THREAD_MAX_THREADS];


struct thread* runningT;



int size=0;
struct block *exHead = NULL;
struct block *exCurr = NULL;
struct block *readyHead = NULL;
struct block *readyCurr = NULL;

void deleteIt2(){
	struct block *curr = exHead;
	struct block *next;
	if(curr!=NULL){
		while(curr!=NULL && next!=NULL){
			next=curr->next;	
			free(curr);		
			curr=next;
			exHead=curr;
		}
	free(curr);
	exHead=exCurr=NULL;
	}
	
	for(int i=0;i<THREAD_MAX_THREADS;++i){
		if(allThreads[i]->stack!=NULL && (allThreads[i]->state==killed || allThreads[i]->state==exited)){
			free(allThreads[i]->stack);
			allThreads[i]->state=empty;
			//free(allThreads[i]->currContext); 
		}
	}
	
}
void deleteIt(){
	struct block *curr = readyHead;
	struct block *next;

	while(curr!=NULL && next!=NULL){
		next=curr->next;
		free(curr);
		curr=next;
	}
	free(curr);

	readyHead=readyCurr=NULL;
}
struct block* create_list2(int val)
{
 
    struct block *ptr = (struct block*)malloc(sizeof(struct block));
    if(NULL == ptr)
    {
        return NULL;
    }
    ptr->myTid = val;
    ptr->next = NULL;

    exHead = exCurr = ptr;
    return ptr;
}

struct block* add_to_list2(int val)
{
    if(NULL == exHead)
    {
        return (create_list2(val));
    }

    struct block *ptr = (struct block*)malloc(sizeof(struct block));
    if(NULL == ptr)
    {
        return NULL;
    }
    ptr->myTid = val;
    ptr->next = NULL;

    exCurr->next = ptr;
    exCurr = ptr;
    
    return ptr;
}

struct block* search_in_list2(int val, struct block **prev)
{
    struct block *ptr = exHead;
    struct block *tmp = NULL;
    bool found = false;

    while(ptr != NULL)
    {
        if(ptr->myTid == val)
        {
            found = true;
            break;
        }
        else
        {
            tmp = ptr;
            ptr = ptr->next;
        }
    }

    if(true == found)
    {
        if(prev)
            *prev = tmp;
        return ptr;
    }
    else
    {
        return NULL;
    }
}

int delete_from_list2(int val)
{
    struct block *prev = NULL;
    struct block *del = NULL;


    del = search_in_list2(val,&prev);
    if(del == NULL)
    {
        return -1;
    }
    else
    {
        if(prev != NULL)
            prev->next = del->next;

        if(del == exCurr)
        {
            exCurr = prev;
        }
         if(del == exHead)
        {
            exHead = del->next;
        }
    }

    free(del);
    del = NULL;

    return 0;
}

struct block* create_list(int val)
{
 
    struct block *ptr = (struct block*)malloc(sizeof(struct block));
    if(NULL == ptr)
    {
        return NULL;
    }
    ptr->myTid = val;
    ptr->next = NULL;

    readyHead = readyCurr = ptr;
    return ptr;
}

struct block* add_to_list(int val)
{
    if(readyHead==NULL || size==0)
    {
        return (create_list(val));
    }

    struct block *ptr = (struct block*)malloc(sizeof(struct block));
    if(NULL == ptr)
    {
        return NULL;
    }
    ptr->myTid = val;
    ptr->next = NULL;

    readyCurr->next = ptr;
    readyCurr = ptr;
    
    return ptr;
}

struct block* search_in_list(int val, struct block **prev)
{
    struct block *ptr = readyHead;
    struct block *tmp = NULL;
    bool found = false;

    while(ptr != NULL)
    {
        if(ptr->myTid == val)
        {
            found = true;
            break;
        }
        else
        {
            tmp = ptr;
            ptr = ptr->next;
        }
    }

    if(true == found)
    {
        if(prev)
            *prev = tmp;
        return ptr;
    }
    else
    {
        return NULL;
    }
}

int delete_from_list(int val)
{
    struct block *prev = NULL;
    struct block *del = NULL;


    del = search_in_list(val,&prev);
    if(del == NULL)
    {
        return -1;
    }
    else
    {
        if(prev != NULL)
            prev->next = del->next;

        if(del == readyCurr)
        {
            readyCurr = prev;
        }
         if(del == readyHead)
        {
            readyHead = del->next;
        }
    }
   
    free(del);
    del = NULL;

    return 0;
}


void 
thread_stub(void (*thread_main)(void *), void *arg){
	//Tid ret;
	interrupts_on();
	thread_main(arg);


	thread_exit();
	
	
	exit(0);
}


void
thread_init(void)
{
	int enabled;
	enabled=interrupts_off();
	itsDone=false;
        itsemp=true;
	size=0;

	for(int i=1;i<THREAD_MAX_THREADS;++i){
		allThreads[i]=(struct thread*)malloc(sizeof(struct thread));
		allThreads[i]->id=i;
		allThreads[i]->state=empty;
		allThreads[i]->currContext = (struct ucontext_t*)malloc(sizeof(struct ucontext_t));
		allThreads[i]->wq=wait_queue_create();	
	}
	allThreads[0]=(struct thread*)malloc(sizeof(struct thread));
	allThreads[0]->currContext = (struct ucontext_t*)malloc(sizeof(struct ucontext_t));
	allThreads[0]->id=0;
	allThreads[0]->state=running;
	runningT=allThreads[0];
	allThreads[0]->stack=NULL;
	allThreads[0]->wq=wait_queue_create();	
	
	interrupts_set(enabled);
}

Tid
thread_id()
{

	if(runningT!=NULL) return runningT->id;
	return THREAD_INVALID;
}



Tid
thread_create(void (*fn) (void *), void *parg)
{
	int enabled;
	enabled=interrupts_off();
	int foundID=-1;
	for(int i=0;i<THREAD_MAX_THREADS;++i){
		if(allThreads[i]->state==exited || allThreads[i]->state==killed ){
			free(allThreads[i]->stack);
			allThreads[i]->state=empty;
		}
		if(allThreads[i]->state==empty){
			foundID=i;
			break;		
		}
			
	}
	if(foundID==-1) { interrupts_set(enabled);
  		return THREAD_NOMORE; 
	}
	
	int err=getcontext((allThreads[foundID]->currContext));
	assert(!err);
	allThreads[foundID]->id=foundID;
	allThreads[foundID]->state=ready;
	
	
	
	
	
	void *myStack=(void*)malloc(THREAD_MIN_STACK);
	

	if(myStack==NULL) {
		free(myStack);	
		allThreads[foundID]->state=empty;
		interrupts_set(enabled);
		return THREAD_NOMEMORY;
	}
	
	allThreads[foundID]->stack=myStack;
	add_to_list(foundID);
	size++;
	
	//manipulating the field in ucontext for the thread
	allThreads[foundID]->currContext->uc_mcontext.gregs[REG_RIP]=(long )thread_stub;
	allThreads[foundID]->currContext->uc_mcontext.gregs[REG_RSP]=(long )myStack+THREAD_MIN_STACK-8;
	allThreads[foundID]->currContext->uc_mcontext.gregs[REG_RBP]=(long)myStack;
	allThreads[foundID]->currContext->uc_mcontext.gregs[REG_RDI]=(long )fn;
	allThreads[foundID]->currContext->uc_mcontext.gregs[REG_RSI]=(long )parg;

	
	interrupts_set(enabled);
	return foundID;

	return THREAD_FAILED;
}

bool valid_thread(Tid tid) {
    if(tid == THREAD_SELF || tid == runningT->id || tid == THREAD_ANY)
        return true; 
    struct block* head = readyHead;
    while (head!=NULL) {
        if (head->myTid == tid)
            return true;
        head = head->next;
    }
    return false;
}
Tid
thread_yield(Tid want_tid)

{
	int enabled;
	enabled=interrupts_off();

    if(want_tid == THREAD_ANY && readyHead == NULL ) {
         interrupts_set(enabled);
        return THREAD_NONE;
    } else if (want_tid<-2 || want_tid> THREAD_MAX_THREADS ) {
        interrupts_set(enabled);
        return THREAD_INVALID;
    }

	if (want_tid == THREAD_SELF || want_tid == runningT->id) {
        interrupts_set(enabled);
        return runningT->id;
    } else if (want_tid == THREAD_ANY) {
     	   if(size==0 ){
      	      deleteIt2(); 
	    interrupts_set(enabled);
	    return THREAD_NONE;
	    }
     	   else{
		want_tid = readyHead->myTid;
	    }
	}
 

	if(size==0) {interrupts_set(enabled); return  THREAD_INVALID;}
	if(allThreads[want_tid]->state==exited) {  interrupts_set(enabled); return THREAD_INVALID;} //i changed this line
	if(allThreads[want_tid]->state!=ready) {interrupts_set(enabled); return THREAD_INVALID;}
    volatile int context_is_set = 0;
    int err = getcontext(runningT->currContext);
    assert(!err);
    if(context_is_set) {
	interrupts_set(enabled);
        return want_tid;
    }       

    runningT->state = ready;
    add_to_list(runningT->id);
    delete_from_list(want_tid);
    allThreads[want_tid]->state =  running;
    runningT=  allThreads[want_tid];
    context_is_set = 1;

    int err1 = setcontext(allThreads[want_tid]->currContext);
    assert(!err1);
    interrupts_set(enabled);
    return THREAD_FAILED;
}

void
thread_exit()//free exit add current to exit then set context 
{	
	int enabled;
	enabled=interrupts_off();

 	deleteIt2();


	thread_wakeup(allThreads[runningT->id]->wq,1);
	
	if(readyHead==NULL) {
		free(allThreads[runningT->id]->stack);
		allThreads[runningT->id]->state=empty;
		interrupts_set(enabled);
		exit(0);
	}
	
        allThreads[runningT->id]->state=exited;
 	add_to_list2(runningT->id);

	Tid nextT=readyHead->myTid;
	//printf("%d %d\n",nextT,runningT->id);
	//if(allThreads[nextT]->state==exited){ 
		
	//	add_to_list2(nextT); 
	//	deleteIt2();
	//	interrupts_set(enabled);
	//	exit(0);
	//}

	delete_from_list(nextT);
	size--;
	runningT=allThreads[nextT];
	allThreads[nextT]->state=running;

	if(size==0) itsDone=true;
	else itsDone=false;

	
	

	int err=setcontext(allThreads[nextT]->currContext);
	assert(!err);
	interrupts_set(enabled);
	
}

Tid
thread_kill(Tid tid)//just put in eixt q
{
	int enabled;
	enabled=interrupts_off();

	if(tid<0 || tid>THREAD_MAX_THREADS-1 || tid==runningT->id) {interrupts_set(enabled); return THREAD_INVALID;}
	if(allThreads[tid]==NULL || allThreads[tid]->state==killed || allThreads[tid]->state==exited ||  allThreads[tid]->state==empty) {interrupts_set(enabled); return THREAD_INVALID;}
		
		if( allThreads[tid]->state==blocked){	
			allThreads[tid]->state=exitedSleep;
			thread_wakeup(allThreads[tid]->wq,1);
		}
		
		else{
		thread_wakeup(allThreads[tid]->wq,1);
		delete_from_list(tid);
		size--;
		allThreads[tid]->state=exited;
		add_to_list2(tid);
		if(size==0){
			deleteIt();

		}
		}
		interrupts_set(enabled);
		return tid;

	return THREAD_FAILED;
}

/*******************************************************************
 * Important: The rest of the code should be implemented in Lab 3. *
 *******************************************************************/

void addWait(int tid,struct wait_queue *q){
	
	struct block *ptr = (struct block*)malloc(sizeof(struct block));
	ptr->myTid=tid;
	ptr->next=NULL;
	if(q->head==NULL){	
		q->head=q->tail=ptr;
		ptr->next=NULL;
		return;
	}
	
	q->tail->next=ptr;
	q->tail=ptr;
	ptr->next=NULL;
	
	return;
}

int deleteWait(int num,struct wait_queue *q){
	if(q->head!=NULL){
		struct block *curr =q->head;
		int tid=curr->myTid;
		if(curr==q->tail) {q->head=q->tail=NULL; free(curr); return tid;}
		if(curr->next==q->tail) {
			q->head=q->tail;
			free(curr); curr=NULL;
			return tid;
		}else{	
		q->head=curr->next;
		free(curr);
		curr=NULL;
		return tid;
		}
	}
	
	return -1;
}



/* make sure to fill the wait_queue structure defined above */
struct wait_queue *
wait_queue_create()
{
	int enabled=interrupts_off();
	struct wait_queue *wq;

	wq = malloc(sizeof(struct wait_queue));
	assert(wq);
	
	wq->head=wq->tail=NULL;
	
	
	interrupts_set(enabled);
	return wq;
}

void
wait_queue_destroy(struct wait_queue *wq)
{
	int enabled=interrupts_off();
	free(wq);
	interrupts_set(enabled);
}

Tid
thread_sleep(struct wait_queue *queue)
{
	int enabled=interrupts_off();
	assert(!interrupts_enabled());
	if(queue==NULL) {
		interrupts_set(enabled); 
		return THREAD_INVALID; 
	}
	if(readyHead==NULL ){
		interrupts_set(enabled);
	//	printf("ready is fucked\n");
		return THREAD_NONE;
	}
	
	// printf("%d Im sleeping\n",runningT->id);
	addWait(runningT->id,queue);
	Tid nextT=readyHead->myTid;
	delete_from_list(nextT);
	size--;
	volatile int set=0;
	int err = getcontext(runningT->currContext);
	assert(!err);
	
	
       
	allThreads[runningT->id]->state=blocked;
 	
	if(set) {

		interrupts_set(enabled);
		return allThreads[nextT]->id;
   	 }       
	else {		set=1;
		runningT=allThreads[nextT];
		allThreads[nextT]->state=running;
		int err2 = setcontext(runningT->currContext);
	 	assert(!err2);
		
		interrupts_set(enabled);
		return runningT->id;
	}
 	
	return nextT;
}

/* when the 'all' parameter is 1, wakeup all threads waiting in the queue.
 * returns whether a thread was woken up on not. */
int
thread_wakeup(struct wait_queue *queue, int all)
{
	
	int enabled=interrupts_off();
	assert(!interrupts_enabled());
	
	if(queue==NULL || queue->head==NULL) {
		interrupts_set(enabled);
		return 0; 
	}
	else if(all==0){
		int del=deleteWait(0,queue);
		int wake=del;
		
	
		if(allThreads[wake]->state==exitedSleep){
			allThreads[wake]->state=exited;
			add_to_list2(wake);
			deleteIt2();
		}else{
			add_to_list(wake);
			size++;
			allThreads[wake]->state=ready;	
		}
		interrupts_set(enabled);
		return 1;
	}else{
		int count=0;
		int del=deleteWait(0,queue);
		while(del!=-1){
			int wake=del;	
			if(allThreads[wake]->state==exitedSleep){
				allThreads[wake]->state=exited;
				add_to_list2(wake);
				
			}else{
				add_to_list(wake);
				allThreads[wake]->state=ready;
				size++;
			}
			del=deleteWait(0,queue);
			count++;
			}
		deleteIt2();
		interrupts_set(enabled);
		return count;

	}
	
	interrupts_set(enabled);
	return 0;
}

/* suspend current thread until Thread tid exits */
Tid
thread_wait(Tid tid)
{

	int enabled=interrupts_off();
	if(tid<0 || tid>THREAD_MAX_THREADS || tid==runningT->id || allThreads[tid]->state==empty || allThreads[tid]->state==killed || allThreads[tid]->state==exited || allThreads[tid]->state==exitedSleep){
		interrupts_set(enabled); 
		return THREAD_INVALID;
	}
	
	
	thread_sleep(allThreads[tid]->wq);
	
	allThreads[runningT->id]->state=running;
	
	interrupts_set(enabled);
	
	return tid;
	return 0;
}

struct lock {
	int taken; 
	struct wait_queue *lockQ;
 
};

struct lock *
lock_create()
{
	int enabled=interrupts_off();
	struct lock *lock;

	lock = malloc(sizeof(struct lock));
	assert(lock);

	lock->taken=0;
	lock->lockQ=wait_queue_create();

	interrupts_set(enabled);
	return lock;
}

void
lock_destroy(struct lock *lock)
{
	int enabled=interrupts_off();

	assert(lock != NULL);

	if(lock->taken==false){
		free(lock->lockQ);
		free(lock);
	}

	interrupts_set(enabled);
}

void
lock_acquire(struct lock *lock)
{
	int enabled=interrupts_off();
	assert(lock != NULL);

	while(lock->taken==1)
		thread_sleep(lock->lockQ);
	lock->taken=1;
	interrupts_set(enabled);
}

void
lock_release(struct lock *lock)
{
	int enabled=interrupts_off();
	assert(lock != NULL);

	if(lock->taken==1){
		lock->taken=0;
		thread_wakeup(lock->lockQ,1);
	}
	interrupts_set(enabled);
}

struct cv {
	struct wait_queue *cvQ;
};

struct cv *
cv_create()
{
	int enabled=interrupts_off();
	struct cv *cv;

	cv = malloc(sizeof(struct cv));
	assert(cv);

	cv->cvQ=wait_queue_create();

	interrupts_set(enabled);

	return cv;
}

void
cv_destroy(struct cv *cv)
{

	int enabled=interrupts_off();

	assert(cv != NULL);

	if(cv->cvQ==NULL) // or not
		free(cv);

	interrupts_set(enabled);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
	int enabled=interrupts_off();
	assert(cv != NULL);
	assert(lock != NULL);

	if(lock->taken==1){
		lock_release(lock);
		thread_sleep(cv->cvQ);
		lock_acquire(lock);
	}
	interrupts_set(enabled);
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
	
	int enabled=interrupts_off();

	assert(cv != NULL);
	assert(lock != NULL);

	if(lock->taken==1) thread_wakeup(cv->cvQ,0);

	interrupts_set(enabled);
}
	

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	int enabled=interrupts_off();
	assert(cv != NULL);
	assert(lock != NULL);

	if(lock->taken==1) thread_wakeup(cv->cvQ,1);

	interrupts_set(enabled);
}

