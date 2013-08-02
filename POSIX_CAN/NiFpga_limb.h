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
static const char* const NiFpga_Signature = "58A9C9DEE61855BA1197BD9B1F8B2984";

typedef enum
{
   NiFpga_thumb_switch = 0x8122,
   NiFpga_index_switch= 0x811E,
   NiFpga_middle_switch = 0x811A,
   NiFpga_ring_switch = 0x8116,
   NiFpga_pinkie_switch = 0x8112,
} NiFpga_IndicatorBool;

typedef enum
{
   NiFpga_thumb_flex = 0x8134,
   NiFpga_index_flex = 0x8130,
   NiFpga_middle_flex = 0x812C,
   NiFpga_ring_flex = 0x8128,
   NiFpga_pinkie_flex = 0x8124,
} NiFpga_IndicatorI32;

typedef enum
{
   NiFpga_stop = 0x810E,
} NiFpga_limb_ControlBool;

#endif
