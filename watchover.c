#include <stdio.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/stat.h>           // for the inode 
#include <stdlib.h> 
#include <errno.h>
#include <pthread.h>
#include <signal.h>

typedef enum {true, false} bool;


void *watchers();
void ctrl_c_handler(int);
struct watch{

	int fd; //file descriptor
	int wd; //watch description
};

static bool run = true;
 
/*	A mutex is a MUTual EXclusion device, and is useful for protecting shared data structures from 
 *	concurrent modifications, and implementing critical sections and monitors.
 *
 *	A mutex has two possible states: unlocked (not owned by any thread), and locked (owned by one 
 *	thread). A mutex can never be owned by two different threads simultaneously. A thread attempting
 *	to lock a mutex that is already locked by another thread is suspended until the owning thread 
 *	unlocks the mutex 
 *	first.
 */
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

/*	device that allows threads to suspend execution and relinquish the processors until some 
 *	predicate on shared data is satisfied. The basic operations on conditions are: signal the 
 *	condition (when the 
 *	predicate becomes true), and wait for the condition, suspending the thread execution until 
 *	another thread signals the condition.
 */
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[])
{
	struct watch o_w;
	FILE *fp;
	int ret;
	pthread_t id;
	
	/*	
	if(argc == 1)
	{
		perror("No directory to watch over");
		exit(1);
	}
	*/
	fp = fopen(argv[1], "r");

	o_w.fd = inotify_init();	//create inotify for file descriptor


	if( pthread_create(&id, NULL, watchers, NULL)){
		fprintf(stderr, "Failed Creating watcher thread!");
	}

	if( o_w.fd == -1 )
	{
		perror("Failed to initialise, bye");
	}

	o_w.wd = inotify_add_watch(o_w.fd, argv[1], IN_ALL_EVENTS);

	if(o_w.wd < 0)
	{
		perror("Failed to initialise to watch file descriptor");
		exit(1);
	}
	signal(SIGINT, ctrl_c_handler); //handle INT
	while(run){
	pthread_cond_wait(&condition, &mut);
	}

	fclose(fp);

	return 0;
}

void *watchers()
{
	while(1)
	{
		printf("I am threadFunction.\n");
	}
	pthread_cond_signal(&condition);
}	
void ctrl_c_handler(int signum)
{
	printf(" Kill signal, exiting now");
	run = false;
	//removing from watch list
	(void)inotify_rm_watch(root-> root->wd);


}
