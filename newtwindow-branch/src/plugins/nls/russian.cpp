#include "config.h"
#include "libbeye/libbeye.h"
using namespace	usr;
/**
 * @namespace	usr_plugins_II
 * @file        plugins/nls/russian.c
 * @brief       This file contains support of russian codepages.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Nickols_K
 * @since       1995
 * @note        Development, fixes and improvements
**/
#include <sstream>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "beye.h"
#include "plugins/textmode.h"
#include "bconsole.h"
#include "beyeutil.h"
#include "listbox.h"
#include "libbeye/osdep/system.h"

namespace	usr {
enum {
    TXT_CURRCP      =0,
    TXT_CP866       =1,
    TXT_WIN1251     =2,
    TXT_KOIR8       =3,
    TXT_ISO         =4,
    TXT_CP855       =5,
    TXT_APPLE       =6,
    TXT_UNICODE     =7,
    TXT_BIG_UNICODE =8,
    TXT_EBCDIC      =9,
    TXT_MIK16       =10,
    TXT_GOST87      =11,
    TXT_KOI0        =12,
    TXT_KOI7        =13,
    TXT_KOI8A       =14,
    TXT_KOI8E       =15,
    TXT_KOIR8V2     =16,
    TXT_TRANSLIT    =17,
    TXT_7BIT        =18,

    TXT_MAXMODE     =18
};
static const char * ru_cp_names[] =
{
   "~Current",
   "CP 8~66 (DOS)",
   "CP 1251 (~Windows)",
   "CP 878 (Unix ~KOI8-R)",
   "~ISO-8859-5-Cyrillic",
   "CP ~855",
   "CP 10007 (Apple-~Macintosh)",
   "~Unicode ISO/IEC 10646-1 UCS (UTF-16 little-endian)",
   "Unicode ISO/IEC 10646-1 UCS (UTF-16 ~big-endian)",
   "~EBCDIC",
   "~Bulgarian Prawek 16 PC",
   "~GOST 19768-87",
   "KOI-~0 (GOST 13052)",
   "KOI-~7 (GOST 19768-74)",
   "O~riginal KOI-8 (GOST 19768-74)",
   "~European KOI-8 (ISO-IR-111)",
   "KOI~8-R v2",
   "~Translit of KOI8-R",
   "~View as UUCP 7bit compatible (meaningful only for KOI8-R)"
};


static unsigned cp_mode = TXT_CURRCP;
static unsigned char cp_symb_len = 1;

#if 0
static const uint16_t cp866_2_Unicode[] =
{
  0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, /*0x00*/
  0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
  0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, /*0x10*/
  0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
  0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, /*0x20*/
  0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
  0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, /*0x30*/
  0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
  0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, /*0x40*/
  0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
  0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, /*0x50*/
  0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
  0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, /*0x60*/
  0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
  0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, /*0x60*/
  0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F,

  0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, /*0x80*/
  0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
  0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, /*0x90*/
  0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
  0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, /*0xA0*/
  0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,

  0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, /*0xB0*/
  0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,

  0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, /*0xC0*/
  0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,

  0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, /*0xD0*/
  0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,

  0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, /*0xE0*/
  0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F,
  0x0401, 0x0451, 0x0404, 0x0454, 0x0407, 0x0457, 0x040E, 0x045E, /*0xF0*/
  0x00B0, 0x2022, 0x00B7, 0x221A, 0x2116, 0x00A4, 0x25A0, 0x00A0
};
#endif
static const unsigned char uni_draw[] =
{
0xc4,0x3f,0xb3,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0xda,0x3f,0x3f,0x3f, /*0x00*/
0xbf,0x3f,0x3f,0x3f,0xc0,0x3f,0x3f,0x3f,0xd9,0x3f,0x3f,0x3f,0xc3,0x3f,0x3f,0x3f, /*0x10*/
0x3f,0x3f,0x3f,0x3f,0xb4,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0xc2,0x3f,0x3f,0x3f, /*0x20*/
0x3f,0x3f,0x3f,0x3f,0xc1,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0xc5,0x3f,0x3f,0x3f, /*0x30*/
0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f, /*0x40*/
0xcd,0xba,0xd5,0xd6,0xc9,0xb8,0xb7,0xbb,0xd4,0xd3,0xc8,0xbe,0xbd,0xbc,0xc6,0xc7, /*0x50*/
0xcc,0xb5,0xb6,0xb9,0xd1,0xd2,0xcb,0xcf,0xd0,0xca,0xd8,0xd7,0xce,0x3f,0x3f,0x3f, /*0x60*/
0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f, /*0x70*/
0xdf,0x3f,0x3f,0x3f,0xdc,0x3f,0x3f,0x3f,0xdb,0x3f,0x3f,0x3f,0xdd,0x3f,0x3f,0x3f, /*0x80*/
0xde,0xb0,0xb1,0xb2,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f, /*0x90*/
0xfe,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f, /*0xA0*/
0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f, /*0xB0*/
0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f, /*0xC0*/
0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f, /*0xD0*/
0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f, /*0xE0*/
0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f, /*0xF0*/
};

static void  __FASTCALL__ txt_cvt_unicode(unsigned char * str,int size,bool big_endian)
{
  int i;
  unsigned high_byte,low_byte;
  size >>= 1;
  for(i = 0;i < size;i++)
  {
    if(big_endian)
    {
      low_byte = str[(i<<1)+1];
      high_byte  = str[(i<<1)];
    }
    else
    {
      high_byte = str[(i<<1)+1];
      low_byte  = str[(i<<1)];
    }
    switch(high_byte)
    {
      case 0x00:  /** Basic latin & latin 1 supplement zone */
		    if(low_byte < 0x80) str[i] = low_byte;
		    else
		      if(low_byte == 0xA0) str[i] = 0xFF;
		      else
			if(low_byte == 0xB7) str[i] = 0xFD;
			else
			  if(low_byte == 0xB0) str[i] = 0xF8;
			  else
			    if(low_byte == 0xB7) str[i] = 0xFA;
			    else           str[i] = '?';
		  break;
      case 0x04:  /** Cyrillic zone */
		  if(low_byte >= 0x10 && low_byte < 0x40) str[i] = (unsigned char)(low_byte + 0x80 - 0x410);
		  else
		    if(low_byte >= 0x40 && low_byte < 0x50) str[i] = (unsigned char)(low_byte + 0xE0 - 0x440);
		    else str[i] = '?';
		  break;
      case 0x20:
		  if(low_byte == 0x22) str[i] = 0xF9;
		  else           str[i] = '?';
		  break;
      case 0x21:
		  if(low_byte == 0x16) str[i] = 0xFC;
		  else           str[i] = '?';
		  break;
      case 0x22:
		  if(low_byte == 0x1A) str[i] = 0xFB;
		  else           str[i] = '?';
		  break;
      case 0x25: /** Box draw zone */
		  str[i] = uni_draw[low_byte];
		  break;
      default:    str[i] = '?';
    }
  }
}

static const unsigned char e2a[256] = {
0x00,0x01,0x02,0x03,0x9c,0x09,0x86,0x7f,0x97,0x8d,0x8e,0x0b,0x0c,0x0d,0x0e,0x0f,
0x10,0x11,0x12,0x13,0x9d,0x85,0x08,0x87,0x18,0x19,0x92,0x8f,0x1c,0x1d,0x1e,0x1f,
0x80,0x81,0x82,0x83,0x84,0x0a,0x17,0x1b,0x88,0x89,0x8a,0x8b,0x8c,0x05,0x06,0x07,
0x90,0x91,0x16,0x93,0x94,0x95,0x96,0x04,0x98,0x99,0x9a,0x9b,0x14,0x15,0x9e,0x1a,
0x20,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0x5b,0x2e,0x3c,0x28,0x2b,0x21,
0x26,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0x5d,0x24,0x2a,0x29,0x3b,0x5e,
0x2d,0x2f,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0x7c,0x2c,0x25,0x5f,0x3e,0x3f,
0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0x60,0x3a,0x23,0x40,0x27,0x3d,0x22,
0xc3,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0xc0,0xc5,0xc6,0xc7,0xc8,0xc9,
0xca,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,
0xd1,0x7e,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,
0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
0x7b,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0xe8,0xe9,0xea,0xeb,0xec,0xed,
0x7d,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0xee,0xef,0xf0,0xf1,0xf2,0xf3,
0x5c,0x9f,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,
0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0xfa,0xfb,0xfc,0xfd,0xfe,0xff
};

static const unsigned char Win2Alt[] = {
0xf4,0xc3,0xf0,0xf6,0xf1,0xf5,0xf3,0xf2,0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xf7,0xc4,0xc5,0xc6,0xc7,
0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,
0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef
};

static const unsigned char koi8r2AltV2[] = { /** KOI8-R to AltDOS v2 */
0xf4,0xc3,0xf0,0xf6,0xf1,0xf5,0xf3,0xf2,0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xf7,0xc4,0xc5,0xc6,0xc7,
0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,
0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
0xee,0xa0,0xa1,0xe6,0xa4,0xa5,0xe4,0xa3,0xe5,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,
0xaf,0xef,0xe0,0xe1,0xe2,0xe3,0xa6,0xa2,0xec,0xeb,0xa7,0xe8,0xed,0xe9,0xe7,0xea,
0x9e,0x80,0x81,0x96,0x84,0x85,0x94,0x83,0x95,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,
0x8f,0x9f,0x90,0x91,0x92,0x93,0x86,0x82,0x9c,0x9b,0x87,0x98,0x9d,0x99,0x97,0x9a
};

static const unsigned char koi8r2Alt[] = { /** by Chernov 1994 */
0xc4,0xb3,0xda,0xbf,0xc0,0xd9,0xc3,0xb4,0xc2,0xc1,0xc5,0xdf,0xdc,0xdb,0xdd,0xde,
0xb0,0xb1,0xb2,0xf4,0xfe,0xf9,0xfb,0xf7,0xf3,0xf2,0xff,0xf5,0xf8,0xfd,0xfa,0xf6,
0xcd,0xba,0xd5,0xf1,0xd6,0xc9,0xb8,0xb7,0xbb,0xd4,0xd3,0xc8,0xbe,0xbd,0xbc,0xc6,
0xc7,0xcc,0xb5,0xf0,0xb6,0xb9,0xd1,0xd2,0xcb,0xcf,0xd0,0xca,0xd8,0xd7,0xce,0xfc,
0xee,0xa0,0xa1,0xe6,0xa4,0xa5,0xe4,0xa3,0xe5,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,
0xaf,0xef,0xe0,0xe1,0xe2,0xe3,0xa6,0xa2,0xec,0xeb,0xa7,0xe8,0xed,0xe9,0xe7,0xea,
0x9e,0x80,0x81,0x96,0x84,0x85,0x94,0x83,0x95,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,
0x8f,0x9f,0x90,0x91,0x92,0x93,0x86,0x82,0x9c,0x9b,0x87,0x98,0x9d,0x99,0x97,0x9a
};

static const unsigned char mac2Alt[128] = {
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F, /*0x80*/
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, /*0x90*/
0xDD,0xDE,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF, /*0xA0*/
0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, /*0xB0*/
0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, /*0xC0*/
0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xF0,0xF1,0xEF, /*0xD0*/
0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, /*0xE0*/
0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xDF  /*0xF0*/
};

static const unsigned char cp855Alt[128] = {
 0x80,0x81,0x66,0x46,0xF1,0xF0,0x86,0x87,0x73,0x53,0x69,0x49,0x8C,0x8D,0x8E,0x8F, /*0x80*/
 0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0xEE,0x9E,0xEA,0x9A, /*0x90*/
 0xA0,0x80,0xA1,0x81,0xE6,0x96,0xA4,0x84,0xA5,0x85,0xE4,0x94,0xA3,0x83,0xAE,0xAF, /*0xA0*/
 0xB0,0xB1,0xB2,0xB3,0xB4,0xE5,0x95,0xA8,0x88,0xB9,0xBA,0xBB,0xBC,0xA9,0x89,0xBF, /*0xB0*/
 0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xAA,0x8A,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, /*0xC0*/
 0xAB,0x8B,0xAC,0x8C,0xAD,0x8D,0xAE,0x8E,0xAF,0xD9,0xDA,0xDB,0xDC,0x8F,0xEF,0xDF, /*0xD0*/
 0x9F,0xE0,0x90,0xE1,0x91,0xE2,0x92,0xE3,0x93,0xA6,0x86,0xA2,0x82,0xEC,0x9C,0xEF, /*0xE0*/
 0xF0,0xEB,0x9B,0xA7,0x87,0xE8,0x98,0xED,0x9D,0xE9,0x99,0xE7,0x97,0xFD,0xFE,0xFF, /*0xF0*/
};

static const unsigned char ISO2Alt[] = {
0xf4,0xc3,0xf0,0xf6,0xf1,0xf5,0xf3,0xf2,0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xf7,0xc4,0xc5,0xc6,0xc7,
0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff
};

static const unsigned char koi0Alt[128] = {
 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f, /*0x00*/
 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f, /*0x10*/
 0x20,0x21,0x22,0x23,0xfd,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f, /*0x20*/
 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f, /*0x30*/
 0xee,0xa0,0xa1,0xe6,0xa4,0xa5,0xe4,0xa3,0xe5,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae, /*0x40*/
 0xaf,0xef,0xe0,0xe1,0xe2,0xe3,0xa6,0xa2,0xec,0xeb,0xa7,0xe8,0xed,0xe9,0xe7,0xea, /*0x50*/
 0x9e,0x80,0x81,0x96,0x84,0x85,0x94,0x83,0x95,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e, /*0x60*/
 0x8f,0x9f,0x90,0x91,0x92,0x93,0x86,0x82,0x9c,0x9b,0x87,0x98,0x9d,0x99,0x97,0x7F, /*0x70*/
};

static const unsigned char koi7Alt[128] = {
 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f, /*0x00*/
 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f, /*0x10*/
 0x20,0x21,0x22,0x23,0xfd,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f, /*0x20*/
 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f, /*0x30*/
 0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f, /*0x40*/
 0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f, /*0x50*/
 0x9e,0x80,0x81,0x96,0x84,0x85,0x94,0x83,0x95,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e, /*0x60*/
 0x8f,0x9f,0x90,0x91,0x92,0x93,0x86,0x82,0x9c,0x9b,0x87,0x98,0x9d,0x99,0x97,0x7F, /*0x70*/
};

static const unsigned char koi8a2Alt[128] = {
 0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f, /*0x80*/
 0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f, /*0x90*/
 0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf, /*0xa0*/
 0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf, /*0xb0*/
 0xee,0xa0,0xa1,0xe6,0xa4,0xa5,0xe4,0xa3,0xe5,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae, /*0xc0*/
 0xaf,0xef,0xe0,0xe1,0xe2,0xe3,0xa6,0xa2,0xec,0xeb,0xa7,0xe8,0xed,0xe9,0xe7,0xea, /*0xd0*/
 0x9e,0x80,0x81,0x96,0x84,0x85,0x94,0x83,0x95,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e, /*0xe0*/
 0x8f,0x9f,0x90,0x91,0x92,0x93,0x86,0x82,0x9c,0x9b,0x87,0x98,0x9d,0x99,0x97,0xFF, /*0xf0*/
};

static const unsigned char koi8e2Alt[128] = {
 0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f, /*0x80*/
 0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f, /*0x90*/
 0xa0,0xa1,0xa2,0xf1,0xf2,0x73,0x69,0xf5,0x6a,0xa9,0xaa,0xab,0xac,0xad,0xf7,0xaf, /*0xa0*/
 0xfc,0xb1,0xb2,0xf0,0xf3,0x53,0x49,0xf4,0x4a,0xb9,0xba,0xbb,0xbc,0xfd,0xf6,0xbf, /*0xb0*/
 0xee,0xa0,0xa1,0xe6,0xa4,0xa5,0xe4,0xa3,0xe5,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae, /*0xc0*/
 0xaf,0xef,0xe0,0xe1,0xe2,0xe3,0xa6,0xa2,0xec,0xeb,0xa7,0xe8,0xed,0xe9,0xe7,0xea, /*0xd0*/
 0x9e,0x80,0x81,0x96,0x84,0x85,0x94,0x83,0x95,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e, /*0xe0*/
 0x8f,0x9f,0x90,0x91,0x92,0x93,0x86,0x82,0x9c,0x9b,0x87,0x98,0x9d,0x99,0x97,0x9A, /*0xf0*/
};

static const unsigned char gost87Alt[128] = {
 0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f, /*0x80*/
 0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f, /*0x90*/
 0xa0,0xf0,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf, /*0xa0*/
 0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f, /*0x80*/
 0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f, /*0x90*/
 0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf, /*0xa0*/
 0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef, /*0xe0*/
 0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff, /*0xf0*/
};

static const unsigned char Mik16Alt[128] = {
 0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f, /*0x80*/
 0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f, /*0x90*/
 0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf, /*0xa0*/
 0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef, /*0xb0*/
 0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf, /*0xc0*/
 0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf, /*0xd0*/
 0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf, /*0xe0*/
 0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff, /*0xf0*/
};

static void  __FASTCALL__ Unix7Bit(char *str,int size)
{
  int i;
  for(i = 0;i < size;i++) str[i] &= 0x7F;
}

static void  __FASTCALL__ Translit7Bit(char *str,int size)
{
  int i;
  for(i = 0;i < size;i++)
  {
    unsigned char ch;
    ch = str[i];
    switch(ch) /** need some correcting for normal viewing translit */
    {
       case 'p': str[i] = '�'; break;
       case '@': str[i] = '�'; break;
       case '\\':str[i] = '�'; break;
       case ']': str[i] = '�'; break;
       case '[': str[i] = '�'; break;
       case '^': str[i] = '�'; break;
       case '|': str[i] = '�'; break;
       case '~': str[i] = '�'; break;
       case '{': str[i] = '�'; break;
       case '}': str[i] = '�'; break;
       case '_': str[i] = '�'; break;
       default: if(isalpha((unsigned char)str[i])) str[i] |= 0x80; txt_cvt_hi80(&str[i],1,koi8r2Alt);
    }
  }
}

static unsigned __FASTCALL__ ru_convert_buffer(char *buff,unsigned size, bool use_fs_nls)
{
  switch(cp_mode)
  {
      case TXT_7BIT:    Unix7Bit(buff,size); break;
      case TXT_KOI0:    txt_cvt_lo80(buff,size,koi0Alt); break;
      case TXT_KOI7:    txt_cvt_lo80(buff,size,koi7Alt); break;
      case TXT_KOI8A:   txt_cvt_hi80(buff,size,koi8a2Alt); break;
      case TXT_KOI8E:   txt_cvt_hi80(buff,size,koi8e2Alt); break;
      case TXT_GOST87:  txt_cvt_hi80(buff,size,gost87Alt); break;
      case TXT_WIN1251: txt_cvt_hi80(buff,size,Win2Alt); break;
      case TXT_MIK16:   txt_cvt_hi80(buff,size,Mik16Alt); break;
      case TXT_KOIR8:   txt_cvt_hi80(buff,size,koi8r2Alt); break;
      case TXT_KOIR8V2: txt_cvt_hi80(buff,size,koi8r2AltV2); break;
      case TXT_APPLE:   txt_cvt_hi80(buff,size,mac2Alt); break;
      case TXT_ISO:     txt_cvt_hi80(buff,size,ISO2Alt); break;
      case TXT_CP855:   txt_cvt_hi80(buff,size,cp855Alt); break;
      case TXT_EBCDIC:  txt_cvt_full(buff,size,e2a); break;
      case TXT_BIG_UNICODE:
      case TXT_UNICODE: txt_cvt_unicode((unsigned char *)buff,size,cp_mode == TXT_BIG_UNICODE);
			break;
      case TXT_TRANSLIT:Translit7Bit(buff,size); break;
  };
  if(cp_mode != TXT_CURRCP)
  {
    if(use_fs_nls) beye_context().system().nls_oem2fs((unsigned char *)buff,size);
    else           beye_context().system().nls_oem2osdep((unsigned char *)buff,size);
  }
  return size / cp_symb_len;
}

static unsigned __FASTCALL__ ru_get_symbol_size() { return cp_symb_len; }

static bool __FASTCALL__ ru_select_table()
{
    unsigned nModes;
    int i;
    nModes = sizeof(ru_cp_names)/sizeof(char *);
    ListBox lb(beye_context());
    i = lb.run(ru_cp_names,nModes," Select cyrillic code page: ",ListBox::Selective|ListBox::UseAcc,cp_mode);
    if(i != -1) {
	cp_mode = i;
	if(cp_mode == TXT_UNICODE || cp_mode == TXT_BIG_UNICODE) cp_symb_len = 2;
	else                                                     cp_symb_len = 1;
	return true;
    }
    return false;
}

static void __FASTCALL__ ru_read_ini( Ini_Profile& ini )
{
    std::string tmps;
    if(beye_context().is_valid_ini_args()) {
	tmps=beye_context().read_profile_string(ini,"Beye","Browser","LastSubMode","0");
	std::istringstream is(tmps);
	is>>cp_mode;
	if(cp_mode > TXT_MAXMODE) cp_mode = 0;
    }
    if(cp_mode == TXT_UNICODE || cp_mode == TXT_BIG_UNICODE) cp_symb_len = 2;
    else                                                     cp_symb_len = 1;
}

static void __FASTCALL__ ru_save_ini( Ini_Profile& ini )
{
    std::ostringstream os;
    os<<cp_mode;
    beye_context().write_profile_string(ini,"Beye","Browser","LastSubMode",os.str());
}

extern const REGISTRY_NLS RussianNLS =
{
  "Russian set",
  ru_convert_buffer,
  ru_get_symbol_size,
  ru_select_table,
  ru_read_ini,
  ru_save_ini,
  NULL,
  NULL
};
} // namespace	usr

