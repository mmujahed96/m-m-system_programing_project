#include <stdio.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/stat.h>           // for the inode 
#include <stdlib.h> 
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>
#include <signal.h>


#define IE_SIZE (sizeof(struct inotify_event))
#define BUFF (1024 * (IE_SIZE))

struct watch{

	int fd; //file descriptor
	int wd; //watch description
};

typedef enum {false, true} bool;
 
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

static struct watch *root;
static bool verbose = true;
void *watcher(struct watch o_w);
static bool run = true;

void *watchers();
void ctrl_c_handler(int, struct watch o_w);

int main(int argc, char *argv[])
{
	struct watch o_w;
	FILE *fp;
	
	//root = (struct watch *) malloc(sizeof(struct watch));

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

void *watch(struct watch o_w)
{
	char buffer[BUFF];
	int size;
	int i = 0;

	size = read(o_w.fd, buffer, BUFF);
	
	while(i < size)
	{
		struct inotify_event *p = (struct inotify_event*)&buffer[i];
		
		if(p -> len)
		{
			int masking = (p -> mask&IN_CREATE || p-> mask&IN_MODIFY || p-> mask&IN_DELETE || p-> mask&IN_DELETE_SELF);
	
		if(verbose)
		{
			if(p->mask&IN_MOVED_FROM || masking)
			{
				time_t t = time(NULL);
				struct tm *p = localtime(&t);

				char c[25];
				strftime(c, sizeof(c), "%c", p);	//%c date and time representation
				printf("{ %s }", c);

			}
			
			if(p -> mask&IN_CREATE)
			{
				printf("Created %s", p->name);

			}else if(p -> mask&IN_MODIFY)
			{
				printf("Modified");
			}else if(p -> mask&IN_MOVED_FROM)
			{
				printf("Renamed");
				
			}else if(p -> mask&IN_MOVED_TO)
			{
				printf("%s", p->name);
			}
			}else if(p -> mask&IN_DELETE)
			{
				printf("Deleted %s", p->name);
			}else if(p -> mask&IN_DELETE_SELF)
			{
				printf("The directory \"%s\" was deleted. \n ", p->name);
			}

			
			if(masking)
			{
				printf("{");
			}

			if(masking)
			{
				printf("}");
			}

			if(p-> mask&IN_MOVED_FROM)
			{
				printf("{");
			}

			if(p-> mask&IN_MOVED_FROM)
			{
				printf("}");
			}
			
			if(masking)
			{
				printf("%s", p-> name);
			}	

			if(p-> mask&IN_MOVED_FROM)
			{
				printf("%s", p->name);
			}
			
			if(masking)
			{
				printf("{");
				if(p-> mask&IN_ISDIR)
				{
					printf("Directory");
				}
				else
				{
					printf("File");
				}
				printf("}");
			}

			if(p-> mask&IN_MOVED_FROM)
			{
				printf("{");
				if (p-> mask&IN_ISDIR)
				{
					printf("Directory");
				}
				else
				{
					printf("File");
				}
				printf("}");
			}

		}

	}

}

void *watchers()
{
	while(1)
	{
		printf("I am threadFunction.\n");
	}
	pthread_cond_signal(&condition);
}

void ctrl_c_handler(int signum, struct watch w_o)
{
	printf(" Kill signal, exiting now");
	run = false;
	//removing from watch list
	(void)inotify_rm_watch(w_o.fd , w_o.wd);
}
