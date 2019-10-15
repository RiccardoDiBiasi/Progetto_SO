#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){
  
  int id_sem = running->syscall_args[0];

  SemDescriptor* desc = SemDescriptor_alloc(&running->sem_descriptors, id_sem);

  if(!desc){
	running->syscall_retvalue = DSOS_ESEMDESCR;
    return;
  } 

  
}
