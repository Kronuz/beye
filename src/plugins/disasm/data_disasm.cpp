#include "config.h"
#include "libbeye/libbeye.h"
using namespace	usr;
/**
 * @namespace	usr_plugins_II
 * @file        plugins/disasm/null_da.c
 * @brief       This file contains implementation of Data disassembler.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Nickols_K
 * @since       1999
 * @note        Development, fixes and improvements
**/
#include <sstream>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "beye.h"
#include "plugins/disasm.h"
#include "bconsole.h"
#include "beyehelp.h"
#include "beyeutil.h"
#include "listbox.h"
#include "libbeye/file_ini.h"

namespace	usr {
    class Data_Disassembler : public Disassembler {
	public:
	    Data_Disassembler(BeyeContext& bc,const Bin_Format& b,binary_stream& h,DisMode& parent);
	    virtual ~Data_Disassembler();
	
	    virtual const char*	prompt(unsigned idx) const;
	    virtual bool	action_F3();

	    virtual DisasmRet	disassembler(__filesize_t shift,MBuffer insn_buff,unsigned flags);

	    virtual void	show_short_help() const;
	    virtual int		max_insn_len() const;
	    virtual ColorAttr	get_insn_color(unsigned long clone);

	    virtual Bin_Format::bitness	get_bitness() const;
	    virtual char	clone_short_name(unsigned long clone);
	    virtual void	read_ini(Ini_Profile&);
	    virtual void	save_ini(Ini_Profile&);
	private:
	    BeyeContext&	bctx;
	    DisMode&		parent;
	    binary_stream&	main_handle;
	    const Bin_Format&	bin_format;
	    int			nulWidth;
	    char*		outstr;

	    static const char*	width_names[];
    };

const char* Data_Disassembler::width_names[] =
{
   "~Byte",
   "~Word",
   "~Double word",
   "~Quad word"
};

bool Data_Disassembler::action_F3()
{
    unsigned nModes;
    int i;
    nModes = sizeof(width_names)/sizeof(char *);
    ListBox lb(bctx);
    i = lb.run(width_names,nModes," Select bitness mode: ",ListBox::Selective|ListBox::UseAcc,nulWidth);
    if(i != -1) {
	nulWidth = i;
	return true;
    }
    return false;
}

DisasmRet Data_Disassembler::disassembler(__filesize_t ulShift,
					MBuffer buffer,
					unsigned flags)
{
  DisasmRet ret;
  int cl;
  DisMode::e_disarg type;
  const char *preface;
  if(!((flags & __DISF_SIZEONLY) == __DISF_SIZEONLY))
  {
    switch(nulWidth)
    {
      case 0: preface = "db ";
	      type = DisMode::Arg_Byte;
	      cl = 1;
	      break;
      default:
      case 1: preface = "dw ";
	      type = DisMode::Arg_Word;
	      cl = 2;
	      break;
      case 2: preface = "dd ";
	      type = DisMode::Arg_DWord;
	      cl = 4;
	      break;
      case 3: preface = "dq ";
	      type = DisMode::Arg_QWord;
	      cl = 8;
	      break;
    }
    ret.codelen = cl;
    strcpy(outstr,preface);
    std::string stmp = outstr;
    parent.append_digits(main_handle,stmp,ulShift,Bin_Format::Use_Type,cl,buffer,type);
    strcpy(outstr,stmp.c_str());
    ret.str = outstr;
  }
  else
    if(flags & __DISF_GETTYPE) ret.pro_clone = __INSNT_ORDINAL;
    else
    switch(nulWidth)
    {
      case 0: ret.codelen = 1; break;
      default:
      case 1: ret.codelen = 2; break;
      case 2: ret.codelen = 4; break;
      case 3: ret.codelen = 8; break;
    }
  return ret;
}

void Data_Disassembler::show_short_help() const
{
    Beye_Help bhelp(bctx);
    if(bhelp.open(true)) {
	bhelp.run(20010);
	bhelp.close();
    }
}

int Data_Disassembler::max_insn_len() const { return 8; }
ColorAttr Data_Disassembler::get_insn_color( unsigned long clone )
{
  UNUSED(clone);
  return disasm_cset.cpu_cset[0].clone[0];
}
Bin_Format::bitness Data_Disassembler::get_bitness() const { return Bin_Format::Use16; }
char Data_Disassembler::clone_short_name( unsigned long clone )
{
  UNUSED(clone);
  return ' ';
}
Data_Disassembler::Data_Disassembler(BeyeContext& bc,const Bin_Format& b,binary_stream& h,DisMode& _parent )
		:Disassembler(bc,b,h,_parent)
		,bctx(bc)
		,parent(_parent)
		,main_handle(h)
		,bin_format(b)
		,nulWidth(1)
{
    outstr = new char [1000];
}

Data_Disassembler::~Data_Disassembler()
{
    delete outstr;
}

void Data_Disassembler::read_ini( Ini_Profile& ini )
{
  std::string tmps;
  if(bctx.is_valid_ini_args()) {
    tmps=bctx.read_profile_string(ini,"Beye","Browser","SubSubMode3","1");
    std::istringstream is(tmps);
    is>>nulWidth;
    if(nulWidth > 3) nulWidth = 0;
  }
}

void Data_Disassembler::save_ini( Ini_Profile& ini )
{
  std::ostringstream os;
  os<<nulWidth;
  bctx.write_profile_string(ini,"Beye","Browser","SubSubMode3",os.str());
}

const char* Data_Disassembler::prompt(unsigned idx) const {
    switch(idx) {
	case 1: return "Width"; break;
	default: break;
    }
    return "";
}

static Disassembler* query_interface(BeyeContext& bc,const Bin_Format& b,binary_stream& h,DisMode& _parent) { return new(zeromem) Data_Disassembler(bc,b,h,_parent); }
extern const Disassembler_Info data_disassembler_info = {
    DISASM_DATA,
    "~Data",	/**< plugin name */
    query_interface
};
} // namespace	usr
