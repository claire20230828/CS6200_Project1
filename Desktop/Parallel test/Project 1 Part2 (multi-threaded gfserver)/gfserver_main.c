#include <stdlib.h>

#include "gfserver-student.h"
#include <pthread.h>
#include "steque.h"
#include <fcntl.h>


#define BUFSIZE 2048

#define USAGE                                                                                     \
  "usage:\n"                                                                                      \
  "  gfserver_main [options]\n"                                                                   \
  "options:\n"                                                                                    \
  "  -m [content_file]   Content file mapping keys to content files (Default: content.txt\n"      \
  "  -p [listen_port]    Listen port (Default: 17394)\n"                                          \
  "  -h                  Show this help message.\n"                                               \
  "  -t [nthreads]       Number of threads (Default: 16)\n"                                       \
  "  -d [delay]          Delay in content_get, default 0, range 0-5000000 "                       \
  "(microseconds)\n "

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
    {"delay", required_argument, NULL, 'd'},
    {"port", required_argument, NULL, 'p'},
    {"nthreads", required_argument, NULL, 't'},
    {"content", required_argument, NULL, 'm'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}};

extern unsigned long int content_delay;

extern gfh_error_t gfs_handler(gfcontext_t **ctx, const char *path, void *arg);

static void _sig_handler(int signo) {
  if ((SIGINT == signo) || (SIGTERM == signo)) {
    exit(signo);
  }
}

//declare Global resuorce***
pthread_mutex_t work_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t work_cond = PTHREAD_COND_INITIALIZER;
pthread_t * workers;
steque_t* work_queue;



void* worker_function(void* arg){
  //combine with previous code (transferserver/gfserver)
  client_info *item; 
  char buf[BUFSIZE]; 
  ssize_t bytes_read;
  ssize_t bytes_sent;
  ssize_t total_sent;
  size_t file_len;
  int file_fd;
  //test*****
  int id = *((int*)arg);

  while(1){
    pthread_mutex_lock(&work_mutex);
    //test
    printf("******work %d lock queue \n",id);
    while (steque_isempty(work_queue)){
      pthread_cond_wait(&work_cond, &work_mutex);
      //test
      printf("******work %d waiting \n",id);
    }


    //worker get work
    item = steque_pop(work_queue); 
    pthread_mutex_unlock(&work_mutex);
    //test
    printf("****Get file %s\n", item->path); 

    //open file & check if file exist
    file_fd = open((item->path), O_RDONLY);
    if (file_fd < 0) {
        //test
        printf("open file fail, FILE NOT FOUND header for path %s\n", item->path);
        gfs_sendheader(&item->context, GF_FILE_NOT_FOUND, 0);
        gfs_abort(&item->context);
        free(item->path);
        free(item);
        continue;
    }

    //transfer document
    //clean up buffer & initialize var
    memset(buf, 0, BUFSIZE);
    bytes_read= 0;
    bytes_sent= 0;
    file_len = 0;
    int error_flag = 0;

    while (1) {
      bytes_read = read(file_fd, buf, BUFSIZE);
      
      //check if reading file error
      //file complete transfer
      if (bytes_read == 0){
        break;
      }
      //reading error
      if (bytes_read < 0){
        //test
        printf("read file error, header for path %s\n", item->path);
        gfs_sendheader(&item->context, GF_ERROR, 0);
        //gfs_abort(&item->context);
        //free(item->path);
        //free(item);
        error_flag = 1;
        break;
      }

      //reading ok -> send data
      total_sent = 0;
      while (total_sent < bytes_read) {
        bytes_sent = gfs_send(&item->context, buf + total_sent, bytes_read - total_sent);
        //sending error
        if (bytes_sent < 0) {
            //test
            printf("sent file error, header for path %s\n", item->path);
            gfs_sendheader(&item->context, GF_ERROR, 0);
            //gfs_abort(&item->context);
            //free(item->path);
            //free(item);
            error_flag = 1;
            break;
        }
        total_sent += bytes_sent;
        file_len += bytes_sent;
      }
      if (error_flag){
        break;
      }
    }

    //file correctly send -> send header and free memory
    if(!error_flag){
      gfs_sendheader(&item->context, GF_OK, file_len);
    }

    //clean up
    close(file_fd);
    gfs_abort(&item->context);
    free(item->path);
    free(item);
  }
  return NULL;
}





/* Main ========================================================= */
int main(int argc, char **argv) {
  char *content_map = "content.txt";
  int option_char = 0;
  gfserver_t *gfs = NULL;
  int nthreads = 14;
  unsigned short port = 17394;

  setbuf(stdout, NULL);

  if (SIG_ERR == signal(SIGINT, _sig_handler)) {
    fprintf(stderr, "Can't catch SIGINT...exiting.\n");
    exit(EXIT_FAILURE);
  }

  if (SIG_ERR == signal(SIGTERM, _sig_handler)) {
    fprintf(stderr, "Can't catch SIGTERM...exiting.\n");
    exit(EXIT_FAILURE);
  }

  // Parse and set command line arguments
  while ((option_char = getopt_long(argc, argv, "p:d:rhm:t:", gLongOptions,
                                    NULL)) != -1) {
    switch (option_char) {
      case 'h':  /* help */
        fprintf(stdout, "%s", USAGE);
        exit(0);
        break;
      case 'd':  /* delay */
        content_delay = (unsigned long int)atoi(optarg);
        break;
	  case 'p':  /* listen-port */
        port = atoi(optarg);
        break;
      case 'm':  /* file-path */
        content_map = optarg;
        break;
      case 't':  /* nthreads */
        nthreads = atoi(optarg);
        break;
      default:
        fprintf(stderr, "%s", USAGE);
        exit(1);
    }
  }

  /* not useful, but it ensures the initial code builds without warnings */
  if (nthreads < 1) {
    nthreads = 1;
  }

  if (content_delay > 5000000) {
    fprintf(stderr, "Content delay must be less than 5000000 (microseconds)\n");
    exit(__LINE__);
  }

  content_init(content_map);

  /* Initialize thread management */
  //malloc and initialize Global resuorce***
  workers = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
  work_queue = (steque_t*) malloc(sizeof(steque_t));
  steque_init(work_queue);
  // add your threadpool creation here*****
  int workerNum[nthreads];
  for (int i = 0; i < nthreads; i++){
    workerNum[i] = i;
    pthread_create(&workers[i], NULL, worker_function, &workerNum[i]);
  }


  /*Initializing server*/
  gfs = gfserver_create();

  //Setting options
  gfserver_set_port(&gfs, port);
  gfserver_set_maxpending(&gfs, 24);
  gfserver_set_handler(&gfs, gfs_handler);
  gfserver_set_handlerarg(&gfs, NULL);  // doesn't have to be NULL!

  /*Loops forever*/
  gfserver_serve(&gfs);


  //clean up
  //join & free worker
  for (int i = 0; i < nthreads; i++){
    //test
    //printf("******worker %d start to join \n",i);
    pthread_join(workers[i], NULL);
    //test
    //printf("******worker %d joined \n",i);
  }
  free(workers);

  //clean other
  pthread_mutex_destroy(&work_mutex);
  pthread_cond_destroy(&work_cond);
  steque_destroy(work_queue);; 
  free(work_queue);  

  return 0;
}
