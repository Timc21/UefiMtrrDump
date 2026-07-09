/** @file
  Common MTRR dump helper - shared between PEI and DXE.

  Copyright (c) 2026, Jabil Inc. All rights reserved.
**/

#ifndef MTRR_DUMP_COMMON_H_
#define MTRR_DUMP_COMMON_H_

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/MtrrLib.h>
#include <Register/ArchitecturalMsr.h>

/**
  Dump current MTRR settings with a phase label.

  @param[in] PhaseLabel   ASCII string describing current boot phase.
**/
VOID
MtrrDumpPrint (
  IN CONST CHAR8  *PhaseLabel
  );

#endif // MTRR_DUMP_COMMON_H_
