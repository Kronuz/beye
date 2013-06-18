#include "config.h"
#include "libbeye/libbeye.h"
using namespace	usr;
#include "libbeye/osdep/__os_dep.h"
/**
 * @namespace   libbeye
 * @file        libbeye/sysdep/x86_64/cpu_info.c
 * @brief       This file contains function for retrieving CPU information for
 *              64-bit AMD x86 compatible platform
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 * @remark      I used such form of this file because of build-in assembler
 *              allow write calling convention independed code. In addition,
 *              GNU C compiler is ported under multiple OS's. If somebody will
 *              port it under ABC-xyz platform, then more easy find compiler
 *              with build-in assembler, instead rewriting of makefile with .s
 *              (or .asm) extensions for choosen development system.
 *
 * @author      Nickols_K
 * @since       2009
 * @note        Development, fixes and improvements
**/
#include <stdio.h>
#include <string.h>

#define CPU_CLONE     0x000F
#define __HAVE_FPU    0x8000
#define __HAVE_CPUID  0x4000
#define __HAVE_MMX    0x2000
#define __HAVE_SSE    0x1000

#if !defined(__DISABLE_ASM) && defined(__GNUC__)

static inline void do_cpuid(unsigned int ax, unsigned int *p)
{
	__asm __volatile(
	"cpuid"
	: "=a" (p[0]), "=b" (p[1]), "=c" (p[2]), "=d" (p[3])
	:  "0" (ax)
	);
}


static unsigned  __FASTCALL__ __cpu_type()
{
   return 8|__HAVE_CPUID|__HAVE_MMX|__HAVE_SSE|__HAVE_FPU;
}

static void  __FASTCALL__ __cpu_name(char *buff)
{
    unsigned int p[4];
    do_cpuid(0,p);
    memcpy(&buff[0],&p[1],4);
    memcpy(&buff[4],&p[3],4);
    memcpy(&buff[8],&p[2],4);
    buff[12]='\0';
}

static void  __FASTCALL__ __extended_name(char *buff)
{
/*
    unsigned int p[4];
    do_cpuid(0x80000002,p);
    memcpy(&buff[0], &p[0],4);
    memcpy(&buff[4], &p[1],4);
    memcpy(&buff[8], &p[2],4);
    memcpy(&buff[12],&p[3],4);
    do_cpuid(0x80000003,p);
    memcpy(&buff[16],&p[0],4);
    memcpy(&buff[20],&p[1],4);
    memcpy(&buff[24],&p[2],4);
    memcpy(&buff[28],&p[3],4);
    do_cpuid(0x80000004,p);
    memcpy(&buff[32],&p[0],4);
    memcpy(&buff[36],&p[1],4);
    memcpy(&buff[40],&p[2],4);
    memcpy(&buff[44],&p[3],4);
    buff[48]='\0';
*/
   __asm __volatile("movl	$0x80000002, %%eax\n"
      "cpuid\n"
      "	stosl\n"
      "	movl	%%ebx, %%eax\n"
      "	stosl\n"
      "	movl	%%ecx, %%eax\n"
      "	stosl\n"
      "	movl	%%edx, %%eax\n"
      "	stosl\n"
      "	movl	$0x80000003, %%eax\n"
      "cpuid\n"
      "	stosl\n"
      "	movl	%%ebx, %%eax\n"
      "	stosl\n"
      "	movl	%%ecx, %%eax\n"
      "	stosl\n"
      "	movl	%%edx, %%eax\n"
      "	stosl\n"
      "	movl	$0x80000004, %%eax\n"
      "cpuid\n"
      "	stosl\n"
      "	movl	%%ebx, %%eax\n"
      "	stosl\n"
      "	movl	%%ecx, %%eax\n"
      "	stosl\n"
      "	movl	%%edx, %%eax\n"
      "	stosl\n"
      "	xorb	%%al, %%al\n"
      "	stosb\n"	:
			:
      "D"(buff)		:
      "eax","ebx","ecx","edx");
}

static unsigned long  __FASTCALL__ __cpuid_edx(unsigned long *__r_eax)
{
  unsigned int p[4];
  do_cpuid(*__r_eax,p);
  *__r_eax=p[0];
  return p[3];
}

static unsigned long  __FASTCALL__ __cpuid_ebxecx(unsigned long *__r_eax)
{
  unsigned int p[4];
  do_cpuid(*__r_eax,p);
  *__r_eax=p[2];
  return p[1];
}

static unsigned  __FASTCALL__ __fpu_type()
{
   return 8;
}

static unsigned long  __FASTCALL__ __OPS_nop(volatile unsigned *time_val)
{
  register unsigned long retval;
   __asm __volatile(
"1:\n"
      "	cmpl	$0, (%1)\n"
      "	jz	1b\n"
"2:\n"
      "	cmpl	$0, (%1)\n"
      "	jz	3f\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
".byte  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90\n"
      "	inc	%0\n"
      "	jmp	2b\n"
"3:"			:
      "=a"(retval)	:
      "r"(time_val),
      "0"(0));
  return retval;
}
static unsigned long  __FASTCALL__ __OPS_std(volatile unsigned *counter,char *arr8byte)
{
  unsigned long retval;
  register long long dummy;
   retval=0;
   while(*counter==0);
   while(*counter!=0){
   __asm __volatile(
      "pushq	%0\n"

      "movq	$0x14, %0\n"
      "movq	$0x07, %%rcx\n"
      "mulq	%%rcx\n"
      "imulq	%%rcx\n"
      "divq	%%rcx\n"
      "idivq	%%rcx\n"
      "addq	%%rcx, %0\n"
      "adcq	$0x01, %0\n"
      "subq	%%rcx, %0\n"
      "sbbq	$0x01, %0\n"
      "pushq	%2\n"
      "pushq	%1\n"
      "movq	%2, %2\n"
//      "movsq\n"
//      "cmpsq\n"
      "popq	%1\n"
      "popq	%2\n"
      "pushq	%0\n"
      "pushq	%%rdx\n"
      "popq	%%rdx\n"
      "popq	%0\n"
      "movq	$0x14, %0\n"
      "movq	$0x07, %%rcx\n"
      "mulq	%%rcx\n"
      "imulq	%%rcx\n"
      "divq	%%rcx\n"
      "idivq	%%rcx\n"
      "addq	%%rcx, %0\n"
      "adcq	$0x01, %0\n"
      "subq	%%rcx, %0\n"
      "sbbq	$0x01, %0\n"
      "pushq	%2\n"
      "pushq	%1\n"
      "movq	%2, %2\n"
//      "movsq\n"
//      "cmpsq\n"
      "popq	%1\n"
      "popq	%2\n"
      "pushq	%0\n"
      "pushq	%%rdx\n"
      "popq	%%rdx\n"
      "popq	%0\n"
      "movq	$0x14, %0\n"
      "movq	$0x07, %%rcx\n"
      "mulq	%%rcx\n"
      "imulq	%%rcx\n"
      "divq	%%rcx\n"
      "idivq	%%rcx\n"
      "addq	%%rcx, %0\n"
      "adcq	$0x01, %0\n"
      "subq	%%rcx, %0\n"
      "sbbq	$0x01, %0\n"
      "pushq	%2\n"
      "pushq	%1\n"
      "movq	%2, %2\n"
//      "movsq\n"
//      "cmpsq\n"
      "popq	%1\n"
      "popq	%2\n"
      "pushq	%0\n"
      "pushq	%%rdx\n"
      "popq	%%rdx\n"
      "popq	%0\n"
      "movq	$0x14, %0\n"
      "movq	$0x07, %%rcx\n"
      "mulq	%%rcx\n"
      "imulq	%%rcx\n"
      "divq	%%rcx\n"
      "idivq	%%rcx\n"
      "adcq	$0x01, %0\n"
      "subq	%%rcx, %0\n"
      "sbbq	$0x01, %0\n"
      "pushq	%%rdx\n"
      "popq	%%rdx\n"
      "popq	%0\n"
:"=r"(dummy)
:"S"(counter),"D"(arr8byte)
:"rcx","rdx","cc");
     retval++;
  }
  return retval;
}

static unsigned long  __FASTCALL__ __FOPS_nowait(volatile unsigned *counter,char *arr18bytes)
{
  register unsigned long retval,dummy;
   retval=0;
   while(*counter==0);
   while(*counter!=0){
   __asm __volatile(
      "	fninit\n"
      "	fldt	8(%1)\n"
      "	fstpt	8(%1)\n"
      "	fstp	%%st(0)\n"
      "	fldz\n"
      "	fld1\n"
      "	fcompp\n"
      "	fnstsw	4(%1)\n"
      "	fnstcw	(%1)\n"
      "	fldcw	(%1)\n"
      "	fldpi\n"
      "	fstp	%%st(1)\n"
      "	fst	%%st(2)\n"
      "	fst	%%st(3)\n"
      "	f2xm1\n"
      "	fabs\n"
      "	fchs\n"
      "	fprem\n"
      "	fptan\n"
      "	fsqrt\n"
      "	frndint\n"
      "	faddp	%%st,%%st(1)\n"
      "	fstp	%%st(1)\n"
      "	fmulp	%%st,%%st(1)\n"
      "	fstp	%%st(1)\n"
      "	fld1\n"
      "	fstp	%%st(1)\n"
      "	fpatan\n"
      "	fstp	%%st(1)\n"
      "	fscale\n"
      "	fstp	%%st(1)\n"
      "	fdivrp	%%st, %%st(1)\n"
      "	fstp	%%st(1)\n"
      "	fsubp	%%st, %%st(1)\n"
      "	fstp	%%st(1)\n"
      "	fyl2x\n"
      "	fstp	%%st(1)\n"
      "	fyl2xp1\n"
      "	fstp	%%st(1)\n"
      "	fbld	8(%1)\n"
      "	fbstp	8(%1)\n"
      "	fild	(%1)\n"
      "	fistp	(%1)\n"
      "	fldt	4(%1)\n"
      "	fstpt	4(%1)\n"
      "	fstp	%%st(1)\n"
      "	fldz\n"
      "	fld1\n"
      "	fcompp\n"
      "	fnstsw	4(%1)\n"
      "	fnstcw	(%1)\n"
      "	fldcw	(%1)\n"
      "	fldpi\n"
      "	fstp	%%st(1)\n"
      "	fst	%%st(2)\n"
      "	fst	%%st(3)\n"
      "	f2xm1\n"
      "	fabs\n"
      "	fchs\n"
      "	fprem\n"
      "	fptan\n"
      "	fsqrt\n"
      "	frndint\n"
      "	faddp	%%st,%%st(1)\n"
      "	fstp	%%st(1)\n"
      "	fmulp	%%st,%%st(1)\n"
      "	fstp	%%st(1)\n"
      "	fld1\n"
      "	fstp	%%st(1)\n"
      "	fpatan\n"
      "	fstp	%%st(1)\n"
      "	fscale\n"
      "	fstp	%%st(1)\n"
      "	fdivrp	%%st, %%st(1)\n"
      "	fstp	%%st(1)\n"
      "	fsubp	%%st, %%st(1)\n"
      "	fstp	%%st(1)\n"
      "	fyl2x\n"
      "	fstp	%%st(1)\n"
      "	fyl2xp1\n"
      "	fstp	%%st(1)\n"
      "	fild	(%1)\n"
      "	fistp	(%1)"
     ::"r"(counter),"r"(arr18bytes)
     :"st","st(1)","st(2)","st(3)");
     retval++;
  }
  return retval;
}

static unsigned long  __FASTCALL__ __FOPS_w_wait(volatile unsigned *counter,char *arr14bytes)
{
  return __FOPS_nowait(counter,arr14bytes);
}

static unsigned long  __FASTCALL__ __MOPS_std(volatile unsigned *counter,char *arr)
{
  register unsigned long retval;
   retval=0;
   while(*counter==0);
   while(*counter!=0){
    __asm __volatile(
"movd	%0,%%mm0\n"
"packssdw %%mm0,%%mm5 \n"
"packsswb %%mm0,%%mm4 \n"
"packuswb %%mm0,%%mm7 \n"
"paddb    %%mm3,%%mm2 \n"
"paddd    %%mm5,%%mm1 \n"
"psubsb   %%mm4,%%mm6 \n"
"psubusb  %%mm3,%%mm4 \n"
"pand     %%mm1,%%mm3 \n"
"pcmpeqd  %%mm0,%%mm0 \n"
"pcmpgtb  %%mm2,%%mm2 \n"
"pmaddwd  %%mm7,%%mm7 \n"
"pmullw   %%mm6,%%mm6 \n"
"por      %%mm2,%%mm4 \n"
"psllq    %%mm0,%%mm6 \n"
"psrad    %%mm1,%%mm3 \n"
"psubb    %%mm1,%%mm1 \n"
"psubsw   %%mm2,%%mm7 \n"
"psubusw  %%mm3,%%mm1 \n"
"punpckhdq %%mm0,%%mm4 \n"
"punpcklwd %%mm2,%%mm0 \n"
"pxor     %%mm4,%%mm2 \n"
"packssdw %%mm0,%%mm5 \n"
"packsswb %%mm0,%%mm4 \n"
"packuswb %%mm0,%%mm7 \n"
"paddb    %%mm3,%%mm2 \n"
"paddd    %%mm5,%%mm1 \n"
"psubsb   %%mm4,%%mm6 \n"
"psubusb  %%mm3,%%mm4 \n"
"pand     %%mm1,%%mm3 \n"
"pcmpeqd  %%mm0,%%mm0 \n"
"pcmpgtb  %%mm2,%%mm2 \n"
"pmaddwd  %%mm7,%%mm7 \n"
"pmullw   %%mm6,%%mm6 \n"
"por      %%mm2,%%mm4 \n"
"psllq    %%mm0,%%mm6 \n"
"psrad    %%mm1,%%mm3 \n"
"psubb    %%mm1,%%mm1 \n"
"psubsw   %%mm2,%%mm7 \n"
"psubusw  %%mm3,%%mm1 \n"
"punpckhdq %%mm0,%%mm4 \n"
"punpcklwd %%mm2,%%mm0 \n"
"pxor     %%mm4,%%mm2 \n"
"movd     %0,%%mm0 \n"
"packssdw %%mm0,%%mm5 \n"
"packsswb %%mm0,%%mm4 \n"
"packuswb %%mm0,%%mm7 \n"
"paddb    %%mm3,%%mm2 \n"
"paddd    %%mm5,%%mm1 \n"
"psubsb   %%mm4,%%mm6 \n"
"psubusb  %%mm3,%%mm4 \n"
"pand     %%mm1,%%mm3 \n"
"pcmpeqd  %%mm0,%%mm0 \n"
"pcmpgtb  %%mm2,%%mm2 \n"
"pmaddwd  %%mm7,%%mm7 \n"
"pmullw   %%mm6,%%mm6 \n"
"por      %%mm2,%%mm4 \n"
"psllq    %%mm0,%%mm6 \n"
"psrad    %%mm1,%%mm3 \n"
"psubb    %%mm1,%%mm1 \n"
"psubsw   %%mm2,%%mm7 \n"
"psubusw  %%mm3,%%mm1 \n"
"punpckhdq %%mm0,%%mm4 \n"
"punpcklwd %%mm2,%%mm0 \n"
"pxor     %%mm4,%%mm2 \n"
"packssdw %%mm0,%%mm5 \n"
"packsswb %%mm0,%%mm4 \n"
"packuswb %%mm0,%%mm7 \n"
"paddb    %%mm3,%%mm2 \n"
"paddd    %%mm5,%%mm1 \n"
"psubsb   %%mm4,%%mm6 \n"
"psubusb  %%mm3,%%mm4 \n"
"pand     %%mm1,%%mm3 \n"
"pcmpeqd  %%mm0,%%mm0 \n"
"pcmpgtb  %%mm2,%%mm2 \n"
"pmaddwd  %%mm7,%%mm7 \n"
"pmullw   %%mm6,%%mm6 \n"
"por      %%mm2,%%mm4 \n"
"psllq    %%mm0,%%mm6 \n"
"psrad    %%mm1,%%mm3 \n"
"psubb    %%mm1,%%mm1 \n"
"psubsw   %%mm2,%%mm7 \n"
"psubusw  %%mm3,%%mm1 \n"
"punpckhdq %%mm0,%%mm4 \n"
"punpcklwd %%mm2,%%mm0 \n"
"pxor     %%mm4,%%mm2"
  ::"r"(retval)
  :"mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7");
  retval++;
  }
  return retval;
}

static unsigned long  __FASTCALL__ __SSEOPS_std(volatile unsigned *counter,char *arr)
{
  register unsigned long retval;
   retval=0;
   while(*counter==0);
   while(*counter!=0){
    __asm __volatile(
"movaps   (%0), %%xmm0 \n"
"movhps   (%0),%%xmm1 \n"
"movlps   (%0),%%xmm2 \n"
"movups   (%0),%%xmm3 \n"
"addps    %%xmm0,%%xmm1 \n"
"addss    %%xmm0,%%xmm1 \n"
"cvtps2pi %%xmm1,%%mm0 \n"
"cvttps2pi %%xmm2,%%mm1 \n"
"maxps    %%xmm0,%%xmm1 \n"
"maxss    %%xmm0,%%xmm1 \n"
"minps    %%xmm0,%%xmm1 \n"
"minss    %%xmm0,%%xmm1 \n"
"mulps    %%xmm0,%%xmm1 \n"
"mulss    %%xmm0,%%xmm1 \n"
"andps    %%xmm0,%%xmm1 \n"
"orps     %%xmm0,%%xmm1 \n"
"xorps    %%xmm1,%%xmm1 \n"
"divps    %%xmm0,%%xmm1 \n"
"divss    %%xmm0,%%xmm1 \n"
"rcpps    %%xmm0,%%xmm1 \n"
"rcpss    %%xmm0,%%xmm1 \n"
"rsqrtps  %%xmm0,%%xmm1 \n"
"rsqrtss  %%xmm0,%%xmm1 \n"
"sqrtps   %%xmm0,%%xmm1 \n"
"sqrtss   %%xmm0,%%xmm1 \n"
"subps    %%xmm0,%%xmm1 \n"
"subss    %%xmm0,%%xmm1 \n"
"ucomiss  %%xmm0,%%xmm1 \n"
"unpckhps %%xmm0,%%xmm1 \n"
"unpcklps %%xmm0,%%xmm1 \n"
"movaps   (%0),%%xmm0 \n"
"movhps   (%0),%%xmm1 \n"
"movlps   (%0),%%xmm2 \n"
"movups   (%0),%%xmm3 \n"
"addps    %%xmm0,%%xmm1 \n"
"addss    %%xmm0,%%xmm1 \n"
"cvtps2pi %%xmm1,%%mm0 \n"
"cvttps2pi %%xmm2,%%mm1 \n"
"maxps    %%xmm0,%%xmm1 \n"
"maxss    %%xmm0,%%xmm1 \n"
"minps    %%xmm0,%%xmm1 \n"
"minss    %%xmm0,%%xmm1 \n"
"mulps    %%xmm0,%%xmm1 \n"
"mulss    %%xmm0,%%xmm1 \n"
"andps    %%xmm0,%%xmm1 \n"
"orps     %%xmm0,%%xmm1 \n"
"xorps    %%xmm1,%%xmm1 \n"
"divps    %%xmm0,%%xmm1 \n"
"divss    %%xmm0,%%xmm1 \n"
"rcpps    %%xmm0,%%xmm1 \n"
"rcpss    %%xmm0,%%xmm1 \n"
"rsqrtps  %%xmm0,%%xmm1 \n"
"rsqrtss  %%xmm0,%%xmm1 \n"
"sqrtps   %%xmm0,%%xmm1 \n"
"sqrtss   %%xmm0,%%xmm1 \n"
"subps    %%xmm0,%%xmm1 \n"
"subss    %%xmm0,%%xmm1 \n"
"ucomiss  %%xmm0,%%xmm1 \n"
"unpckhps %%xmm0,%%xmm1 \n"
"unpcklps %%xmm0,%%xmm1 \n"
::"r"(arr)
:"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7");
  retval++;
  }
  return retval;
}

#define __ASMPART_DEFINED 1
#include "libbeye/sysdep/ia32/cmn_ix86.cpp"

#elif defined(__WATCOMC__) && __WATCOMC__ >= 1100

#include "libbeye/sysdep/x86_64/cpu_info.wc"

#define __ASMPART_DEFINED 1
#include "libbeye/sysdep/ia32/cmn_ix86.cpp"

#elif defined(__WATCOMC__) && defined(__QNX4__)

#include "libbeye/sysdep/x86_64/qnx/cpu_info.qnx"

#define __ASMPART_DEFINED 1
#include "libbeye/sysdep/ia16/cmn_ix86.cpp"

#else

#include "libbeye/sysdep/generic/cpu_info.cpp"

#endif
