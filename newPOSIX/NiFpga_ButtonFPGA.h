/*
 * Generated with the FPGA Interface C API Generator 12.0.0
 * for NI-RIO 12.0.0 or later.
 */

#ifndef __NiFpga_ButtonFPGA_h__
#define __NiFpga_ButtonFPGA_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1200
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_ButtonFPGA_Bitfile;
 */
#define Bitfile "NiFpga_ButtonFPGA.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FPGA_Signature = "135DCE7EB5A47CEBF864D0AC5952991B";

typedef enum
{
   NiFpga_stop = 0x8112,
} NiFpga_ButtonFPGA_ControlBool;

typedef enum
{
   NiFpga_input = 0x810E,
} NiFpga_ButtonFPGA_IndicatorArrayBool;

typedef enum
{
   NiFpga_ButtonFPGA_IndicatorArrayBoolSize_input = 5,
} NiFpga_ButtonFPGA_IndicatorArrayBoolSize;

#endif
