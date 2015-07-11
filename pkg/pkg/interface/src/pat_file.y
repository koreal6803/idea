%{
// **************************************************************************
// File       [ pat_file.y ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ ]
// **************************************************************************

#define YYPARSE_PARAM param

#include <cstdio>
#include <cstring>

#include "pat_file.h"

using namespace std;
using namespace IntfNs;

extern char *pat_filetext;
extern int  pat_fileline;
extern int  pat_filelex(void);

void pat_fileerror(char const *msg);
void pat_filefreeNames(PatNameList *names);
void pat_filefreeCycles(PatCycleList *cycles);

%}

%union {
    char                 *ychar;
    IntfNs::PatCycle     *ycycle;
    IntfNs::PatNameList  *ynames;
    IntfNs::PatCycleList *ycycles;
}


%token CLOCK PATTERN PI_ORDER PO_ORDER SCAN_ORDER CAPT HOLD
%token <ychar> NAME

%type <ynames>  names
%type <ycycle>  cycle
%type <ycycles> cycles

%start start

%%

start
    : header patterns
    ;


header
    : pi_order po_order scan_order
    ;


pi_order
    : PI_ORDER names {
        if (!((PatFile *)param)->setPiOrder($2)) {
            pat_fileerror("setPiOrder() failed");
            YYABORT;
        }
        pat_filefreeNames($2);
    }
    | PI_ORDER
    ;


po_order
    : PO_ORDER names {
        if (!((PatFile *)param)->setPoOrder($2)) {
            pat_fileerror("setPoOrder() failed");
            YYABORT;
        }
        pat_filefreeNames($2);
    }
    | PO_ORDER
    ;


scan_order
    : SCAN_ORDER names {
        if (!((PatFile *)param)->setScanOrder($2)) {
            pat_fileerror("setScanOrder() failed");
            YYABORT;
        }
        pat_filefreeNames($2);
    }
    | SCAN_ORDER
    ;


patterns
    : patterns pattern
    |
    ;


pattern
    : PATTERN NAME NAME cycles NAME NAME {
        if (!((PatFile *)param)->addPattern(atoi($2), atoi($3), $4, $5, $6)) {
            pat_fileerror("addPattern() failed");
            YYABORT;
        }
        delete $2;
        delete $3;
        delete $5;
        delete $6;
        pat_filefreeCycles($4);
    }
    | PATTERN NAME NAME cycles {
        if (!((PatFile *)param)->addPattern(atoi($2), atoi($3), $4, NULL, NULL)) {
            pat_fileerror("addPattern() failed");
            YYABORT;
        }
        delete $2;
        delete $3;
        pat_filefreeCycles($4);
    }
    ;

cycles
    : cycles cycle { $$ = $1;
                     $$->insert($$->end(), $2); }
    | cycle        { $$ = new PatCycleList;
                     $$->insert($$->end(), $1); }
    ;

cycle
    : NAME NAME CAPT { $$ = new PatCycle;
                       $$->pi = strdup($1);
                       $$->po = strdup($2);
                       $$->type = PAT_CLK_CAPT; }
    | NAME NAME HOLD { $$ = new PatCycle;
                       $$->pi = strdup($1);
                       $$->po = strdup($2);
                       $$->type = PAT_CLK_HOLD; }
    ;

names
    : names NAME { $$ = $1;
                   $$->insert($1->end(), $2); }
    | NAME       { $$ = new PatNameList;
                   $$->insert($$->end(), $1); }
    ;



%%

void pat_fileerror(const char *msg) {
    fprintf(stderr, "**ERROR pattern_fileerror(): ");
    fprintf(stderr, "At line %d. ", pat_fileline);
    fprintf(stderr, "Near `%s'. ", pat_filetext);
    fprintf(stderr, "%s\n", msg);
}

void pat_filefreeNames(PatNameList *names) {
    PatNameList::iterator it = names->begin();
    for ( ; it != names->end(); ++it)
        delete [] (*it);
    delete names;
}

void pat_filefreeCycles(PatCycleList *cycles) {
    PatCycleList::iterator it = cycles->begin();
    for ( ; it != cycles->end(); ++it) {
        delete [] (*it)->pi;
        delete [] (*it)->po;
    }
    delete cycles;
}

