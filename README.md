# Advanced-Virtual-Memory-Management

The report provided outlines the work conducted as part of the Practical Exercises on implementing a virtual memory management system. The primary objective was to understand and implement the key components of such a system, including page fault handling routines, a page replacement algorithm, and memory-mapped files.

First, we focused on setting up separate address spaces to allow multiple processes to run, each with its own private page table. This required modifications to the SaveProcessorState and RestoreProcessorState methods in the Thread class.

Next, we altered the program loading process to make it demand-driven, triggering a page fault upon the first access to a non-allocated page. This involved changes in the constructor of the AddrSpace class and the StackAllocate method.

Finally, we implemented a page fault handling routine and a page replacement algorithm. These components are essential for managing scenarios where physical memory is full, and a process needs to load a page that is not currently in physical memory.

The report details the various stages of our work, the challenges encountered, and the solutions devised to meet the project requirements.
