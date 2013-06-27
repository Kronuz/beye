#include "config.h"
#include "libbeye/libbeye.h"
using namespace	usr;
/**
 * @namespace   libbeye
 * @file        libbeye/libbeye.c
 * @brief       This file contains implementation of extension of C library.
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
 * @todo        Increase number of functions
**/
#include <sstream>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

void __FASTCALL__ memupr(any_t*ptr,unsigned n)
{
   unsigned i;
   for(i = 0;i < n;i++)
   ((char *)ptr)[i] = toupper(((char *)ptr)[i]);
}

void __FASTCALL__ memlwr(any_t*ptr,unsigned n)
{
   unsigned i;
   for(i = 0;i < n;i++)
   ((char *)ptr)[i] = tolower(((char *)ptr)[i]);
}

namespace	usr {
bool isseparate(int ch) { return (isspace(ch) || ispunct(ch)); }

int __FASTCALL__ szTrimTrailingSpace(char *str)
{
  unsigned len;
  int ret;
  len = strlen(str);
  ret = 0;
  while(len)
  {
      unsigned char ch;
      ch = str[len-1];
      if(isspace(ch) && ch < 0x80) { str[--len] = '\0'; ret++; }
      else break;
  }
  return ret;
}

std::string __FASTCALL__ szTrimTrailingSpace(const std::string& str)
{
    unsigned len = str.length();
    while(len) {
	unsigned char ch;
	ch = str[len-1];
	if(isspace(ch)) --len;
	else break;
    }
    return str.substr(0,len);
}

int __FASTCALL__ szTrimLeadingSpace(char *str)
{
  unsigned i,freq,len;
  len = strlen(str);
  for(i = freq = 0;i < len;i++)
  {
    unsigned char ch;
    ch = str[i];
    if(isspace(ch) && ch < 0x80) freq++;
    else                         break;
  }
  if(freq)
  {
    len -= freq;
    memmove(str,&str[freq],len+1);
  }
  return freq;
}

static const unsigned TEXT_TAB=8;

std::string __FASTCALL__ szSpace2Tab(const std::string& src)
{
  unsigned int i,len,limit,dest_idx;
  int j;
  unsigned char nspc;
  char buff[8];
  std::string dest;
  len = src.length();
  i = 0;
  dest_idx = 0;
  while(1)
  {
     if(i + TEXT_TAB < len)
     {
	memcpy(buff,&src[i],8);
	i+=8;
	/* scan */
	nspc = 0;
	for(j = TEXT_TAB-1;j >= 0;j--)
	{
	  if(buff[j] != ' ') break;
	  else nspc++;
	}
	limit = TEXT_TAB - nspc;
	dest.insert(dest_idx,buff,limit);
	dest_idx += limit;
	if(nspc) dest.insert(dest_idx++,1,'\t');
     }
     else
     {
       limit = len - i;
       dest.insert(dest_idx,&src[i],limit);
       dest_idx += limit;
       i += limit;
       break;
     }
  }
  return dest;
}

char * __FASTCALL__ szKillSpaceAround(char *str,char *place)
{
  char *sptr;
  unsigned nmoves,len,idx,freq;
  unsigned char prev;
  unsigned char ch;
  prev = *place;
  len = strlen(str);
  *place = 0;
  idx = place - str;
  nmoves = szTrimTrailingSpace(str);
  sptr = place;
  freq = 0;
  sptr++;
  while((ch = *sptr) != 0)
  {
    if(isspace(ch)) freq++;
    else            break;
    sptr++;
  }
  memmove(&str[idx-nmoves],&str[idx+freq],len-idx+1-freq);
  str[idx-nmoves] = prev;
  return &str[idx-nmoves];
}

		/** Exchanges two bytes in memory.
		  * @return         none
		  * @param _val1    specified pointer to the first byte to be exchanged
		  * @param _val2    specified pointer to the second byte to be exchanged
		  * @note           Main difference from ByteSwap function family -
				    it is work with different number, rather than
				    changing byte order within given number.
		 **/
inline void __XchgB__(uint8_t* _val1,uint8_t* _val2) {
#if defined(__i386__) || defined(__x86_64__)
    register char _tmp;
    __asm("xchgb	%b1,(%2)":
	"=q"(_tmp):
	"0"(*_val2),
	"r"(_val1));
    *_val2 = _tmp;
#else
    register uint8_t _charv;
    _charv = *((uint8_t *)_val2);
    *((uint8_t *)_val2) = *((uint8_t *)_val1);
    *((uint8_t *)_val1) = _charv;
#endif
}

any_t* rnd_fill(any_t* buffer,size_t size)
{
    unsigned i;
    char ch;
    for(i=0;i<size;i++) {
	ch=::rand()%255;
	((char *)buffer)[i]=ch;
    }
    return buffer;
}

any_t* make_false_pointer(any_t* tmplt) {
    long lo_mask=(sizeof(any_t*)*8/2)-1;
    long hi_mask=~lo_mask;
    long false_pointer;
    false_pointer=::rand()&lo_mask;
    false_pointer|=(reinterpret_cast<long>(tmplt)&hi_mask);
    return reinterpret_cast<any_t*>(false_pointer);
}

any_t*	__FASTCALL__ make_false_pointer_to(any_t* tmplt,unsigned size) {
    long false_pointer=reinterpret_cast<long>(tmplt);
    false_pointer+=::rand()%size;
    return reinterpret_cast<any_t*>(false_pointer);
}

any_t* fill_false_pointers(any_t* buffer,size_t size)
{
    unsigned i,psize=(size/sizeof(any_t*))*sizeof(any_t*);
    any_t* filler;
    for(i=0;i<psize/sizeof(long);i++) {
	filler=make_false_pointer(buffer);
	((long *)buffer)[i]=::rand()%2?reinterpret_cast<long>(filler):0;
    }
    ::memset(&((char *)buffer)[psize],0,size-psize);
    return buffer;
}

any_t* get_caller_address(unsigned num_caller) {
    any_t*	stack[3+num_caller];
    ::backtrace(stack,3+num_caller);
    return stack[2+num_caller];
}

Opaque::Opaque() {
    fill_false_pointers(&false_pointers,reinterpret_cast<long>(&unusable)-reinterpret_cast<long>(&false_pointers));
    fill_false_pointers(&unusable,sizeof(any_t*));
}

Opaque::~Opaque() {}

missing_device_exception::missing_device_exception() throw() {}
missing_device_exception::~missing_device_exception() throw() {}
const char* missing_device_exception::what() const throw() { return "missing device"; }

missing_driver_exception::missing_driver_exception() throw() {}
missing_driver_exception::~missing_driver_exception() throw() {}
const char* missing_driver_exception::what() const throw() { return "missing driver"; }
} // namespace	usr
