/*!
	@header			DbpInclude.h
	@abstract 		DBP Include files.
	@discussion 	DBP Includes
					
*/

#if !defined(dbpinclude_h)
#define dbpinclude_h

// QXP2017
// QXP2017
// QXP2017

#include "Main.h"

// LEGACY
// LEGACY
// LEGACY

extern "C" {
#include "XP_XTLegacy.h" 
#include "XP_XTLegacyE7.h" 
#include "XP_XTLegacyE6.h" 
}

// always the first: they must be present in the project files
#include "XTConfig.h"
#include "QXPConfig.h"

#endif // !defined(dbpinclude_h)
