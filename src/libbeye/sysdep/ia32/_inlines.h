/**
 * @namespace   libbeye
 * @file        libbeye/sysdep/ia32/_inlines.h
 * @brief       This file includes 32-bit Intel architecture little inline functions.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Nickols_K
 * @since       2000
 * @note        Development, fixes and improvements
**/
#if defined(_MSC_VER) || defined(__WATCOMC__)
#define inline __inline
#endif

#ifndef ___INLINES_H
#define ___INLINES_H 1

#define COREDUMP() { __asm __volatile(".short 0xffff":::"memory"); }

#endif
