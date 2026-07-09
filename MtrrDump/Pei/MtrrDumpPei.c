/** @file
  MTRR Dump PEI Module - Dumps MTRR at EndOfPei.

  Copyright (c) 2026, Jabil Inc. All rights reserved.
**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include "../Common/MtrrDumpCommon.h"

//
// Forward declaration
//
EFI_STATUS
EFIAPI
MtrrDumpEndOfPeiCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR mEndOfPeiNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  MtrrDumpEndOfPeiCallback
};

/**
  EndOfPei callback - dump MTRR at end of PEI phase.
**/
EFI_STATUS
EFIAPI
MtrrDumpEndOfPeiCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  MtrrDumpPrint ("PEI-EndOfPei");
  return EFI_SUCCESS;
}

/**
  Entry point - register EndOfPei notify.
**/
EFI_STATUS
EFIAPI
MtrrDumpPeiEntry (
  IN EFI_PEI_FILE_HANDLE    FileHandle,
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
  return PeiServicesNotifyPpi (&mEndOfPeiNotify);
}
