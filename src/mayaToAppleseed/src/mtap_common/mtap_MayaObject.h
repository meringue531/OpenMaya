#ifndef MTAP_MAYA_OBJECT_H
#define MTAP_MAYA_OBJECT_H

#include <maya/MMatrix.h>

#include "mayaObject.h"
#include "renderer/api/scene.h"

namespace asr = renderer;
namespace asf = foundation;

class mtap_MayaObject;

class mtap_ObjectAttributes : public ObjectAttributes
{
public:
	mtap_ObjectAttributes();
	mtap_ObjectAttributes(std::shared_ptr<ObjectAttributes>);
	bool needsOwnAssembly;
	MMatrix objectMatrix;
	//std::shared_ptr<MayaObject> assemblyObject;
};

class mtap_MayaObject : public MayaObject
{
public:
	mtap_MayaObject(MObject&);
	mtap_MayaObject(MDagPath&);
	~mtap_MayaObject();

	virtual bool geometryShapeSupported();
	virtual std::shared_ptr<ObjectAttributes> getObjectAttributes(std::shared_ptr<ObjectAttributes> parentAttributes = nullptr);
	std::shared_ptr<MayaObject> getAssemblyObject();
	MString getAssemblyInstName();
	MString getAssemblyName();
	MString getObjectName();
	bool needsAssembly();
	asr::Assembly *getObjectAssembly();
	asr::Assembly *objectAssembly;
};

#endif