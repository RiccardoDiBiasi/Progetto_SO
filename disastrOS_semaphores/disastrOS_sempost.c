#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#include "linked_list.h"
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost(){
  
  int fd_sem = running->syscall_args[0];

  SemDescriptor* desc = SemDescriptorList_byFd(&running->sem_descriptors, fd_sem);

  Semaphore* sem = desc->semaphore;

  if(!sem){
    running->syscall_retvalue = DSOS_ERR_SEMNOTALLOC;
    return;
  }

  (sem->count)++;

  if(sem->count <= 0){

    SemDescriptorPtr* desc_ptr = (SemDescriptorPtr*)List_detach(&(sem->waiting_descriptors), (ListItem*) (sem->waiting_descriptors.first));

    if(!desc_ptr){
      running->syscall_retvalue = DSOS_ERESOURCEOPEN;
      return;
    }

    SemDescriptor* desc = desc_ptr->descriptor;

    PCB* process = desc->pcb;

    List_insert(&sem->descriptors, sem->descriptors.last, (ListItem*)desc_ptr); 

    List_detach(&waiting_list, (ListItem*)process); // Rimuovo il pcb dalla lista dei waiting per poi aggiungerlo a quella dei ready

    List_insert(&ready_list, &ready_list.last, (ListItem*)process);

    process->status = Ready;
  }

  running->syscall_retvalue = 0; // In caso di successo ritorno 0;
  return;
}
