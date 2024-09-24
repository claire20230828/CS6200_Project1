#include "gfserver-student.h"
#include "gfserver.h"
#include "workload.h"
#include "content.h"
#include <stdlib.h>
#include <pthread.h> 
#include "steque.h"

//
//  The purpose of this function is to handle a get request
//
//  The ctx is a pointer to the "context" operation and it contains connection state
//  The path is the path being retrieved
//  The arg allows the registration of context that is passed into this routine.
//  Note: you don't need to use arg. The test code uses it in some cases, but
//        not in others.
//

/*
typedef struct client_info {
	gfcontext_t* context;
	char* path;
} client_info;
*/

extern pthread_mutex_t work_mutex;
extern pthread_cond_t work_cond;
extern steque_t *work_queue;

gfh_error_t gfs_handler(gfcontext_t **ctx, const char *path, void* arg){
	if (path == NULL) {
		printf("NULL path\n");
		return gfh_failure;
	}
	
	//create client_info and add to work_queue
	client_info *item = (client_info *)malloc(sizeof(client_info));
	item -> context = *ctx; 
	//copey path value, avoid later boss change path
	item-> path = (char *)malloc(strlen(path) + 1);
	strcpy(item->path, path);

	pthread_mutex_lock(&work_mutex); 
	steque_enqueue(work_queue, item); 
	pthread_cond_signal(&work_cond); 
	pthread_mutex_unlock(&work_mutex);
	//nullifies the original pointer
	*ctx = NULL;
	printf("Finish handler for %s\n", path);
	return gfh_success;
}

