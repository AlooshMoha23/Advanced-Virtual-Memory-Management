#include "userlib/libnachos.h"
#include "userlib/syscall.h"

SemId sema1;
SemId sema2;
SemId sema3;
ThreadId t1;
ThreadId t2;
ThreadId t3;
void threadFunc1() {
  n_printf("Thread 1: waitting\n");
  V(sema1);
  V(sema1);
  P(sema2);
  P(sema3);
  n_printf("Thread 1: continue\n");
}

void threadFunc2() {
  n_printf("Thread 2: waitting\n");
  V(sema2);
  V(sema2);
  P(sema1);
  P(sema3);
  n_printf("Thread 2: continue\n");
}

void threadFunc3() {
  n_printf("Thread 3: waitting\n");
  V(sema3);
  V(sema3);
  P(sema1);
  P(sema2);
  n_printf("Thread 3: continue\n");
}

int main() {
  n_printf("**********************RDV***************************\n");
  sema1 = SemCreate("rendezvous1", 0);
  sema2 = SemCreate("rendezvous2", 0);
  sema3 = SemCreate("rendezvous3", 0);
  t1 = threadCreate("Thread 1", &threadFunc1);
  t2 = threadCreate("Thread 2", &threadFunc2);
  t3 = threadCreate("Thread 3", &threadFunc3);

  Join(t1);
  Join(t2);
  Join(t3);

  SemDestroy(sema1);
  SemDestroy(sema2);
  SemDestroy(sema3);

  return 0;
}
