# OpenBootloader Porting Guide: NUCLEO-N657X0-Q

## Overview
This document summarizes the steps and findings for porting the `OpenBootloader` application from the **STM32N6570-DK** to the **NUCLEO-N657X0-Q** board.

## 1. Hardware Differences
| Feature | STM32N6570-DK | NUCLEO-N657X0-Q |
| :--- | :--- | :--- |
| **Board** | DK | NUCLEO |
| **NOR Flash** | Macronix **MX66UW1G45G** | Macronix **MX25UM51245G** |
| **Size** | 128 MB (`0x08000000`) | 64 MB (`0x04000000`) |
| **Sector Size** | 64 KB (`0x10000`) | 64 KB (`0x10000`) |

## 2. Project Modifications
The OpenBootloader logic is generic, but the project configuration must match the target hardware.

### A. Configuration Header
**File**: `Applications\OpenBootloader\OpenBootloader\Target\openbootloader_conf.h`
- **Updated `EXT_MEMORY_SIZE`**: `0x04000000` (64MB)
- **Updated `EXT_MEMORY_END_ADDRESS`**: `0x74000000`
- **Verified `EXT_MEMORY_SECTOR_SIZE`**: `0x10000` (64KB)

### B. Project Settings (`.cproject`)
**File**: `Applications\OpenBootloader\STM32CubeIDE\.cproject`
- **Board Definition**: Changed from `STM32N6570-DK` to `NUCLEO-N657X0-Q`.
- **Include Paths**: Added `../../../../../../Drivers/BSP/STM32N6xx_Nucleo` to ensure access to NUCLEO BSP drivers.

## 3. External Loader Strategy
The OpenBootloader (running in RAM) relies on a **separate External Loader binary** to communicate with the specific flash chip. You cannot use the precompiled `MX66...bin` provided for the DK board.

### Source Code Location
- Use the **ExtMemLoader** project found in:
  `Projects\NUCLEO-N657X0-Q\Templates\Template_FSBL_XIP\ExtMemLoader`
- This project already references the Correct driver (`extmem_nor_sfdp` / `mx25um51245g`) for the NUCLEO board.

### Build Steps
1.  Open `Template_FSBL_XIP`.
2.  Switch configuration to **ExtMemLoader**.
3.  Build to generate the raw binary (e.g., `MX25UM51245G_NUCLEO.bin`).

## 4. Generating the Signed External Loader Binary (`-OBL.bin`)
To use the External Loader with the OpenBootloader protocol, it must be wrapped with a specific header.

**Tool**: `STM32_SigningTool_CLI.exe` (part of STM32CubeProgrammer).

**Command**:
```bash
STM32_SigningTool_CLI.exe -bin MX25UM51245G_NUCLEO.bin -nk -of 0x38000000 -t fsbl-extfl -o MX25UM51245G_NUCLEO-OBL.bin -hv 2.3
```

**Parameters Explained**:
- `-bin`: Input raw binary.
- `-of 0x38000000`: Load address in RAM (Matching `external_memory_interface.c`).
- `-t fsbl-extfl`: Binary type for External Flash Loader.
- `-o`: Output filename.


## 5. Generating the Signed OpenBootloader Binary (`.stm32`)
To boot the OpenBootloader via the BootROM (FSBL loading), you must sign the main application binary. The NUCLEO-N657X0-Q OpenBootloader is linked to run at `0x34180400`.

**Input**: `STM32CubeIDE\Debug\OpenBootloader.bin` (from the OpenBootloader project build)
**Tool**: `STM32_SigningTool_CLI.exe`

**Command**:
```bash
STM32_SigningTool_CLI.exe -bin OpenBootloader.bin -nk -of 0x34180400 -t fsbl -o OpenBootloader_NUCLEO-N657X0-Q-trusted.stm32 -hv 2.3
```

**Parameters Explained**:
- `-of 0x34180400`: Load address in AXI SRAM2 (Must match linker script `STM32N657X0HXQ_AXISRAM2_fsbl.ld`).
- `-t fsbl`: Binary type for First Stage Boot Loader (Application).

## 6. Usage
1.  Connect STM32CubeProgrammer.
2.  Boot the NUCLEO board.
3.  Load `OpenBootloader_NUCLEO-N657X0-Q-trusted.stm32` to RAM (via BootROM or manually to `0x34180400`).
4.  Load the signed `MX25UM51245G_NUCLEO-OBL.bin` to RAM (`0x38000000`).
5.  Execute operations (Read/Write/Erase).
