/*
 * Generated with the FPGA Interface C API Generator 12.0.0
 * for NI-RIO 12.0.0 or later.
 */

#ifndef __NiFpga_limb_h__
#define __NiFpga_limb_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1200
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_limb_Bitfile;
 */
#define Bitfile "NiFpga_limb.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_Signature = "35F869D1306350BF28075F7574EEC2FF";

typedef enum
{
   NiFpga_thumb_switch = 0x8136,
   NiFpga_index_switch = 0x8132,
   NiFpga_middle_switch = 0x812E,
   NiFpga_ring_switch = 0x812A,
   NiFpga_pinkie_switch = 0x8126,
} NiFpga_limb_IndicatorBool;

typedef enum
{
   NiFpga_thumb_flex = 0x8120,
   NiFpga_index_flex = 0x811C,
   NiFpga_middle_flex = 0x8118,
   NiFpga_ring_flex = 0x8114,
   NiFpga_pinkie_flex = 0x8110,
} NiFpga_limb_IndicatorI32;

typedef enum
{
   NiFpga_stop = 0x810E,
} NiFpga_limb_ControlBool;

#endif
