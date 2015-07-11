%{
// **************************************************************************
// File       [ vlog_file.y ]
// Author     [ littleshamoo ]
// Synopsis   [ yacc for parsing verilog file ]
// Date       [ 2010/07/07 created ]
// **************************************************************************

#define YYPARSE_PARAM param

#include <cstdio>
#include <cstring>

#include "vlog_file.h"

using namespace std;
using namespace IntfNs;

extern char *vlog_filetext;
extern int  vlog_fileline;
extern int  vlog_filelex(void);

void vlog_fileerror(char const *msg);
void vlog_filefreeNames(VlogNameList *names);
void vlog_filefreePairs(VlogPairList *pairs);

%}

%union {
    char                 *ychar;
    IntfNs::VlogNetType  ynetType;
    IntfNs::VlogNameList *ynames;
    IntfNs::VlogPairList *ypairs;
}

%token MODULE ENDMODULE PRIMITIVE ENDPRIMITIVE ASSIGN INPUT OUTPUT INOUT
%token WIRE REG SUPPLY_L SUPPLY_H
%token <ychar> NAME NUMBER STRENGTH

%type <ynetType> net_type
%type <ynames>   list strength_list
%type <ypairs>   mapping_list

%start start


%%

start
    : modules
    ;



modules
    : modules module
    | module
    ;


module
    : MODULE add_module ports body ENDMODULE
    | PRIMITIVE add_module ports body ENDPRIMITIVE
    ;


add_module
    : NAME {
        if (!((VlogFile *)param)->addModule($1)) {
            vlog_fileerror("addModule() failed");
            YYABORT;
        }
        delete [] $1;
    }
    ;


ports
    : '(' list ')' ';' {
        if (!((VlogFile *)param)->addPorts($2)) {
            vlog_fileerror("addPorts() failed");
            YYABORT;
        }
        vlog_filefreeNames($2);
    }
    | ';'
    ;


body
    : body net
    | body assign
    | body inst
    |
    ;


net
    : net_type list ';' {
        switch ($1) {
            case VLOG_NET_INPUT:
                if (!((VlogFile *)param)->setInputNets($2)) {
                    vlog_fileerror("setInputNets() failed");
                    YYABORT;
                }
                break;
            case VLOG_NET_OUTPUT:
                if (!((VlogFile *)param)->setOutputNets($2)) {
                    vlog_fileerror("setOutputNets() failed");
                    YYABORT;
                }
                break;
            case VLOG_NET_INOUT:
                if (!((VlogFile *)param)->setInoutNets($2)) {
                    vlog_fileerror("setInoutNets() failed");
                    YYABORT;
                }
                break;
            case VLOG_NET_WIRE:
                if (!((VlogFile *)param)->setWireNets($2)) {
                    vlog_fileerror("setWireNets() failed");
                    YYABORT;
                }
                break;
            case VLOG_NET_REG:
                if (!((VlogFile *)param)->setRegNets($2)) {
                    vlog_fileerror("setRegNets() failed");
                    YYABORT;
                }
                break;
            case VLOG_NET_SUPPLY_L:
                if (!((VlogFile *)param)->setSupplyLNets($2)) {
                    vlog_fileerror("setSupplyLNets() failed");
                    YYABORT;
                }
                break;
            case VLOG_NET_SUPPLY_H:
                if (!((VlogFile *)param)->setSupplyHNets($2)) {
                    vlog_fileerror("setSupplyHNets() failed");
                    YYABORT;
                }
                break;
        }
        vlog_filefreeNames($2);
    }
    ;


net_type
    : INPUT    { $$ = VLOG_NET_INPUT;    }
    | OUTPUT   { $$ = VLOG_NET_OUTPUT;   }
    | INOUT    { $$ = VLOG_NET_INOUT;    }
    | WIRE     { $$ = VLOG_NET_WIRE;     }
    | REG      { $$ = VLOG_NET_REG;      }
    | SUPPLY_L { $$ = VLOG_NET_SUPPLY_L; }
    | SUPPLY_H { $$ = VLOG_NET_SUPPLY_H; }
    ;


assign
    : ASSIGN NAME '=' NAME ';' {
        if (!((VlogFile *)param)->addAssign($2, $4))
            YYABORT;
        delete [] $2;
        delete [] $4;
    }
    ;


inst
    : NAME NAME '(' list ')' ';' {
        if (!((VlogFile *)param)->addInst($1, $2, $4))
            YYABORT;
        delete [] $1;
        delete [] $2;
        vlog_filefreeNames($4);
    }
    | NAME '(' list ')' ';' {
        if (!((VlogFile *)param)->addInst($1, "", $3))
            YYABORT;
        delete [] $1;
        vlog_filefreeNames($3);
    }
    | NAME NAME '(' mapping_list ')' ';' {
        if (!((VlogFile *)param)->addInst($1, $2, $4))
            YYABORT;
        delete [] $1;
        delete [] $2;
        vlog_filefreePairs($4);
    }
    | NAME '(' strength_list ')' NAME '(' list ')' ';' {
        if (!((VlogFile *)param)->addInst($1, $3, $5, $7))
            YYABORT;
        delete [] $1;
        delete [] $5;
        vlog_filefreeNames($3);
        vlog_filefreeNames($7);
    }
    ;


list
    : list ',' NAME { $$ = $1;
                      $1->insert($1->end(), $3);         }
    | list ','      { $$ = $1;
                      $1->insert($1->end(), strdup("")); }
    | NAME          { $$ = new VlogNameList;
                      $$->insert($$->end(), $1);         }
    |               { $$ = new VlogNameList;
                      $$->insert($$->end(), strdup("")); }
    ;


strength_list
    : strength_list ',' STRENGTH { $$ = $1;
                                   $1->insert($1->end(), $3); }
    | STRENGTH                   { $$ = new VlogNameList;
                                   $$->insert($$->end(), $1); }
    ;


mapping_list
    : mapping_list ',' '.' NAME '(' NAME ')' {
        $$ = $1;
        $1->insert($1->end(), make_pair($4, $6));
    }
    | mapping_list ',' '.' NAME '(' ')' {
        $$ = $1;
        $1->insert($1->end(), make_pair($4, strdup("")));
    }
    | '.' NAME '(' NAME ')' {
        $$ = new VlogPairList;
        $$->insert($$->end(), make_pair($2, $4));
    }
    | '.' NAME '(' ')' {
        $$ = new VlogPairList;
        $$->insert($$->end(), make_pair($2, strdup("")));
    }
    ;


%%

void vlog_fileerror(const char *msg) {
    fprintf(stderr, "**ERROR vlog_fileerror(): ");
    fprintf(stderr, "At line %d. Near `%s'. ", vlog_fileline, vlog_filetext);
    fprintf(stderr, "%s\n", msg);
}

void vlog_filefreeNames(VlogNameList *names) {
    VlogNameList::iterator it = names->begin();
    for ( ; it != names->end(); ++it)
        delete [] (*it);
    delete names;
}

void vlog_filefreePairs(VlogPairList *pairs) {
    VlogPairList::iterator it = pairs->begin();
    for ( ; it != pairs->end(); ++it) {
        delete [] (*it).first;
        delete [] (*it).second;
    }
    delete pairs;
}

