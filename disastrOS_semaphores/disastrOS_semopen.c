#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

#include "disastrOS_globals.h"
#include "disastrOS_resource.h"



// Cerco il semaforo attraverso l'id, se non esiste viene allocato e aggiunto alla lista dei semafori

void internal_semOpen(){
  
    int id_sem = running->syscall_args[0];
    int val_sem = running->syscall_args[1];

    printf("Il processo %d ha richiesto l'apertura del semaforo %d\n", running->pid, id_sem);

    //Controllo se non esiste già un semaforo aperto con quell'ID

    Semaphore* sem = SemaphoreList_byId(&semaphores_list, id_sem);

    if(!sem){
      s = Semaphore_alloc(id_sem, val_sem);

      if(!sem){
      running->syscall_retvalue = DSOS_SEMNOTALLOC;
      return;
        }

      //Aggiungo il semaforo alla lista

      List_insert(&semaphores_list, semaphores_list.last, (ListItem*) sem);

    } 


}
