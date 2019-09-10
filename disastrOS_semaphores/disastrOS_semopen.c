#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"


// Creazione del semaforo avente un proprio ID
// proprio tramite questo ID è possibile accedere al semaforo da tutto il sistema
// la funzione ritorna il file descriptor(>=0) in caso di successo, un codice di errore(<0) in caso di fallimento

void internal_semOpen(){
  
    int id_sem = running->syscall_args[0];
    int val_sem = running->syscall_args[1];

    printf("[X] Processo %d ha richiesto l'apertura del semaforo %d\n", running->pid, id_sem);

    //Controllo se non esiste già un semaforo aperto con quell'ID

    Semaphore* s = SemaphoreList_byId(&semaphores_list, id_sem);
    if(!sem){
      sem = Semaphore_alloc(id_sem, val_sem);

      if(!sem){
      running->syscall_retvalue = DSOS_SEMNOTALLOC;
      return;
        }

      //Aggiungo il semaforo alla lista

      List_insert(&semaphores_list, semaphores_list.last, (ListItem*) sem);

    } 


}
