#include <irrlicht.h>
#include "AssimpWrapper.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _DEBUG
#pragma comment(lib, "Irrlicht_d.lib")
#else
#pragma comment(lib, "Irrlicht.lib")
#endif
#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")

#define WORLD_SIZE 100
#define NODE_COUNT 100
IAnimatedMeshSceneNode* gNodes[NODE_COUNT];

IrrlichtDevice *device;
f32 random(float min, float max)
{
    static IRandomizer* randomizer = device->getRandomizer();
    f32 v = randomizer->frand();
    return min + v * (max - min);
}

int main()
{
	device = createDevice(video::EDT_OGLES2, dimension2d<u32>(800, 600), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"SynthFlow");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

    scene::ISceneNode* skydome = smgr->addSkyDomeSceneNode(driver->getTexture("../../media/skydome.jpg"), 16, 8, 0.95f, 2.0f);

    c8* meshFiles[] =
    {
        //"../../media/duck.fbx",
        "../../media/Cockatoo/Cockatoo.FBX",
    };

    c8* texFiles[] =
    {
        "../../media/Shanghai5.jpg",
        "../../media/Belgium_block_pxr128.jpg",
        "../../media/Slag_stone_pxr128.jpg",
        "../../media/dwarf.jpg",
        "../../media/Cockatoo/Cockatoo_D.png",
    };

    const float kCamDistZ = 40;
    int idx = 0;
    for (auto node : gNodes)
    {
        auto emptyNode = smgr->addEmptySceneNode();
        emptyNode->setPosition({
            random(-WORLD_SIZE, WORLD_SIZE),
            random(-WORLD_SIZE, WORLD_SIZE),
            random(-WORLD_SIZE, WORLD_SIZE)
        });

        IAnimatedMesh* mesh = getMeshFromAssimp(smgr, meshFiles[rand() % _countof(meshFiles)]);
        node = smgr->addAnimatedMeshSceneNode(mesh, emptyNode);
        node->setFrameLoop(0, 0);

        core::aabbox3df bbox = node->getBoundingBox();
        float newScale = kCamDistZ * 0.6f / bbox.getRadius();
        node->setScale({ newScale, newScale, newScale });
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setMaterialTexture(0, driver->getTexture(texFiles[rand() % _countof(texFiles)]));

        // animator
        auto rotAnimator = smgr->createRotationAnimator({
            random(0, 1),
            random(0, 1),
            random(0, 1),
        });
        node->addAnimator(rotAnimator);
        rotAnimator->drop();

        auto flyAnimator = smgr->createFlyCircleAnimator({}, random(0, 20));
        node->addAnimator(flyAnimator);
        flyAnimator->drop();
    }

#if 1
    smgr->addCameraSceneNode(0, vector3df(0, 0, -kCamDistZ * 4), vector3df(0, 0, 0));
#else
	auto camera = smgr->addCameraSceneNodeFPS(0);
    camera->setPosition({ 0.0f, 0.0f, -kCamDistZ * 3 });
#endif

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		//guienv->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}

/*
That's it. Compile and run.
**/
