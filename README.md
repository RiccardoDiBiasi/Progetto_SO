# Progetto_SO
Progetto_SO  Progetto di Sistemi Operativi A.A. 2018-2019 a cura di Di Biasi Riccardo 1694305.
Il progetto consiste nell'implementazione delle principali operazioni relative ai semafori modificando i file forniti dal professore, disastrOS.

Le funzioni definite dallo studente sono 4: 

# internal_semOpen(): 
  Questa è la funzione dedicata all'apertura del semaforo. Essa è stata poi ridefinita in 'disastrOS.c' come disastrOS_openSem(int           id, int value).
  
# internal_semClose():
  Questa è la funzione dedicata alla chiusura del semaforo passato.
  
# internal_semWait():
  Questa è la funzione che decrementa il valore del semaforo passato. Dopo un accurato controllo al valore del semaforo, il processo viene eventualmente messo in waiting_list.
  
# internal_semPost():
  Questa è la funzione che incrementa il valore del semaforo passato. Dopo un accurato controllo al valore del semaforo, il processo viene eventualmente messo in ready_list.

  
Il progetto va eseguito con il comando ./disastrOS_test da terminale dall'interno della cartella disastrOS_semaphores. E' stato testato con il tool valgrind e l'output dell'analisi di valgrind è il seguente:


==6783== 
==6783== HEAP SUMMARY:\n
==6783==     in use at exit: 0 bytes in 0 blocks
==6783==   total heap usage: 2 allocs, 2 frees, 2,048 bytes allocated
==6783== 
==6783== All heap blocks were freed -- no leaks are possible
==6783== 
==6783== For counts of detected and suppressed errors, rerun with: -v
==6783== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)


 
