/*
 *  This file is for use by students to define anything they wish.  It is used by the gf server implementation
 */
#ifndef __GF_SERVER_STUDENT_H__
#define __GF_SERVER_STUDENT_H__

#include "gf-student.h"
#include "gfserver.h"
#include "content.h"

typedef struct client_info {
	gfcontext_t* context;
	char* path;
} client_info;

void init_threads(size_t numthreads);
void cleanup_threads();

#endif // __GF_SERVER_STUDENT_H__
