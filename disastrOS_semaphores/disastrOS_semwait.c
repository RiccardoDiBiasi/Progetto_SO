#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "linked_list.h" // Aggiungo le funzioni per operare con le linked list, come detach e insert
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){
  
  int fd_sem = running->syscall_args[0];

  SemDescriptor* desc = (SemDescriptor*) SemDescriptorList_byFd(&running->sem_descriptors, fd_sem);

  if(!desc){
    running->syscall_retvalue = DSOS_ERR_SEMNOTFD;
    return;
  }

  Semaphore* s = desc->semaphore;

  s->count-=1;

  if(s->count < 0){
    SemDescriptorPtr* desc_ptr = SemDescriptorPtr_alloc(desc);
    List_insert(&(s->waiting_descriptors), s->waiting_descriptors.last, (ListItem*)desc_ptr);
    running->status = Waiting;
    List_insert(&waiting_list, waiting_list.last, (ListItem*)running);
  }

  running->syscall_retvalue = 0; //Come al solito ritorno 0 se l'operazione avviene con successo

}
