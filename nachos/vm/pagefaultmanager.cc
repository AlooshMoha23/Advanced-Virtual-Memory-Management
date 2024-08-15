/*! \file pagefaultmanager.cc
Routines for the page fault managerPage Fault Manager
 * -----------------------------------------------------
 * This file is part of the Nachos-RiscV distribution
 * Copyright (c) 2022 University of Rennes 1.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details
 * (see see <http://www.gnu.org/licenses/>).
 * -----------------------------------------------------
*/

#include "vm/pagefaultmanager.h"
#include "kernel/msgerror.h"
#include "kernel/thread.h"
#include "vm/physMem.h"
#include "vm/swapManager.h"

PageFaultManager::PageFaultManager() {}

// PageFaultManager::~PageFaultManager()
/*! Nothing for now
 */
PageFaultManager::~PageFaultManager() {}

// ExceptionType PageFault(uint64_t virtualPage)
/*!
//	This method is called by the Memory Management Unit when there is a
//      page fault. This method loads the page from :
//      - read-only sections (text,rodata) $\Rightarrow$ executive
//        file
//      - read/write sections (data,...) $\Rightarrow$ executive
//        file (1st time only), or swap file
//      - anonymous mappings (stack/bss) $\Rightarrow$ new
//        page from the MemoryManager (1st time only), or swap file
//
//	\param virtualPage the virtual page subject to the page fault
//	  (supposed to be between 0 and the
//        size of the address space, and supposed to correspond to a
//        page mapped to something [code/data/bss/...])
//	\return the exception (generally the NO_EXCEPTION constant)
*/

#ifndef ETUDIANTS_TP
ExceptionType PageFaultManager::PageFault(uint64_t virtualPage) {
  printf("**** Warning: page fault manager is not implemented yet\n");
  exit(ERROR);
  return ((ExceptionType)0);
}
#endif

#ifdef ETUDIANTS_TP
ExceptionType PageFaultManager::PageFault(uint64_t virtualPage) {

  // Check if the virtual page is valid
  while (g_current_thread->GetProcessOwner()
             ->addrspace->translationTable->getBitIo(virtualPage)) {
    IntStatus oldLevel = g_machine->interrupt->SetStatus(INTERRUPTS_OFF);

    g_current_thread->Sleep();
    (void)g_machine->interrupt->SetStatus(oldLevel);
  }
  g_current_thread->GetProcessOwner()->addrspace->translationTable->setBitIo(
      virtualPage);

  int physicalPage = g_physical_mem_manager->AddPhysicalToVirtualMapping(
      g_current_thread->GetProcessOwner()->addrspace, virtualPage);

  // Check the swap bit of the page
  if (g_current_thread->GetProcessOwner()
          ->addrspace->translationTable->getBitSwap(virtualPage)) {
    printf("**** swap,%x\n",
           g_current_thread->GetProcessOwner()
               ->addrspace->translationTable->getAddrDisk(virtualPage));
    // The page is in the swap area
    g_swap_manager->GetPageSwap(
        g_current_thread->GetProcessOwner()
            ->addrspace->translationTable->getAddrDisk(virtualPage),
        (char *)&(g_machine->mainMemory[g_current_thread->GetProcessOwner()
                                            ->addrspace->translationTable
                                            ->getPhysicalPage(virtualPage) *
                                        g_cfg->PageSize]));

    // Clear the swap bit
    // g_current_thread->GetProcessOwner()->addrspace->translationTable->clearBitSwap(virtualPage);
  } else if (g_current_thread->GetProcessOwner()
                 ->addrspace->translationTable->getAddrDisk(virtualPage) ==
             INVALID_SECTOR) {
    printf("**** anonymous,%x\n",
           g_current_thread->GetProcessOwner()
               ->addrspace->translationTable->getAddrDisk(virtualPage));
    // The page is an "anonymous" page

    memset(&(g_machine->mainMemory[g_current_thread->GetProcessOwner()
                                       ->addrspace->translationTable
                                       ->getPhysicalPage(virtualPage) *
                                   g_cfg->PageSize]),
           0, g_cfg->PageSize);

  } else {
    printf("**** fetch fault reading from disk,%x\n",
           g_current_thread->GetProcessOwner()
               ->addrspace->translationTable->getAddrDisk(virtualPage));
    // The page is to be loaded from the executable
    g_current_thread->GetProcessOwner()->exec_file->ReadAt(
        (char *)&(g_machine->mainMemory[g_current_thread->GetProcessOwner()
                                            ->addrspace->translationTable
                                            ->getPhysicalPage(virtualPage) *
                                        g_cfg->PageSize]),
        g_cfg->PageSize,
        g_current_thread->GetProcessOwner()
            ->addrspace->translationTable->getAddrDisk(virtualPage));
  }

  // Update the translation table to point to the new physical page

  // set bit valid
  g_current_thread->GetProcessOwner()->addrspace->translationTable->setBitValid(
      virtualPage);
  g_current_thread->GetProcessOwner()->addrspace->translationTable->clearBitIo(
      virtualPage);
  g_current_thread->GetProcessOwner()->addrspace->translationTable->setBitU(
      virtualPage);
  g_current_thread->GetProcessOwner()->addrspace->translationTable->clearBitM(
      virtualPage);

  // Unlock the the physical page
  g_physical_mem_manager->UnlockPage(physicalPage);

  printf("**** end pagefault\n");

  // Return NO_EXCEPTION to indicate that the page fault has been handled
  return NO_EXCEPTION;
}

#endif