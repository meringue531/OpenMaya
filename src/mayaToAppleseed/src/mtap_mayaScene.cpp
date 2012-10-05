#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDagNode.h>

#include "mtap_mayaScene.h"
#include "utilities/logging.h"
#include "utilities/tools.h"

static Logging logger;


mtap_MayaScene::mtap_MayaScene()
{
	getRenderGlobals();
	this->mtap_renderer.mtap_scene = this;
	this->mtap_renderer.renderGlobals = this->renderGlobals;
	this->mtap_renderer.definePreRender();
}

mtap_MayaScene::~mtap_MayaScene()
{
}

//
// we only need to update the assembly instances, all other elements are static.
//

//TODO implement lights and cameras here and in global parseScene
void mtap_MayaScene::transformUpdateCallback(mtap_MayaObject *obj)
{
	logger.debug(MString("mtap_MayaScene::transformUpdateCallback"));

	if( obj->objectAssembly == NULL)
		return;

	if( obj->objectAssembly != NULL)
		logger.trace(MString("transformUpdateCallback:Object has objAssembly: ") + obj->shortName);

	// search for ass inst, update transform
}

//
//	will be called for every geometry deform step
//	the very first time it will create an assembly and fill it with data
//
void mtap_MayaScene::deformUpdateCallback(mtap_MayaObject *obj)
{
	if( obj->instanceNumber > 0)
		return;
	if( obj->objectAssembly == NULL)
		return;
	if( !obj->geometryShapeSupported() )
		return;

	logger.trace(MString("deformUpdateCallback:Object has objAssembly: ") + obj->shortName);

	//if( !obj.mobject.hasFn(MFn::kMesh))
	//	return
	//logger.debug(MString("mtap_MayaScene::deformUpdateCallback"));
	//this->mtap_renderer.defineMeshDeformStep( (mtap_MayaObject *)&obj);
}

MayaObject *mtap_MayaScene::mayaObjectCreator(MObject& mobject)
{
	mtap_MayaObject *mobj = new mtap_MayaObject(mobject);
	mobj->scenePtr = this;
	return mobj;
}

MayaObject *mtap_MayaScene::mayaObjectCreator(MDagPath& objDagPath)
{
	mtap_MayaObject *mobj = new mtap_MayaObject(objDagPath);
	mobj->scenePtr = this;
	return mobj;
}

void mtap_MayaScene::mayaObjectDeleter(MayaObject *obj)
{
	mtap_MayaObject *mtap_obj = (mtap_MayaObject *)obj;
	delete mtap_obj;
	obj = NULL;
}

void mtap_MayaScene::getRenderGlobals()
{
	this->renderGlobals = new mtap_RenderGlobals();
	MayaScene::renderGlobals = renderGlobals;
}


bool mtap_MayaScene::translateShaders(int timeStep)
{
	logger.debug("mtap_MayaScene::translateShaders");
	return true;
}

bool mtap_MayaScene::translateShapes(int timeStep)
{
	logger.debug("mtap_MayaScene::translateShapes");
	return true;
}

bool mtap_MayaScene::doPreRenderJobs()
{
	logger.debug("mtap_MayaScene::doPreRenderJobs");
	return true;
}

bool mtap_MayaScene::doPreFrameJobs()
{
	logger.debug("mtap_MayaScene::doPreFrameJobs");

	// creating scene, env
	//this->mtap_renderer.definePreRender();
	this->mtap_renderer.defineLights();
	return true;
}

bool mtap_MayaScene::doPostFrameJobs()
{
	logger.debug("mtap_MayaScene::doPostFrameJobs");
	return true;
}

bool mtap_MayaScene::doPostRenderJobs()
{
	logger.debug("mtap_MayaScene::doPostRenderJobs");
	return true;
}


//
//	for easy findings, put all objects into a map
//
void  mtap_MayaScene::makeMayaObjectMObjMap()
{
	mayaObjMObjMap.clear();
	std::vector<MayaObject *>::iterator iter = this->objectList.begin();
	for( ;iter != this->objectList.end(); iter++)
	{
		this->mayaObjMObjMap.append((*iter)->mobject, *iter);
	}
}

mtap_MayaObject *mtap_MayaScene::getMayaObjectFromMap(MObject& obj)
{
	mtap_MayaObject *mobj = (mtap_MayaObject *)*mayaObjMObjMap.find(obj);
	return mobj;
}

//
//	Check if the selected nodes exist in the scene->MayaObject lists. 
//	At the moment only shape nodes are exported because we do not have hierarchies yet.
//	So we make sure that we select a shape node for comparision.
//
void mtap_MayaScene::mobjectListToMayaObjectList(std::vector<MObject>& mObjectList, std::vector<MayaObject *>& mtapObjectList)
{
	std::vector<MObject>::iterator moIter, moIterFound;
	std::vector<MayaObject *>::iterator mIter;
	std::vector<MObject> cleanMObjectList;
	std::vector<MObject> foundMObjectList;

	for( moIter = mObjectList.begin(); moIter != mObjectList.end(); moIter++)
	{
		MSelectionList select;
		MString objName = getObjectName(*moIter);
		logger.debug(MString("Object to modify:") + objName);
		select.add(objName);
		MDagPath dp;
		select.getDagPath(0, dp);
		dp.extendToShape();
		MObject dagObj = dp.node();
		//MFnDagNode dagNode(*moIter);
		//dagObj = dagNode.dagPath().node();
		for( mIter = this->camList.begin(); mIter != camList.end(); mIter++)
		{
			MayaObject *mo = *mIter;
			if( dagObj == mo->mobject)
			{
				mtapObjectList.push_back(mo);
				foundMObjectList.push_back(*moIter);
			}
		}		
		for( mIter = this->objectList.begin(); mIter != objectList.end(); mIter++)
		{
			MayaObject *mo = *mIter;
			if( dagObj == mo->mobject)
			{
				mtapObjectList.push_back(mo);
				foundMObjectList.push_back(*moIter);
			}
		}		
		for( mIter = this->lightList.begin(); mIter != lightList.end(); mIter++)
		{
			MayaObject *mo = *mIter;
			if( dagObj == mo->mobject)
			{
				mtapObjectList.push_back(mo);
				foundMObjectList.push_back(*moIter);
			}
		}		
	}

	for( moIter = mObjectList.begin(); moIter != mObjectList.end(); moIter++)
	{
		bool found = false;
		for( moIterFound = foundMObjectList.begin(); moIterFound != foundMObjectList.end(); moIterFound++)
		{
			if( *moIter == *moIterFound)
			{
				found = true;
				continue;
			}
		}
		if(!found)
			cleanMObjectList.push_back(*moIter);
	}

	mObjectList = cleanMObjectList;

}

//
//	after idle process has started, this procedure is called. It does:
//		search the mobject in the MayaObject list
//		puts all found objects into a updateList
//		sets the renderer to restart. 
//	Then the renderer calls the appleseed updateEntities procedure at the beginning
//	of a new rendering.
//
void mtap_MayaScene::updateInteraciveRenderScene(std::vector<MObject> mobjList)
{
	std::vector<MayaObject *> mayaObjectList;
	mobjectListToMayaObjectList(mobjList, mayaObjectList);
	std::vector<MayaObject *>::iterator mIter;
	this->mtap_renderer.interactiveUpdateList.clear();

	for( mIter = mayaObjectList.begin(); mIter != mayaObjectList.end(); mIter++)
	{
		mtap_MayaObject *mo = (mtap_MayaObject *)*mIter;
		mo->updateObject(); // update transforms
		this->mtap_renderer.interactiveUpdateList.push_back(mo);
	}	

	this->mtap_renderer.interactiveUpdateMOList.clear();
	for( size_t i = 0; i < mobjList.size(); i++)
		this->mtap_renderer.interactiveUpdateMOList.push_back(mobjList[i]);

	if( (this->mtap_renderer.interactiveUpdateList.size() > 0) || (this->mtap_renderer.interactiveUpdateMOList.size() > 0))
		this->mtap_renderer.mtap_controller.status = asr::IRendererController::ReinitializeRendering;
		//this->mtap_renderer.mtap_controller.status = asr::IRendererController::RestartRendering;
}

void mtap_MayaScene::stopRendering()
{
	this->mtap_renderer.mtap_controller.status = asr::IRendererController::AbortRendering;
}

bool mtap_MayaScene::renderImage()
{
	logger.debug("mtap_MayaScene::renderImage");
	logger.debug(MString("Current frame: ") + this->renderGlobals->currentFrame);

	this->renderGlobals->getImageName();

	this->mtap_renderer.defineScene(this->renderGlobals, this->objectList, this->lightList, this->camList, this->instancerNodeElements);

	if( this->renderGlobals->exportXMLFile)
		this->mtap_renderer.writeXML();

	this->mtap_renderer.render();

	return true;
}

bool mtap_MayaScene::parseScene(ParseType ptype)
{
	MayaScene::parseScene(ptype);
	postParseCallback();
	return true;
}
//
//	here we build the assemblies and add them to the scene
//  Because all assemblies will need an assembly instance, the 
//	instances are defined as well.
//
bool mtap_MayaScene::postParseCallback()
{
	logger.debug("mtap_MayaScene::postParseCallback");

	std::vector<MayaObject *>::iterator mIter = this->objectList.begin();
	for(;mIter!=this->objectList.end(); mIter++)
	{
		mtap_MayaObject *obj = (mtap_MayaObject *)*mIter;
		
		mtap_ObjectAttributes *att = (mtap_ObjectAttributes *)obj->attributes;
		//logger.trace(MString("postParseCallback: Check obj ") + obj->shortName);

		if( att->needsOwnAssembly)
		{
			//logger.trace(MString("Object ") + obj->shortName + " needs own assembly");
			obj->objectAssembly = createAssembly(obj);
		}
	}

	// all assemblies need their own assembly instance because assemblies are only created where instances are necessary.
	mIter  = this->objectList.begin();
	for(;mIter!=this->objectList.end(); mIter++)
	{
		mtap_MayaObject *obj = (mtap_MayaObject *)*mIter;
		
		mtap_ObjectAttributes *att = (mtap_ObjectAttributes *)obj->attributes;
		
		if( obj->instanceNumber > 0)
			continue;

		if( obj->objectAssembly == NULL)
			continue;

		// simply add instances for all paths
		MFnDagNode objNode(obj->mobject);
		MDagPathArray pathArray;
		objNode.getAllPaths(pathArray);
				
		for( uint pId = 0; pId < pathArray.length(); pId++)
		{
			// find mayaObject...
			MDagPath currentPath = pathArray[pId];
			logger.trace(MString("Define assembly instance for obj: ") + obj->shortName + " path " + currentPath.fullPathName());		    

			asf::auto_release_ptr<asr::AssemblyInstance> ai = asr::AssemblyInstanceFactory::create(
			(currentPath.fullPathName() + "assembly_inst").asChar(),
			asr::ParamArray(),
			*obj->objectAssembly);
			this->mtap_renderer.scene->assembly_instances().insert(ai);
		}
	}

	return true;
}



asr::Assembly *mtap_MayaScene::createAssembly(mtap_MayaObject *obj)
{
	
	logger.trace(MString("Creating new assembly for: ") + obj->fullName);
	asf::auto_release_ptr<asr::Assembly> assembly = asr::AssemblyFactory::create( obj->fullName.asChar(), asr::ParamArray());
	
	this->mtap_renderer.scene->assemblies().insert(assembly);
	asr::Assembly *assemblyPtr = this->mtap_renderer.scene->assemblies().get_by_name(obj->fullName.asChar());
	
	//// for testing, can be removed later
	this->mtap_renderer.addDefaultMaterial(assemblyPtr);

	//return assembly.get();
	return assemblyPtr;
}
