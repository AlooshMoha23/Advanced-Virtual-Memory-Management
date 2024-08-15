#include "userlib/libnachos.h"
#include "userlib/syscall.h"

LockId my_lock;

void my_function() {
  LockAcquire(my_lock);
  n_printf("Inside critical section\n");
  LockRelease(my_lock);
}

int main() {
  my_lock = LockCreate("my_lock");
  ThreadId t1 = threadCreate("thread1", my_function);
  ThreadId t2 = threadCreate("thread2", my_function);
  Join(t2);
  Join(t1);

  LockDestroy(my_lock);
  return 0;
}
