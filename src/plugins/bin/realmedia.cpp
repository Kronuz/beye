#include "config.h"
#include "libbeye/libbeye.h"
using namespace	usr;
/**
 * @namespace	usr_plugins_auto
 * @file        plugins/bin/realmdeia.c
 * @brief       This file contains implementation of decoder for jpeg
 *              file format.
 * @version     -
 * @remark      this source file is part of rmary vIEW project (BEYE).
 *              The rmary vIEW (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Nickols_K
 * @since       1995
 * @note        Development, fixes and improvements
**/
#include <stddef.h>

#include "bconsole.h"
#include "beyehelp.h"
#include "colorset.h"
#include "libbeye/kbd_code.h"
#include "plugins/disasm.h"
#include "plugins/bin/mmio.h"
#include "libbeye/bstream.h"
#include "plugins/binary_parser.h"
#include "beye.h"

namespace	usr {
#define MKTAG(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))
    class RM_Parser : public Binary_Parser {
	public:
	    RM_Parser(BeyeContext& b,binary_stream&,CodeGuider&,udn&);
	    virtual ~RM_Parser();

	    virtual const char*		prompt(unsigned idx) const;

	    virtual __filesize_t	show_header() const;
	    virtual int			query_platform() const;
	private:
	    BeyeContext&		bctx;
	    binary_stream&		main_handle;
	    udn&			_udn;
    };
static const char* txt[]={ "", "", "", "", "", "", "", "", "", "" };
const char* RM_Parser::prompt(unsigned idx) const { return txt[idx]; }

__filesize_t RM_Parser::show_header() const
{
    bctx.ErrMessageBox("Not implemented yet!","RM format");
    return bctx.tell();
}

RM_Parser::RM_Parser(BeyeContext& b,binary_stream& h,CodeGuider& code_guider,udn& u)
	    :Binary_Parser(b,h,code_guider,u)
	    ,bctx(b)
	    ,main_handle(h)
	    ,_udn(u)
{
    main_handle.seek(0,binary_stream::Seek_Set);
    if(main_handle.read(type_dword)!=MKTAG('.', 'R', 'M', 'F')) throw bad_format_exception();
}
RM_Parser::~RM_Parser() {}
int  RM_Parser::query_platform() const { return DISASM_DEFAULT; }

static Binary_Parser* query_interface(BeyeContext& b,binary_stream& h,CodeGuider& _parent,udn& u) { return new(zeromem) RM_Parser(b,h,_parent,u); }
extern const Binary_Parser_Info rm_info = {
    "Real Media file format",	/**< plugin name */
    query_interface
};
} // namespace	usr
