#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"
#include "linked_list.h"

#include "disastrOS_globals.h"
#include "disastrOS_constants.h"
#include "disastrOS_resource.h"



// Cerco il semaforo attraverso l'id, se non esiste viene allocato e aggiunto alla lista dei semafori

void internal_semOpen(){
  
    int id_sem = running->syscall_args[0];
    int val_sem = running->syscall_args[1];

    printf("Il processo %d ha richiesto l'apertura del semaforo %d\n", running->pid, id_sem);

    //Controllo se non esiste già un semaforo aperto con quell'ID

    Semaphore* sem = SemaphoreList_byId(&semaphores_list, id_sem);

    if(!sem){

      //Se non esiste lo alloco
      sem = Semaphore_alloc(id_sem, val_sem);

      //Controllo che sia stato allocato
      if(!sem){
      running->syscall_retvalue = DSOS_ERR_SEMNOTALLOC;
      return;
      }

      //Aggiungo il semaforo alla lista

      List_insert(&semaphores_list, semaphores_list.last, (ListItem*) sem);

    }

    //Controllo che il semaforo abbia un valore >= 0;

    if(val_sem < 0){
      running->syscall_retvalue = DSOS_ERR_SEMWRONGVAL;
      fprintf(stdout, "[BAD] NEGATIVE VALUE FOR THE SEMAPHORE\n");; //Devo gestire l'errore con una f.ne apposita??
      return;
    }
    

    int fd = running->last_sem_fd;

    //Creo il descrittore
    SemDescriptor* des = SemDescriptor_alloc(fd, sem, running);

    if(!des){
      //Caso in cui non riesco ad allocare il descrittore
      running->syscall_retvalue = DSOS_ERESOURCENOFD;
      return;
    }
    

    List_insert(&running->sem_descriptors, running->sem_descriptors.last, (ListItem*) des);

    running->last_sem_fd++;


    

    SemDescriptorPtr* des_ptr = SemDescriptorPtr_alloc(des);


    //Controllo di avere il puntatore allocato
    if(!des_ptr){
      running->syscall_retvalue = DSOS_ERR_SEMNOTDESCPTR;
      return;
    }

    //Assegno il puntatore al semaforo e lo aggiungo alla lista
    des->ptr = des_ptr;
    List_insert(&sem->descriptors, sem->descriptors.last, (ListItem*) des_ptr);

    running->syscall_retvalue = des->fd;
    fprintf(stdout, "[GOOD] SEMAPHORE OPENED\n");
    return;
}
