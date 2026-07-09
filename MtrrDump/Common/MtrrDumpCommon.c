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
    if (MtrrSettings.Fixed.Mtrr[Index] != 0) {
      DEBUG ((DEBUG_ERROR, "  Fixed[%02d]: %016lx\n", Index, MtrrSettings.Fixed.Mtrr[Index]));
    }
  }

  for (Index = 0; Index < VariableMtrrCount; Index++) {
    if ((MtrrSettings.Variables.Mtrr[Index].Mask & BIT11) != 0) {
      UINT8  Type = (UINT8)(MtrrSettings.Variables.Mtrr[Index].Base & 0xFF);
      UINT64 BaseAddr = MtrrSettings.Variables.Mtrr[Index].Base & 0xFFFFFFFFFFFFF000ULL;
      UINT64 MaskAddr = MtrrSettings.Variables.Mtrr[Index].Mask & 0xFFFFFFFFFFFFF000ULL;
      DEBUG ((DEBUG_ERROR, "  Var[%02d]: 0x%012lx Mask=0x%012lx %a\n",
        Index, BaseAddr, MaskAddr,
        (Type == 6) ? "WB" : (Type == 0) ? "UC" :
        (Type == 1) ? "WC" : (Type == 4) ? "WT" :
        (Type == 5) ? "WP" : "??"));
    }
  }

  DEBUG ((DEBUG_ERROR, "=== [MTRR Dump] End ===\n\n"));
}
