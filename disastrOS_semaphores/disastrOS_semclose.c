#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){
  
  int id_sem = running->syscall_args[0];

  SemDescriptor* desc = SemDescriptorList_byFd(&running->sem_descriptors, id_sem);

  //Controllo se ho trovato il file descriptor del semaforo
  if(!desc){
	running->syscall_retvalue = DSOS_ERR_SEMNOTFD;
    return;
  }

  

  Semaphore* s = desc->semaphore; //Puntatore al semaforo

  if(!s){
    running->syscall_retvalue = DSOS_ERR_SEMNOTPRESENT;
    return;
  }

  SemDescriptorPtr* desc_ptr = desc->ptr;
  //assert(desc_ptr);

  if(!desc_ptr){
    running->syscall_retvalue = DSOS_ERR_SEMNOTDESCPTR;
    return;
  }
  
  
  desc = (SemDescriptor*) List_detach(&running->sem_descriptors, (ListItem*)desc); //Faccio la detach del desc
  SemDescriptor_free(desc);

  desc_ptr = (SemDescriptorPtr*) List_detach(&s->descriptors, (ListItem*)desc_ptr);

  if(s->descriptors.size == 0 && s->waiting_descriptors.size == 0){
    printf("Rimuovo il semaforo %d dalla lista\n", id_sem);
    s = (Semaphore*) List_detach(&semaphores_list, (ListItem*)s);
    Semaphore_free(s);
  }

  

  SemDescriptorPtr_free(desc_ptr);
  running->syscall_retvalue = 0;  //Ritorno 0 in caso di successo
}
