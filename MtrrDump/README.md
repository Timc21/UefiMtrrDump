# UEFI MTRR Dump Module

A portable UEFI PEI + DXE module that dumps MTRR (Memory Type Range Register) settings at multiple boot stages for debugging and analysis.

## Boot Stages Covered

| Stage | Module | Trigger |
|-------|--------|---------|
| PEI EndOfPei | MtrrDumpPei | `gEfiEndOfPeiSignalPpiGuid` notify |
| DXE Entry | MtrrDumpDxe | Driver load |
| EndOfDxe | MtrrDumpDxe | `gEfiEndOfDxeEventGroupGuid` event |
| ReadyToBoot | MtrrDumpDxe | `EfiCreateEventReadyToBootEx` |

## Sample Output

```
=== [JBL MTRR Dump] Phase: PEI-EndOfPei ===
  Default: 0x00(UC) En:1 FixEn:1 VCNT:10
  Fixed[00]: 0606060606060606
  Fixed[01]: 0606060606060606
  Var[00]: 0x000000000000 Mask=0x3FFF80000000 WB
  Var[01]: 0x00FF000000   Mask=0x3FFFFF000000 WP
=== [JBL MTRR Dump] End ===

=== [JBL MTRR Dump] Phase: DXE-Entry ===
  Default: 0x00(UC) En:1 FixEn:1 VCNT:10
  Fixed[00]: 0606060606060606
  Fixed[01]: 0606060606060606
  Var[00]: 0x000000000000 Mask=0x3FF000000000 WB
  Var[01]: 0x000080000000 Mask=0x3FFF80000000 UC
=== [JBL MTRR Dump] End ===
```

## File Structure

```
MtrrDump/
├── README.md
├── MtrrDump.sdl              # AMI Aptio V SDL (token control)
├── MtrrDump.cif              # AMI CIF for build system integration
├── Common/
│   ├── MtrrDumpCommon.h      # Function declaration
│   └── MtrrDumpCommon.c      # Shared dump logic (DEBUG_ERROR level)
├── Pei/
│   ├── MtrrDumpPei.c         # PEIM: EndOfPei callback
│   └── MtrrDumpPei.inf       # PEI module INF
└── Dxe/
    ├── MtrrDumpDxe.c         # DXE: Entry + EndOfDxe + ReadyToBoot
    └── MtrrDumpDxe.inf       # DXE driver INF
```

## Integration

### AMI Aptio V (with SDL)

1. Copy `MtrrDump/` folder into your package (e.g., `JabilCorePkg/MtrrDump/`)
2. Add to your package's `.cif` under `[parts]`:
   ```
   "YourPkg.MtrrDump"
   ```
3. Add to your project `.veb`:
   ```
   "YourPkg/MtrrDump/MtrrDump.cif"= Private
   ```
4. Build. The SDL token `JBL_MTRR_DUMP_SUPPORT` controls enable/disable.

### EDK2 (without SDL)

Add to your platform DSC `[Components]` section:

```ini
[Components.IA32]
  YourPkg/MtrrDump/Pei/MtrrDumpPei.inf

[Components.X64]
  YourPkg/MtrrDump/Dxe/MtrrDumpDxe.inf
```

Add to your platform FDF:

```ini
# In PEI FV
INF YourPkg/MtrrDump/Pei/MtrrDumpPei.inf

# In DXE FV
INF YourPkg/MtrrDump/Dxe/MtrrDumpDxe.inf
```

## Dependencies

- `MdePkg`
- `UefiCpuPkg` (for `MtrrLib`)

## Debug Level

All output uses `DEBUG_ERROR` (0x80000000) so it always prints regardless of `PcdDebugPrintErrorLevel` setting.

## License

BSD-2-Clause
