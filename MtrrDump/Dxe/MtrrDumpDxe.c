/** @file
  MTRR Dump DXE Driver - Dumps MTRR at DXE-Entry, EndOfDxe, ReadyToBoot.

  Copyright (c) 2026, Jabil Inc. All rights reserved.
**/

#include <Uefi.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Guid/EventGroup.h>
#include "../Common/MtrrDumpCommon.h"

STATIC EFI_EVENT  mEndOfDxeEvent;
STATIC EFI_EVENT  mReadyToBootEvent;

STATIC
VOID
EFIAPI
MtrrDumpEndOfDxeCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  MtrrDumpPrint ("DXE-EndOfDxe");
  gBS->CloseEvent (Event);
}

STATIC
VOID
EFIAPI
MtrrDumpReadyToBootCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  MtrrDumpPrint ("DXE-ReadyToBoot");
  gBS->CloseEvent (Event);
}

EFI_STATUS
EFIAPI
MtrrDumpDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  //
  // Phase: DXE Driver Load
  //
  MtrrDumpPrint ("DXE-Entry");

  //
  // Phase: EndOfDxe
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MtrrDumpEndOfDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &mEndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Phase: ReadyToBoot
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             MtrrDumpReadyToBootCallback,
             NULL,
             &mReadyToBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
