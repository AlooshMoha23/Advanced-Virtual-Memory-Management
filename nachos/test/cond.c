#include "userlib/libnachos.h"
#include "userlib/syscall.h"

LockId my_lock;
CondId my_cond;
int flag = 0;

void wait_for_flag() {
  LockAcquire(my_lock);
  while (flag == 0) {
    CondWait(my_cond);
  }
  n_printf("Flag is set\n");
  LockRelease(my_lock);
}

void set_flag() {
  LockAcquire(my_lock);
  flag = 1;
  n_printf("waking everyone\n");
  CondBroadcast(my_cond);
  LockRelease(my_lock);
}

int main() {
  n_printf("**********************CONDITIONS***************************\n");

  my_lock = LockCreate("my_lock");
  my_cond = CondCreate("my_cond");
  ThreadId t1 = threadCreate("thread1", &wait_for_flag);
  ThreadId t2 = threadCreate("thread2", &wait_for_flag);
  ThreadId t3 = threadCreate("thread3", &set_flag);
  Join(t1);
  Join(t2);
  Join(t3);
  LockDestroy(my_lock);
  CondDestroy(my_cond);
  return 0;
}
