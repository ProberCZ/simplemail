/* This file includes the amiga specific expat files so
   expat using is transparent */

#if defined(__AROS__) || defined(__AMIGAOS4__)
#include <expat.h>
#else
#include <exec/types.h>
#include <expat/expat.h>
#include <proto/expat.h>

/********************
 expat.library hack
 see startup-morphos.c
*********************/
#if defined(__MORPHOS__)
#include <emul/emulregs.h>
extern struct EmulLibEntry xml_start_tag_trap;
extern struct EmulLibEntry xml_end_tag_trap;
extern struct EmulLibEntry xml_char_data_trap;
#undef XML_SetCharacterDataHandler
#undef XML_SetElementHandler
#define XML_SetCharacterDataHandler(__p0, __p1) \
	LP2NR(96, XML_SetCharacterDataHandler, \
		XML_Parser , __p0, a0, \
		XML_CharacterDataHandler , (void *)&__p1##_trap , a1, \
		, EXPAT_BASE_NAME, 0, 0, 0, 0, 0, 0)
#define XML_SetElementHandler(__p0, __p1, __p2) \
	LP3NR(90, XML_SetElementHandler, \
		XML_Parser , __p0, a0, \
		XML_StartElementHandler , (void *)&__p1##_trap, a1, \
		XML_EndElementHandler , (void *)&__p2##_trap, a2, \
		, EXPAT_BASE_NAME, 0, 0, 0, 0, 0, 0)
#endif

#endif

