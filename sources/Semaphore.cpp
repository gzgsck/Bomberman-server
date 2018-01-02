#include "../headers/Semaphore.h"

void raiseSemaphore(int semid, int semnum, int v) {
   buf.sem_num = semnum;
   buf.sem_op = 1*v;
   buf.sem_flg = 0;
   if (semop(semid, &buf, 1) == -1) {
      perror("Podnoszenie semafora");
      exit(1);
   }
}

void lowerSemaphore(int semid, int semnum, int v) {
   buf.sem_num = semnum;
   buf.sem_op = -1*v;
   buf.sem_flg = 0;
   if (semop(semid, &buf, 1) == -1) {
      perror("Opuszczenie semafora");
      exit(1);
   }
}