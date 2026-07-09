/** @file
  Common MTRR dump implementation - shared between PEI and DXE.

  Copyright (c) 2026, Yan-ting Chen All rights reserved.
**/

#include "MtrrDumpCommon.h"

/**
  Dump current MTRR settings with a phase label.

  @param[in] PhaseLabel   ASCII string describing current boot phase.
**/
VOID
MtrrDumpPrint (
  IN CONST CHAR8  *PhaseLabel
  )
{
  MTRR_SETTINGS                 MtrrSettings;
  UINTN                         Index;
  UINT32                        VariableMtrrCount;
  MSR_IA32_MTRRCAP_REGISTER    MtrrCap;
  UINT8                         DefaultType;

  MtrrGetAllMtrrs (&MtrrSettings);

  MtrrCap.Uint64 = AsmReadMsr64 (MSR_IA32_MTRRCAP);
  VariableMtrrCount = (UINT32)MtrrCap.Bits.VCNT;
  DefaultType = (UINT8)(MtrrSettings.MtrrDefType & 0xFF);

  DEBUG ((DEBUG_ERROR, "\n=== [MTRR Dump] Phase: %a ===\n", PhaseLabel));
  DEBUG ((DEBUG_ERROR, "  Default: 0x%02x(%a) En:%d FixEn:%d VCNT:%d\n",
    DefaultType,
    (DefaultType == 6) ? "WB" : (DefaultType == 0) ? "UC" :
    (DefaultType == 1) ? "WC" : (DefaultType == 4) ? "WT" :
    (DefaultType == 5) ? "WP" : "??",
    (MtrrSettings.MtrrDefType & BIT11) ? 1 : 0,
    (MtrrSettings.MtrrDefType & BIT10) ? 1 : 0,
    VariableMtrrCount));

  for (Index = 0; Index < MTRR_NUMBER_OF_FIXED_MTRR; Index++) {
    DEBUG ((DEBUG_ERROR, "  Fixed[%02d]: %016lx\n", Index, MtrrSettings.Fixed.Mtrr[Index]));
  }

  for (Index = 0; Index < VariableMtrrCount; Index++) {
    if ((MtrrSettings.Variables.Mtrr[Index].Mask & BIT11) != 0) {
      UINT8  Type = (UINT8)(MtrrSettings.Variables.Mtrr[Index].Base & 0xFF);
      UINT64 BaseAddr = MtrrSettings.Variables.Mtrr[Index].Base & 0xFFFFFFFFFFFFF000ULL;
      UINT64 MaskAddr = MtrrSettings.Variables.Mtrr[Index].Mask & 0xFFFFFFFFFFFFF000ULL;
      //
      // Calculate range: Size = 2^N where N = position of lowest set bit in MaskAddr
      // Use bit scan to find lowest set bit
      //
      UINT64 Size = MaskAddr & (~MaskAddr + 1);  // Isolate lowest set bit = size
      UINT64 Start = BaseAddr & MaskAddr;
      UINT64 End = Start + Size - 1;
      UINT32 SizeMB = (UINT32)(Size >> 20);

      DEBUG ((DEBUG_ERROR, "  Var[%02d]: 0x%09lx - 0x%09lx (%dMB) %a\n",
        Index, Start, End, SizeMB,
        (Type == 6) ? "WB" : (Type == 0) ? "UC" :
        (Type == 1) ? "WC" : (Type == 4) ? "WT" :
        (Type == 5) ? "WP" : "??"));
    }
  }

  DEBUG ((DEBUG_ERROR, "=== [MTRR Dump] End ===\n\n"));
}
