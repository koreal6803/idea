%{
// **************************************************************************
// File       [ pat_file.l ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ ]
// **************************************************************************


#include <cstdlib>

#include "pat_file.h"
#include "pat_file.y.hpp"

using namespace std;

int pat_fileline = 1;


%}

D      [0-9]
NUMBER {D}{D}*
WHT    [ \t\v\f]
L      [a-zA-Z_]
LTRAIL [0-9a-zA-Z_./]
ESC    [~`!#$%^&*()+=|{}:;"',<>?/.\-\[\]]
NAME   ({LTRAIL}|("\\"{ESC}))*("["{NUMBER}(":"{NUMBER})?"]")?

%option nounput noyywrap

%%

"#".*            ;

"__CLOCK__"      { return CLOCK;      }
"__PATTERN__"    { return PATTERN;    }
"__PI_ORDER__"   { return PI_ORDER;   }
"__PO_ORDER__"   { return PO_ORDER;   }
"__SCAN_ORDER__" { return SCAN_ORDER; }
"__CAPT__"       { return CAPT;       }
"__HOLD__"       { return HOLD;       }

{NAME}           {
    pat_filelval.ychar = strdup(pat_filetext);
    return NAME;
}

\n               { pat_fileline++; }
[ \t\v\f]        ;
.                ;

%%

