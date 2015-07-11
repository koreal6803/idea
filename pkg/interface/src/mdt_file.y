%{
// **************************************************************************
// File       [ mdt_file.y ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/02/23 created ]
// **************************************************************************

#define YYPARSE_PARAM param

#include <cstdio>
#include <cstring>

#include "mdt_file.h"

using namespace std;
using namespace IntfNs;

extern char *mdt_filetext;
extern int  mdt_fileline;
extern int  mdt_filelex(void);

void mdt_fileerror(char const *msg);
void mdt_filefreeNames(MdtNameList * const names);
void mdt_filefreePairs(MdtPairList * const pairs);

%}

%union {
    char                *ychar;
    IntfNs::MdtPortType yportType;
    IntfNs::MdtNameList *ynames;
    IntfNs::MdtPairList *ypairs;
}

%token MODEL MODEL_SOURCE INPUT OUTPUT INOUT INTERN PRIMITIVE INSTANCE
%token <ychar> NAME

%type <yportType> body_type
%type <ynames> list
%type <ypairs> mapping_list

%start start

%%

start
    : models
    ;


models
    : models model
    | model
    ;


model
    : MODEL_SOURCE '=' NAME { delete [] $3; }
    | MODEL add_model '(' model_port ')' '(' model_bodies ')'
    ;


add_model
    : NAME {
        if (!((MdtFile *)param)->addModel($1)) {
            mdt_fileerror("addModule() failed");
            YYABORT;
        }
        delete [] $1;
    }
    ;


model_port
    : list {
        if (!((MdtFile *)param)->addPorts($1)) {
            mdt_fileerror("addPorts() failed");
            YYABORT;
        }
        mdt_filefreeNames($1);
    }
    ;


model_bodies
    : model_bodies model_body
    | model_body
    ;


model_body
    : body_net '(' attributes ')'
    | body_net '(' ')'
    ;


body_net
    : body_type '(' list ')' {
        switch ($1) {
            case MDT_PORT_INPUT:
                if (!((MdtFile *)param)->setInputNets($3)) {
                    mdt_fileerror("setInputNets() failed");
                    YYABORT;
                }
                break;
            case MDT_PORT_OUTPUT:
                if (!((MdtFile *)param)->setOutputNets($3)) {
                    mdt_fileerror("setOutputNets() failed");
                    YYABORT;
                }
                break;
            case MDT_PORT_INOUT:
                if (!((MdtFile *)param)->setInoutNets($3)) {
                    mdt_fileerror("setInoutNets() failed");
                    YYABORT;
                }
                break;
            case MDT_PORT_INTERN:
                if (!((MdtFile *)param)->setInternNets($3)) {
                    mdt_fileerror("setInternNets() failed");
                    YYABORT;
                }
                break;
        }
        mdt_filefreeNames($3);
    }
    ;


body_type
    : INPUT  { $$ = MDT_PORT_INPUT;  }
    | OUTPUT { $$ = MDT_PORT_OUTPUT; }
    | INOUT  { $$ = MDT_PORT_INOUT;  }
    | INTERN { $$ = MDT_PORT_INTERN; }
    ;


attributes
    : attributes attribute
    | attribute
    ;


attribute
    : primitive
    | instance
    ;


primitive
    : PRIMITIVE '=' NAME NAME '(' list ')' ';' {
        if (!((MdtFile *)param)->addPrimitive($3, $4, $6)) {
            mdt_fileerror("addPrimitive() failed");
            YYABORT;
        }
        delete [] $3;
        delete [] $4;
        mdt_filefreeNames($6);
    }
    | PRIMITIVE '=' NAME '(' list ')' ';' {
        if (!((MdtFile *)param)->addPrimitive($3, "", $5)) {
            mdt_fileerror("addPrimitive() failed");
            YYABORT;
        }
        delete [] $3;
        mdt_filefreeNames($5);
    }
    ;


instance
    : INSTANCE '=' NAME NAME '(' list ')' ';' {
        if (!((MdtFile *)param)->addInstance($3, $4, $6)) {
            mdt_fileerror("addInstance() failed");
            YYABORT;
        }
        delete [] $3;
        delete [] $4;
        mdt_filefreeNames($6);
    }
    | INSTANCE '=' NAME '(' list ')' ';' {
        if (!((MdtFile *)param)->addInstance($3, "", $5)) {
            mdt_fileerror("addInstance() failed");
            YYABORT;
        }
        delete [] $3;
        mdt_filefreeNames($5);
    }
    | INSTANCE '=' NAME NAME '(' mapping_list ')' ';' {
        if (!((MdtFile *)param)->addInstance($3, $4, $6)) {
            mdt_fileerror("addInstance() failed");
            YYABORT;
        }
        delete [] $3;
        delete [] $4;
        mdt_filefreePairs($6);
    }
    | INSTANCE '=' NAME '(' mapping_list ')' ';' {
        if (!((MdtFile *)param)->addInstance($3, "", $5)) {
            mdt_fileerror("addInstance() failed");
            YYABORT;
        }
        delete [] $3;
        mdt_filefreePairs($5);
    }
    ;


list
    : list ',' NAME { $$ = $1;
                      $1->insert($1->end(), $3);         }
    | list ','      { $$ = $1;
                      $1->insert($1->end(), strdup("")); }
    | NAME          { $$ = new MdtNameList;
                      $$->insert($$->end(), $1);         }
    |               { $$ = new MdtNameList;
                      $$->insert($$->end(), strdup("")); }
    ;


mapping_list
    : mapping_list ',' '.' NAME '(' NAME ')' {
        $$ = $1;
        $1->insert($1->end(), make_pair($4, $6));
    }
    | '.' NAME '(' NAME ')' {
        $$ = new MdtPairList;
        $$->insert($$->end(), make_pair($2, $4));
    }
    ;


%%

void mdt_fileerror(const char *msg) {
    fprintf(stderr, "**ERROR mdt_fileerror(): ");
    fprintf(stderr, "At line %d. Near `%s'. ", mdt_fileline, mdt_filetext);
    fprintf(stderr, "%s\n", msg);
}

void mdt_filefreeNames(MdtNameList *names) {
    MdtNameList::iterator it = names->begin();
    for ( ; it != names->end(); ++it)
        delete [] (*it);
    delete names;
}

void mdt_filefreePairs(MdtPairList *pairs) {
    MdtPairList::iterator it = pairs->begin();
    for ( ; it != pairs->end(); ++it) {
        delete [] (*it).first;
        delete [] (*it).second;
    }
    delete pairs;
}

