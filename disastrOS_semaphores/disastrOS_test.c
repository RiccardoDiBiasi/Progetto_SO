#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#include <stdlib.h>

#include "disastrOS.h"
#include "disastrOS_semaphore.h"




#define BUFFER_SIZE 10
#define ITER 5
#define MAX_TRANS 300


int buffer[BUFFER_SIZE];
int write_index, read_index;
int size;

static inline int performRandomTransaction() {

    int amount = rand() % (2 * MAX_TRANS);
    if (amount++ >= MAX_TRANS) {
        return MAX_TRANS - amount;
    } else {
        return amount;
    }
}

// ********PRODUCER**********

void producerJob(int empty_sem, int filled_sem, int prod){
    
    for(int i = 0; i < ITER; i++){
      
      int randomn = performRandomTransaction();
      disastrOS_waitSem(empty_sem);

      printf("[X] %d CICLO INIZIATO\n", i);

      disastrOS_waitSem(prod);
      
      buffer[write_index] = randomn;
      write_index = (write_index + 1) % BUFFER_SIZE;

      disastrOS_postSem(prod);

      printf("Il producer %d ha prodotto la risorsa %d\n", disastrOS_getpid(), randomn);

      disastrOS_postSem(filled_sem);

      printf("[X] IL CICLO DEL PRODUTTORE E' FINITO\n");
    }
}

//***********CONSUMER***********

void consumerJob(int sem_empty, int filled_sem, int cons){

  for(int i = 0; i < ITER; i++){

    disastrOS_waitSem(filled_sem);

    printf("[X] CONSUMER STARTED\n");

    int last = buffer[read_index];
    size+=last;
    buffer[read_index] = 0;
    read_index = (read_index + 1) % BUFFER_SIZE;

    disastrOS_postSem(cons);

    printf("[X] CONSUMED: THE SUM IS %d\n", size);
    
    disastrOS_postSem(sem_empty);

    printf("[X] CONSUMER TERMINATO\n");
  }
}



// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void childFunction(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("I will iterate a bit, before terminating\n");
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);

  read_index = 0;
  write_index = 0;

  int sem_filled = disastrOS_openSem(1,0);

  int sem_empty = disastrOS_openSem(2, BUFFER_SIZE);

  int sem_cons = disastrOS_openSem(3, 1);

  int sem_prod = disastrOS_openSem(4, 1);

  disastrOS_printStatus();

    if(disastrOS_getpid() > 1){
      if(disastrOS_getpid() % 2 == 0){
        printf("[X] AVVIATO IL PROCESSO %d PRODUTTORE\n", disastrOS_getpid());
        disastrOS_sleep(20);
        producerJob(sem_empty, sem_filled, sem_prod);
      }
      else{
        printf("[X] AVVIATO IL PROCESSO %d CONSUMATORE\n", sem_cons);
        disastrOS_sleep(20);
        consumerJob(sem_empty, sem_filled, sem_cons);
      }
    }


  printf("PID: %d, terminating\n", disastrOS_getpid());

  disastrOS_closeSem(sem_empty);
  disastrOS_closeSem(sem_filled);
  disastrOS_closeSem(sem_prod);
  disastrOS_closeSem(sem_cons);

  /*for (int i=0; i<(disastrOS_getpid()+1); ++i){
    printf("PID: %d, iterate %d\n", disastrOS_getpid(), i);
    disastrOS_sleep((20-disastrOS_getpid())*5);
  }*/
  disastrOS_exit(disastrOS_getpid()+1);
}


void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);
  

  printf("I feel like to spawn 10 nice threads\n");
  int alive_children=0;
  for (int i=0; i<10; ++i) {
    //int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource (and creating if necessary)\n");
    //int fd=disastrOS_openResource(i,type,mode);
    //printf("fd=%d\n", fd);
    disastrOS_spawn(childFunction, 0);
    alive_children++;
  }

  
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    //disastrOS_printStatus();
    //printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   //pid, retval, alive_children);
    --alive_children;
  }
  disastrOS_printStatus();
  printf("shutdown!");
  disastrOS_shutdown();
}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
  printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
