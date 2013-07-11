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
static const char* const NiFpga_FPGA_Signature = "1617612A4B001231B03C1535FA655CA7";

typedef enum
{
   NiFpga_middle = 0x8116,
   NiFpga_pinkie = 0x810E,
   NiFpga_index = 0x811A,
   NiFpga_ring = 0x8112,
   NiFpga_thumb = 0x811E, // DI0
} NiFpga_ButtonFPGA_IndicatorBool;

typedef enum
{
   NiFpga_stop = 0x8122,
} NiFpga_ButtonFPGA_ControlBool;

#endif
