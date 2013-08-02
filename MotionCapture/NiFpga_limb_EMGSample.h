/*
 * Generated with the FPGA Interface C API Generator 12.0.0
 * for NI-RIO 12.0.0 or later.
 */

#ifndef __NiFpga_limb_EMGSample_h__
#define __NiFpga_limb_EMGSample_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1200
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_limb_EMGSample_Bitfile;
 */
#define Bitfile "NiFpga_limb_EMGSample.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_Signature = "13327495B39610C453C3CFBED56CB5A1";

typedef enum
{
   NiFpga_stop = 0x8112,
} NiFpga_limb_EMGSample_ControlBool;

typedef enum
{
   NiFpga_EMGSample_Array = 0x810C,
} NiFpga_limb_EMGSample_IndicatorArrayU32;

typedef enum
{
   NiFpga_Size_AnalogIn = 2,
} NiFpga_limb_EMGSample_IndicatorArrayU32Size;

#endif
