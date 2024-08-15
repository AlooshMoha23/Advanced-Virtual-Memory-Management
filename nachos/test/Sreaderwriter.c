#include "userlib/libnachos.h"
#include "userlib/syscall.h"

SemId mutex;
SemId line;
int readcount = 0;

void reader() {
  for (int i = 0; i < 5; i++) {
    P(mutex);
    readcount++;
    if (readcount == 1) {
      P(line);
    }
    V(mutex);
    n_printf("Reading\n");
    P(mutex);
    readcount--;
    if (readcount == 0) {
      V(line);
    }
    V(mutex);
  }
}

void writer() {
  for (int i = 0; i < 5; i++) {
    P(line);
    n_printf("Writing\n");
    V(line);
  }
}

int main() {
  mutex = SemCreate("mutex", 0);
  line = SemCreate("line", 1);
  ThreadId r1 = threadCreate("reader1", reader);
  ThreadId w1 = threadCreate("writer1", writer);
  Join(r1);
  Join(w1);

  SemDestroy(mutex);
  SemDestroy(line);
  return 0;
}
