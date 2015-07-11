%{
// **************************************************************************
// File       [ sdf_file.y ]
// Author     [ littleshamoo ]
// Synopsis   [ Currently only supports SDF file header and absolute and
//              increment delay information ]
// Date       [ 2010/11/18 created ]
// **************************************************************************

#define YYPARSE_PARAM param

#include <cstring>
#include <cstdio>

#include "sdf_file.h"

using namespace std;
using namespace IntfNs;

extern char *sdf_filetext;
extern int  sdf_fileline;
extern int  sdf_filelex(void);
       void sdf_fileerror(char const *msg);

void sdf_filefreeSdfDelayDef(SdfDelayDef *def);

%}

%union {
    float                     yfloat;
    char                      *ychar;
    IntfNs::SdfDelayType      ydelayType;
    IntfNs::SdfValue          yvalue;
    IntfNs::SdfDelayValue     ydelayValue;
    IntfNs::SdfDelayValueList ydelayValueList;
    IntfNs::SdfDelayDef       *ydelayDef;
    IntfNs::SdfIoPath         yioPath;
    IntfNs::SdfPortSpec       yportSpec;
}


%token DELAYFILE

%token SDFVERSION DESIGN DATE VENDOR PROGRAM VERSION DIVIDER VOLTAGE PROCESS
%token TEMPERATURE TIMESCALE

%token CELL CELLTYPE INSTANCE

%token DELAY TIMINGCHECK TIMINGENV PATHPULSE PATHPULSEPERCENT ABSOLUTE
%token INCREMENT IOPATH RETAIN COND SCOND CCOND CONDELSE PORT INTERCONNECT
%token DEVICE SETUP HOLD SETUPHOLD RECOVERY REMOVAL RECREM SKEW WIDTH PERIOD
%token NOCHANGE NAME EXCEPTION PATHCONSTRAINT PERIODCONSTRAINT SUM DIFF
%token SKEWCONSTRAINT ARRIVAL DEPARTURE SLACK WAVEFORM POSEDGE NEGEDGE

%token CASE_EQU CASE_INEQU EQU INEQU L_AND L_OR
%token LESS_OR_EQU GREATER_OR_EQU RIGHT_SHIFT LEFT_SHIFT
%token U_NAND U_NOR U_XNOR U_XNOR_ALT

%token <ychar>  BIT_CONST ONE_CONST ZERO_CONST EDGE
%token <ychar>  PATH IDENTIFIER
%token <ychar>  QSTRING
%token <yfloat> NUMBER

%type <ydelayType>      valuetype
%type <ychar>           hchar
%type <ychar>           cell_type cell_instance
%type <ychar>           port_instance port scalar_port bus_port
%type <yvalue>          value triple
%type <ydelayValue>     delval
%type <ydelayValueList> delval_list
%type <ydelayDef>       del_defs del_def_iopath del_def
%type <yioPath>         input_output_path
%type <yportSpec>       port_spec port_edge edge_identifier

%left L_OR
%left L_AND
%left '|'
%left '^' U_XNOR U_XNOR_ALT
%left '&'
%left EQU INEQU CASE_EQU CASE_INEQU
%left '<' LESS_OR_EQU '>' GREATER_OR_EQU
%left RIGHT_SHIFT LEFT_SHIFT
%left '+' '-'
%left '*' '/' '%'
%left '!' '~'

%start start


%%

start
    : delay_file;


delay_file
    : '(' DELAYFILE sdf_header cells ')'
    ;


sdf_header
    : sdf_header design_name
    | sdf_header date
    | sdf_header vendor
    | sdf_header program_name
    | sdf_header program_version
    | sdf_header hierarchy_divider
    | sdf_header voltage
    | sdf_header process
    | sdf_header temperature
    | sdf_header time_scale
    | sdf_version
    ;


sdf_version
    : '(' SDFVERSION QSTRING ')' { ((SdfFile *)param)->addVersion($3);
                                    free($3);                       }
    ;


design_name
    : '(' DESIGN QSTRING ')' { ((SdfFile *)param)->addDesign($3);
                               free($3);                      }
    ;


date
    : '(' DATE QSTRING ')' { ((SdfFile *)param)->addDate($3);
                             free($3);                    }
    ;


vendor
    : '(' VENDOR QSTRING ')' { ((SdfFile *)param)->addVendor($3);
                               free($3);                      }
    ;


program_name
    : '(' PROGRAM QSTRING ')' { ((SdfFile *)param)->addProgName($3);
                                free($3);                        }
    ;


program_version
    : '(' VERSION QSTRING ')' { ((SdfFile *)param)->addProgVersion($3);
                                free($3);                           }
    ;


hierarchy_divider
    : '(' DIVIDER hchar ')' { ((SdfFile *)param)->addHierChar($3[0]);
                              free($3);                           }
    ;


hchar
    : '/' { $$ = strdup("/"); }
    | '.' { $$ = strdup("."); }
    ;


voltage
    : '(' VOLTAGE triple ')' { ((SdfFile *)param)->addVoltage($3); }
    | '(' VOLTAGE NUMBER ')' { SdfValue v;
                               v.n = 1;
                               v.v[0] = $3;
                               ((SdfFile *)param)->addVoltage(v);  }
    ;


process
    : '(' PROCESS QSTRING ')' { ((SdfFile *)param)->addProcess($3);
                                free( $3);                       }
    ;

temperature
    : '(' TEMPERATURE triple ')' { ((SdfFile *)param)->addTemperature($3); }
    | '(' TEMPERATURE NUMBER ')' { SdfValue v;
                                   v.n = 1;
                                   v.v[0] = $3;
                                   ((SdfFile *)param)->addTemperature(v);  }
    ;


time_scale
    : '(' TIMESCALE tsvalue ')'
    ;


tsvalue
    : NUMBER IDENTIFIER { ((SdfFile *)param)->addTimeScale($1, $2);
                          free($2);                             }
    ;


cells
    : cells cell
    | cell
    ;


cell
    : '(' CELL add_cell ')'
    | '(' CELL add_cell timing_specs ')'
    ;


add_cell
    : cell_type cell_instance { ((SdfFile *)param)->addCell($1, $2); 
                                free($1);
                                free($2);                        }
    ;


cell_type
    : '(' CELLTYPE QSTRING ')' { $$ = $3; }
    ;


cell_instance
    : '(' INSTANCE ')'            { $$ = strdup("");  }
    | '(' INSTANCE PATH ')'       { $$ = $3;          }
    | '(' INSTANCE IDENTIFIER ')' { $$ = $3;          }
    | '(' INSTANCE '*' ')'        { $$ = strdup("*"); }
    ;


timing_specs
    : timing_specs timing_spec
    | timing_spec
    ;


timing_spec
    : del_spec
    | tc_spec
    | te_spec
    ;


del_spec
    : '(' DELAY deltypes ')'
    ;


deltypes
    : deltypes deltype
    | deltype
    ;


tc_spec
    : '(' TIMINGCHECK tchk_defs ')'
    ;


tchk_defs
    : tchk_defs tchk_def
    | tchk_def
    ;


te_spec
    : '(' TIMINGENV te_defs ')'
    ;


te_defs
    : te_defs te_def
    | te_def
    ;


deltype
    : '(' pathtype del_path ')'
    | '(' valuetype del_defs ')' {
        SdfDelayDef *cur = $3->head;
        while (cur) {
            switch (cur->type) {
                case SdfDelayDef::IO_DELAY:
                    ((SdfFile *)param)->addIoDelay($2, cur->pspec, cur->p1,
                                                   cur->vlist);
                    break;
                case SdfDelayDef::IO_RETAIN:
                    ((SdfFile *)param)->addIoRetain($2, cur->pspec, cur->p1,
                                                    cur->vlist);
                    break;
                case SdfDelayDef::PORT_DELAY:
                    ((SdfFile *)param)->addPortDelay($2, cur->p1, cur->vlist);
                    break;
                case SdfDelayDef::INTER_DELAY:
                    ((SdfFile *)param)->addInterconnectDelay($2, cur->p1,
                                                             cur->p2,
                                                             cur->vlist);
                    break;
                case SdfDelayDef::DEVICE_DELAY:
                    ((SdfFile *)param)->addDeviceDelay($2, cur->p1,
                                                       cur->vlist);
                    break;
            }
            cur = cur->next;
        }
        sdf_filefreeSdfDelayDef($3->head);
    }
    ;


pathtype
    : PATHPULSE
    | PATHPULSEPERCENT
    ;


valuetype
    : ABSOLUTE  { $$ = DELAY_ABSOLUTE;  }
    | INCREMENT { $$ = DELAY_INCREMENT; }
    ;


del_path
    : value
    | input_output_path value
    | value value
    | input_output_path value value
    ;


del_defs
    : del_defs del_def {
        $$ = $2;
        $1->next = $$;
        $$->head = $1->head;
        $$->next = NULL;
    }
    | del_def {
        $$ = $1;
        $$->head = $$;
        $$->next = NULL;
    }
    ;


input_output_path
    : port_instance port_instance { $$.in = $1;
                                    $$.out = $2; }
    ;


del_def
    : del_def_iopath {
        $$ = $1;
    }
    | '(' del_cond del_def_iopath ')' {
        $$ = $3;
    }
    | '(' cond_else_type del_def_iopath ')' {
        $$ = $3;
    }
    | '(' PORT port_instance delval_list ')' {
        $$ = new SdfDelayDef;
        $$->type = SdfDelayDef::PORT_DELAY;
        $$->p1 = $3;
        $$->p2 = NULL;
        $$->vlist = $4;
    }
    | '(' INTERCONNECT port_instance port_instance delval_list ')' {
        $$ = new SdfDelayDef;
        $$->type = SdfDelayDef::INTER_DELAY;
        $$->p1 = $3;
        $$->p2 = $4;
        $$->vlist = $5;
    }
    | '(' DEVICE delval_list ')' {
        $$ = new SdfDelayDef;
        $$->type = SdfDelayDef::DEVICE_DELAY;
        $$->p1 = strdup("");
        $$->p2 = NULL;
        $$->vlist = $3;
    }
    | '(' DEVICE port_instance delval_list ')' {
        $$ = new SdfDelayDef;
        $$->type = SdfDelayDef::DEVICE_DELAY;
        $$->p1 = $3;
        $$->p2 = NULL;
        $$->vlist = $4;
    }
    ;


del_cond
    : COND expression
    | COND QSTRING expression
    ;


del_def_iopath
    : '(' IOPATH port_spec port_instance delval_list ')' {
        $$ = new SdfDelayDef;
        $$->type = SdfDelayDef::IO_DELAY;
        $$->pspec.type = $3.type;
        $$->pspec.port = $3.port;
        $$->p1 = $4;
        $$->p2 = NULL;
        $$->vlist = $5;
    }
    | '(' IOPATH port_spec port_instance '(' RETAIN delval_list ')' ')' {
        $$ = new SdfDelayDef;
        $$->type = SdfDelayDef::IO_RETAIN;
        $$->pspec.type = $3.type;
        $$->pspec.port = $3.port;
        $$->p1 = $4;
        $$->p2 = NULL;
        $$->vlist = $7;
    }
    ;


cond_else_type
    : CONDELSE
    ;


tchk_def
    : '(' SETUP port_tchk port_tchk value ')'
    | '(' HOLD port_tchk port_tchk value ')'
    | '(' SETUPHOLD port_tchk port_tchk value value ')'
    | '(' SETUPHOLD port_tchk port_tchk value value scond ')'
    | '(' SETUPHOLD port_tchk port_tchk value value ccond ')'
    | '(' SETUPHOLD port_tchk port_tchk value value scond ccond ')'
    | '(' RECOVERY port_tchk port_tchk value ')'
    | '(' REMOVAL port_tchk port_tchk value ')'
    | '(' RECREM port_tchk port_tchk value value ')'
    | '(' RECREM port_tchk port_tchk value value scond ')'
    | '(' RECREM port_tchk port_tchk value value ccond ')'
    | '(' RECREM port_tchk port_tchk value value scond ccond ')'
    | '(' SKEW port_tchk port_tchk value ')'
    | '(' WIDTH port_tchk value ')'
    | '(' PERIOD port_tchk value ')'
    | '(' NOCHANGE port_tchk port_tchk value value ')'
    ;


port_tchk
    : port_spec
    | '(' COND timing_check_condition port_spec ')'
    | '(' COND QSTRING timing_check_condition port_spec ')'
    ;


scond
    : '(' SCOND timing_check_condition ')'
    | '(' SCOND QSTRING timing_check_condition ')'
    ;


ccond
    : '(' CCOND timing_check_condition ')'
    | '(' CCOND QSTRING timing_check_condition ')'
    ;

name
    : '(' NAME ')'
    | '(' NAME QSTRING ')' { free( $3); }
    ;

exception
    : '(' EXCEPTION cell_instances ')'
    ;


cell_instances
    : cell_instances cell_instance
    | cell_instance
    ;


te_def
    : cns_def
    | tenv_def
    ;


cns_def
    : '(' PATHCONSTRAINT port_instance port_instances value value ')'
    | '(' PATHCONSTRAINT name port_instance port_instances value value ')'
    | '(' PERIODCONSTRAINT port_instance value ')'
    | '(' PERIODCONSTRAINT port_instance value exception ')'
    | '(' SUM constraint_path constraint_path value ')'
    | '(' SUM constraint_path constraint_path value value ')'
    | '(' SUM constraint_path constraint_path constraint_paths value ')'
    | '(' SUM constraint_path constraint_path constraint_paths value value ')'
    | '(' DIFF constraint_path constraint_path value ')'
    | '(' DIFF constraint_path constraint_path value value ')'
    | '(' SKEWCONSTRAINT port_spec value ')'
    ;


port_instances
    : port_instances port_instance
    | port_instance
    ;


constraint_paths
    : constraint_paths constraint_path
    | constraint_path
    ;


tenv_def
    : '(' ARRIVAL port_instance value value value value ')'
    | '(' ARRIVAL port_edge port_instance value value value value ')'
    | '(' DEPARTURE port_instance value value value value ')'
    | '(' DEPARTURE port_edge port_instance value value value value ')'
    | '(' SLACK port_instance value value value value ')'
    | '(' SLACK port_instance value value value value NUMBER ')'
    | '(' WAVEFORM port_instance NUMBER edge_list ')'
    ;


number_q
    : NUMBER
    |
    ;


constraint_path
    : '(' port_instance port_instance ')'
    ;


port_spec
    : port_instance { $$.type = SdfPortSpec::EDGE_NA;
                      $$.port = $1;                   }
    | port_edge     { $$.type = $1.type;
                      $$.port = $1.port;              }
    ;


port_edge
    : '(' edge_identifier port_instance ')' { $$.type = $2.type;
                                              $$.port = $3;      }
    ;


edge_identifier
    : POSEDGE    { $$.type = SdfPortSpec::EDGE_01;     }
    | NEGEDGE    { $$.type = SdfPortSpec::EDGE_10;     }
    | EDGE       { if (!strcmp("0z", $1))
                       $$.type = SdfPortSpec::EDGE_0Z;
                   else
                       $$.type = SdfPortSpec::EDGE_1Z;
                   free( $1);                       }
    | IDENTIFIER { if (!strcmp("z0", $1))
                       $$.type = SdfPortSpec::EDGE_Z0;
                   else
                       $$.type = SdfPortSpec::EDGE_Z1;
                   free( $1);                       }
    | NUMBER     { if ($1 < 10.0)
                       $$.type = SdfPortSpec::EDGE_01;
                   else
                       $$.type = SdfPortSpec::EDGE_10; }
    ;


port_instance
    : port { $$ = $1; }
    ;

port
    : scalar_port { $$ = $1; }
    | bus_port    { $$ = $1; }
    ;


scalar_port
    : IDENTIFIER                { $$ = $1;                                  }
    | PATH                      { $$ = $1;                                  }
    | IDENTIFIER '[' NUMBER ']' { char buf[16];
                                  int ndigit = sprintf(buf, "%d", (int)$3);
                                  $$ = new char[strlen($1) + ndigit + 3];
                                  strcpy($$, $1);
                                  strcat($$, "[");
                                  strcat($$, buf);
                                  strcat($$, "]");
                                  free( $1);                             }
    | PATH '[' NUMBER ']'       { char buf[16];
                                  int ndigit = sprintf(buf, "%d", (int)$3);
                                  $$ = new char[strlen($1) + ndigit + 3];
                                  strcpy($$, $1);
                                  strcat($$, "[");
                                  strcat($$, buf);
                                  strcat($$, "]");
                                  free( $1);                             }
    ;


bus_port
    : IDENTIFIER '[' NUMBER ':' NUMBER ']' {
        char buf1[16];
        char buf2[16];
        int ndigit1 = sprintf(buf1, "%d", (int)$3);
        int ndigit2 = sprintf(buf1, "%d", (int)$5);
        $$ = new char[strlen($1) + ndigit1 + ndigit2 + 4];
        strcpy($$, $1);
        strcat($$, "[");
        strcat($$, buf1);
        strcat($$, ":");
        strcat($$, buf2);
        strcat($$, "]");
        free( $1);
    }
    | PATH '[' NUMBER ':' NUMBER ']' {
        char buf1[16];
        char buf2[16];
        int ndigit1 = sprintf(buf1, "%d", (int)$3);
        int ndigit2 = sprintf(buf1, "%d", (int)$5);
        $$ = new char[strlen($1) + ndigit1 + ndigit2 + 4];
        strcpy($$, $1);
        strcat($$, "[");
        strcat($$, buf1);
        strcat($$, ":");
        strcat($$, buf2);
        strcat($$, "]");
        free( $1);
    }
    ;


edge_list
    : pos_pairs
    | neg_pairs
    ;


pos_pairs
    : pos_pairs pos_pair
    | pos_pair
    ;


neg_pairs
    : neg_pairs neg_pair
    | neg_pair
    ;


pos_pair
    : '(' POSEDGE NUMBER number_q ')' '(' NEGEDGE NUMBER number_q ')'
    ;


neg_pair
    : '(' NEGEDGE NUMBER number_q ')' '(' POSEDGE NUMBER number_q ')'
    ;


value
    : '(' NUMBER ')' { $$.n = 1;
                       $$.v[0] = $2;      }
    | '(' triple ')' { $$.n = $2.n;
                       $$.v[0] = $2.v[0];
                       $$.v[1] = $2.v[1];
                       $$.v[2] = $2.v[2]; }
    | '(' ')'        { $$.n = 0;          }
    ;


triple
    : NUMBER ':' ':'               { $$.n = 3;
                                     $$.v[0] = $1;
                                     $$.v[1] = 0;
                                     $$.v[2] = 0;  }
    | NUMBER ':' NUMBER ':'        { $$.n = 3;
                                     $$.v[0] = $1;
                                     $$.v[1] = $3;
                                     $$.v[2] = 0;  }
    | NUMBER ':' ':' NUMBER        { $$.n = 3;
                                     $$.v[0] = $1;
                                     $$.v[1] = 0;
                                     $$.v[2] = $4; }
    | ':' NUMBER ':'               { $$.n = 3;
                                     $$.v[0] = 0;
                                     $$.v[1] = $2;
                                     $$.v[2] = 0;  }
    | ':' NUMBER ':' NUMBER        { $$.n = 3;
                                     $$.v[0] = 0;
                                     $$.v[1] = $2;
                                     $$.v[2] = $4; }
    | ':' ':' NUMBER               { $$.n = 3;
                                     $$.v[0] = 0;
                                     $$.v[1] = 0;
                                     $$.v[2] = $3; }
    | NUMBER ':' NUMBER ':' NUMBER { $$.n = 3;
                                     $$.v[0] = $1;
                                     $$.v[1] = $3;
                                     $$.v[2] = $5; }
    ;


delval
    : value                     { $$.n = 1;
                                  sdfValueCopy($$.v[0], $1); }
    | '(' value value ')'       { $$.n = 2;
                                  sdfValueCopy($$.v[0], $2);
                                  sdfValueCopy($$.v[1], $3); }
    | '(' value value value ')' { $$.n = 3;
                                  sdfValueCopy($$.v[0], $2);
                                  sdfValueCopy($$.v[1], $3);
                                  sdfValueCopy($$.v[2], $4); }
    ;


delval_list
    : delval {
        $$.n = 1;
        sdfDelayValueCopy($$.v[0], $1);
    }
    | delval delval {
        $$.n = 2;
        sdfDelayValueCopy($$.v[0], $1);
        sdfDelayValueCopy($$.v[1], $2);
    }
    | delval delval delval {
        $$.n = 3;
        sdfDelayValueCopy($$.v[0], $1);
        sdfDelayValueCopy($$.v[1], $2);
        sdfDelayValueCopy($$.v[2], $3);
    }
    | delval delval delval delval delval delval {
        $$.n = 6;
        sdfDelayValueCopy($$.v[0], $1);
        sdfDelayValueCopy($$.v[1], $2);
        sdfDelayValueCopy($$.v[2], $3);
        sdfDelayValueCopy($$.v[3], $4);
        sdfDelayValueCopy($$.v[4], $5);
        sdfDelayValueCopy($$.v[5], $6);
    }
    | delval delval delval delval delval delval delval delval delval delval delval delval {
        $$.n = 12;
        sdfDelayValueCopy($$.v[0], $1);
        sdfDelayValueCopy($$.v[1], $2);
        sdfDelayValueCopy($$.v[2], $3);
        sdfDelayValueCopy($$.v[3], $4);
        sdfDelayValueCopy($$.v[4], $5);
        sdfDelayValueCopy($$.v[5], $6);
        sdfDelayValueCopy($$.v[6], $7);
        sdfDelayValueCopy($$.v[7], $8);
        sdfDelayValueCopy($$.v[8], $9);
        sdfDelayValueCopy($$.v[9], $10);
        sdfDelayValueCopy($$.v[10], $11);
        sdfDelayValueCopy($$.v[11], $12);
    }
    ;


expression
    : conditional_expression
    | '{' conditional_expression '}'
    | '{' expression ',' conditional_expression '}'
    ;


conditional_expression
    : simple_expression
    | simple_expression '?' expression ':' conditional_expression
    ;


simple_expression
    : '(' simple_expression ')'
    | unary_op '(' simple_expression ')'
    | port
    | unary_op port
    | scalar_constant
    | unary_op scalar_constant
    | binary_expression
    ;


binary_expression
    : simple_expression '+' simple_expression
    | simple_expression '-' simple_expression
    | simple_expression '*' simple_expression
    | simple_expression '/' simple_expression
    | simple_expression '%' simple_expression
    | simple_expression EQU simple_expression
    | simple_expression INEQU simple_expression
    | simple_expression CASE_EQU simple_expression
    | simple_expression CASE_INEQU simple_expression
    | simple_expression L_AND simple_expression
    | simple_expression L_OR simple_expression
    | simple_expression '<' simple_expression
    | simple_expression LESS_OR_EQU simple_expression
    | simple_expression '>' simple_expression
    | simple_expression GREATER_OR_EQU simple_expression
    | simple_expression '&' simple_expression
    | simple_expression '^' simple_expression
    | simple_expression '|' simple_expression
    | simple_expression RIGHT_SHIFT simple_expression
    | simple_expression LEFT_SHIFT simple_expression
    ;


timing_check_condition
    : scalar_node
    | inv_op scalar_node
    | scalar_node equ_op scalar_constant
    ;


scalar_node
    : scalar_port
    ;


scalar_constant
    : BIT_CONST
    | NUMBER
    ;


unary_op
    : '+'
    | '-'
    | '!'
    | '~'
    | '&'
    | U_NAND
    | '|'
    | U_NOR
    | '^'
    | U_XNOR
    | U_XNOR_ALT
    ;


inv_op
    : '!'
    | '~'
    ;


equ_op
    : EQU
    | INEQU
    | CASE_EQU
    | CASE_INEQU
    ;


%%

void sdf_fileerror(const char *msg) {
    fprintf(stderr, "**ERROR sdf_fileerror(): ");
    fprintf(stderr, "At line %d. Near `%s'. ", sdf_fileline, sdf_filetext);
    fprintf(stderr, "%s\n", msg);
}

void sdf_filefreeSdfDelayDef(SdfDelayDef *defs) {
}


