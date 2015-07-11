// **************************************************************************
// File       [ primitive.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/04/19 created ]
// **************************************************************************

#ifndef __INTF_PRIMITIVE_H__
#define __INTF_PRIMITIVE_H__

#include "module.h"

namespace IntfNs {

//{{{ class Pmt
class Pmt : public Module {
public:
    virtual ~Pmt() {};

    enum Type { AND,   NAND,  OR,     NOR,
                INV,   BUF,   BUFZ,
                XOR,   XNOR,
                TSL,   TSLI,  TSH,    TSHI,
                MUX,   DFF,   DLAT,   DELAY,  INVF,   WIRE, PULL,
                TIE1,  TIE0,  TIEX,   TIEZ,   UNDEF,
                NMOS,  PMOS,  RNMOS,  RPMOS,  NMOSF,  PMOSF,
                CMOS1, CMOS2, RCMOS1, RCMOS2, CMOS1F, CMOS2F
              };

    Type getType() const;

protected:
    Pmt(const char * const name) : Module(name) { ispmt_ = true; };
    Type type_;
};

inline Pmt::Type Pmt::getType() const {
    return type_;
}
//}}}

//{{{ class PmtAnd
class PmtAnd : public Pmt {
public:
    PmtAnd(const char * const name) : Pmt(name) { type_ = AND; };
    ~PmtAnd() {};
}; //}}}
//{{{ class PmtNand
class PmtNand : public Pmt {
public:
    PmtNand(const char * const name) : Pmt(name) { type_ = NAND; };
    ~PmtNand() {};
}; //}}}
//{{{ class PmtOr
class PmtOr : public Pmt {
public:
    PmtOr(const char * const name) : Pmt(name) { type_ = OR; };
    ~PmtOr() {};
}; //}}}
//{{{ class PmtNor
class PmtNor : public Pmt {
public:
    PmtNor(const char * const name) : Pmt(name) { type_ = NOR; };
    ~PmtNor() {};
}; //}}}
//{{{ class PmtInv
class PmtInv : public Pmt {
public:
    PmtInv(const char * const name) : Pmt(name) { type_ = INV; };
    ~PmtInv() {};
}; //}}}
//{{{ class PmtBuf
class PmtBuf : public Pmt {
public:
    PmtBuf(const char * const name) : Pmt(name) { type_ = BUF; };
    ~PmtBuf() {};
}; //}}}
//{{{ class PmtBufz
class PmtBufz : public Pmt {
public:
    PmtBufz(const char * const name) : Pmt(name) { type_ = BUFZ; };
    ~PmtBufz() {};
}; //}}}
//{{{ class PmtXor
class PmtXor : public Pmt {
public:
    PmtXor(const char * const name) : Pmt(name) { type_ = XOR; };
    ~PmtXor() {};
}; //}}}
//{{{ class PmtXnor
class PmtXnor : public Pmt {
public:
    PmtXnor(const char * const name) : Pmt(name) { type_ = XNOR; };
    ~PmtXnor() {};
}; //}}}
//{{{ class PmtTsl
class PmtTsl : public Pmt {
public:
    PmtTsl(const char * const name) : Pmt(name) { type_ = TSL; };
    ~PmtTsl() {};
}; //}}}
//{{{ class PmtTsli
class PmtTsli : public Pmt {
public:
    PmtTsli(const char * const name) : Pmt(name) { type_ = TSLI; };
    ~PmtTsli() {};
}; //}}}
//{{{ class PmtTsh
class PmtTsh : public Pmt {
public:
    PmtTsh(const char * const name) : Pmt(name) { type_ = TSH; };
    ~PmtTsh() {};
}; //}}}
//{{{ class PmtTshi
class PmtTshi : public Pmt {
public:
    PmtTshi(const char * const name) : Pmt(name) { type_ = TSHI; };
    ~PmtTshi() {};
}; //}}}
//{{{ class PmtMux
class PmtMux : public Pmt {
public:
    PmtMux(const char * const name) : Pmt(name) { type_ = MUX; };
    ~PmtMux() {};
}; //}}}
//{{{ class PmtDff
class PmtDff : public Pmt {
public:
    PmtDff(const char * const name) : Pmt(name) { type_ = DFF; };
    ~PmtDff() {};
}; //}}}
//{{{ class PmtDlat
class PmtDlat : public Pmt {
public:
    PmtDlat(const char * const name) : Pmt(name) { type_ = DLAT; };
    ~PmtDlat() {};
}; //}}}
//{{{ class PmtDelay
class PmtDelay : public Pmt {
public:
    PmtDelay(const char * const name) : Pmt(name) { type_ = DELAY; };
    ~PmtDelay() {};
}; //}}}
//{{{ class PmtInvf
class PmtInvf : public Pmt {
public:
    PmtInvf(const char * const name) : Pmt(name) { type_ = INVF; };
    ~PmtInvf() {};
}; //}}}
//{{{ class PmtWire
class PmtWire : public Pmt {
public:
    PmtWire(const char * const name) : Pmt(name) { type_ = WIRE; };
    ~PmtWire() {};
}; //}}}
//{{{ class PmtPull
class PmtPull : public Pmt {
public:
    PmtPull(const char * const name) : Pmt(name) { type_ = PULL; };
    ~PmtPull() {};
}; //}}}
//{{{ class PmtTie1
class PmtTie1 : public Pmt {
public:
    PmtTie1(const char * const name) : Pmt(name) { type_ = TIE1; };
    ~PmtTie1() {};
}; //}}}
//{{{ class PmtTie0
class PmtTie0 : public Pmt {
public:
    PmtTie0(const char * const name) : Pmt(name) { type_ = TIE0; };
    ~PmtTie0() {};
}; //}}}
//{{{ class PmtTieX
class PmtTieX : public Pmt {
public:
    PmtTieX(const char * const name) : Pmt(name) { type_ = TIEX; };
    ~PmtTieX() {};
}; //}}}
//{{{ class PmtTieZ
class PmtTieZ : public Pmt {
public:
    PmtTieZ(const char * const name) : Pmt(name) { type_ = TIEZ; };
    ~PmtTieZ() {};
}; //}}}
//{{{ class PmtUndef
class PmtUndef : public Pmt {
public:
    PmtUndef(const char * const name) : Pmt(name) { type_ = UNDEF; };
    ~PmtUndef() {};
}; //}}}
//{{{ class PmtNmos
class PmtNmos : public Pmt {
public:
    PmtNmos(const char * const name) : Pmt(name) { type_ = NMOS; };
    ~PmtNmos() {};
}; //}}}
//{{{ class PmtPmos
class PmtPmos : public Pmt {
public:
    PmtPmos(const char * const name) : Pmt(name) { type_ = PMOS; };
    ~PmtPmos() {};
}; //}}}
//{{{ class PmtRnmos
class PmtRnmos : public Pmt {
public:
    PmtRnmos(const char * const name) : Pmt(name) { type_ = PMOS; };
    ~PmtRnmos() {};
}; //}}}
//{{{ class PmtRpmos
class PmtRpmos : public Pmt {
public:
    PmtRpmos(const char * const name) : Pmt(name) { type_ = PMOS; };
    ~PmtRpmos() {};
}; //}}}
//{{{ class PmtNmosf
class PmtNmosf : public Pmt {
public:
    PmtNmosf(const char * const name) : Pmt(name) { type_ = NMOSF; };
    ~PmtNmosf() {};
}; //}}}
//{{{ class PmtPmosf
class PmtPmosf : public Pmt {
public:
    PmtPmosf(const char * const name) : Pmt(name) { type_ = PMOSF; };
    ~PmtPmosf() {};
}; //}}}
//{{{ class PmtCmos1
class PmtCmos1 : public Pmt {
public:
    PmtCmos1(const char * const name) : Pmt(name) { type_ = CMOS1; };
    ~PmtCmos1() {};
}; //}}}
//{{{ class PmtCmos2
class PmtCmos2 : public Pmt {
public:
    PmtCmos2(const char * const name) : Pmt(name) { type_ = CMOS2; };
    ~PmtCmos2() {};
}; //}}}
//{{{ class PmtRcmos1
class PmtRcmos1 : public Pmt {
public:
    PmtRcmos1(const char * const name) : Pmt(name) { type_ = RCMOS1; };
    ~PmtRcmos1() {};
}; //}}}
//{{{ class PmtRcmos2
class PmtRcmos2 : public Pmt {
public:
    PmtRcmos2(const char * const name) : Pmt(name) { type_ = RCMOS2; };
    ~PmtRcmos2() {};
}; //}}}
//{{{ class PmtCmos1f
class PmtCmos1f : public Pmt {
public:
    PmtCmos1f(const char * const name) : Pmt(name) { type_ = CMOS1F; };
    ~PmtCmos1f() {};
}; //}}}
//{{{ class PmtCmos2f
class PmtCmos2f : public Pmt {
public:
    PmtCmos2f(const char * const name) : Pmt(name) { type_ = CMOS2F; };
    ~PmtCmos2f() {};
}; //}}}

};

#endif

