#include "marble.h"
#include <math.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnGenericAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFloatVector.h>

MTypeId Marble::id( 0x0011EF62 );

MObject Marble::outColor;

	
//---------------------------- automatically created static attributes start ------------------------------------
MObject	Marble::high;
MObject	Marble::low;
MObject	Marble::octaves;
MObject	Marble::omega;
MObject	Marble::variation;
//---------------------------- automatically created static attributes end ------------------------------------
	

#define MAKE_INPUT(attr)                                \
 CHECK_MSTATUS ( attr.setKeyable(true) );               \
 CHECK_MSTATUS ( attr.setStorable(true) );              \
 CHECK_MSTATUS ( attr.setReadable(true) );              \
 CHECK_MSTATUS ( attr.setWritable(true) );

#define MAKE_OUTPUT(attr)                               \
  CHECK_MSTATUS ( attr.setKeyable(false) );             \
  CHECK_MSTATUS ( attr.setStorable(false) );            \
  CHECK_MSTATUS ( attr.setReadable(true) );             \
  CHECK_MSTATUS ( attr.setWritable(false) );

//
// DESCRIPTION:
void Marble::postConstructor( )
{
    setMPSafe(true);
}

//
// DESCRIPTION:
Marble::Marble()
{
}

//
// DESCRIPTION:
Marble::~Marble()
{
}

//
// DESCRIPTION:
void *Marble::creator()
{
    return new Marble();
}

//
// DESCRIPTION:
MStatus Marble::initialize()
{
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnGenericAttribute gAttr;
	MFnEnumAttribute eAttr;
	MFnMessageAttribute mAttr;

	MStatus status;

	outColor = nAttr.createColor("outColor", "outColor");
	MAKE_OUTPUT(nAttr);
	CHECK_MSTATUS(addAttribute( outColor ));

//---------------------------- automatically created attributes start ------------------------------------
	high = nAttr.createColor("high", "high");
	nAttr.setDefault(0,0,0);
	CHECK_MSTATUS(addAttribute( high ));


	low = nAttr.createColor("low", "low");
	nAttr.setDefault(1,1,1);
	CHECK_MSTATUS(addAttribute( low ));


	octaves = nAttr.create("octaves", "octaves",  MFnNumericData::kInt, 8);
	CHECK_MSTATUS(addAttribute( octaves ));


	omega = nAttr.create("omega", "omega",  MFnNumericData::kFloat, 0.5);
	CHECK_MSTATUS(addAttribute( omega ));


	variation = nAttr.create("variation", "variation",  MFnNumericData::kFloat, 1.0);
	CHECK_MSTATUS(addAttribute( variation ));


//---------------------------- automatically created attributes end ------------------------------------

    return MS::kSuccess;
}

//
// DESCRIPTION:
MStatus Marble::compute(const MPlug &plug, MDataBlock &block)
{
    return MS::kSuccess;
}
