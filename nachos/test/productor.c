#include "userlib/libnachos.h"
#include "userlib/syscall.h"

#define BUFFER_SIZE 10

SemId empty;
SemId full;
SemId mutex;
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

void producer() {
  for (int i = 0; i < 20; i++) {
    P(empty);
    P(mutex);
    buffer[in] = i;
    in = (in + 1) % BUFFER_SIZE;
    n_printf("produced : %d\n", i);
    V(mutex);
    V(full);
  }
}

void consumer() {
  for (int i = 0; i < 20; i++) {
    P(full);
    P(mutex);
    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    V(mutex);
    V(empty);
    n_printf("Consumed: %d\n", item);
  }
}

int main() {
  n_printf("**********************PRODUCTOR AND "
           "CONSUMER***************************\n");

  empty = SemCreate("empty", BUFFER_SIZE);
  full = SemCreate("full", 0);
  mutex = SemCreate("mutex", 1);
  ThreadId prod = threadCreate("producer", producer);
  ThreadId cons = threadCreate("consumer", consumer);
  Join(prod);
  Join(cons);
  SemDestroy(empty);
  SemDestroy(full);
  SemDestroy(mutex);
  return 0;
}
