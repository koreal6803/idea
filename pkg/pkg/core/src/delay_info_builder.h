
// **************************************************************************
// File       [ sdf_reader.h ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/06 created ]
// **************************************************************************

#ifndef __CORE_SDF_READER_H__
#define __CORE_SDF_READER_H__
#include "interface/src/sdf_file.h"

#include <map>
#include <string>
class Circuit;
class DelayInfo;
class Library;
using namespace IntfNs;

class DelayInfoBuilder :public IntfNs::SdfFile
{
public:

	DelayInfoBuilder();
	~DelayInfoBuilder();

	void 		set(Circuit *cir);
	void 		set(Library *lib);
	bool 		read(const char * const fname);
	DelayInfo 	getDelayInfo();

private:

	Circuit 	*cir;
	DelayInfo 	*delay;
	Library 	*lib;
	
	int 		presentCellID;

	virtual bool 		addInterconnectDelay(const SdfDelayType &type
       			, const char * const from
       			, const char * const to
       			, const SdfDelayValueList &v);
    virtual bool 		addCell(const char * const type, const char * const name);
    virtual bool 		addIoDelay(const SdfDelayType &type
				, const SdfPortSpec &spec
				, const char * const port
				, const SdfDelayValueList &v);
};



#endif
