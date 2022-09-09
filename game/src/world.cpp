#include "world.h"
#include "entt_camera.h"
#include "entt_maincube.h"

;
#include "assert.h"

#define FLT_MAX 340282346638528859811704183484516925440.0f 


world::world() 
{
	//(Levente): Okay... this is clever but not very logical. Apparently 0xcdcdcd... is not a nullptr so we individually assign NULL to every uninitialized entt in the array!
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		entityArray[i] = NULL;
	}

	//(Levente): Same for the shaders.
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		currentlyLoadedShaders[i] = { 0 };
	}
	
	name = "debug";

	/*
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	*/

	/*
	dInitODE2(0);

	physicsWorld = dWorldCreate();
	dWorldSetGravity(physicsWorld, 0.0, -9.81, 0.0);
	physicsSpace = dHashSpaceCreate(NULL);
	collisionContactGroup = dJointGroupCreate(0);
	collisionData = collision_data{ physicsWorld, collisionContactGroup };

	groundGeom = dCreatePlane(physicsSpace, 0, 1, 0, 0);
	*/

	physicsSpace = new q3Scene(1.0 / 60);
	physicsSpace->SetGravity(q3Vec3(0.0, -1.0, 0.0));

	//q3Scene scene(1.0 / 60.0);
	//physicsSpace = scene;

	run_script_on_init();

}

world::~world() //(Levente): Technically this is really bad. We will need a proper shutdown procedure!
{
	/*
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != nullptr)
		{
			entityArray[i]->on_destroy();
			delete entityArray[i];
		}
		else
		{
			continue;
		}
	}
	*/

}

Shader world::make_shader(const char* vertexShader, const char* fragmentShader)
{
	Shader temp = LoadShader(vertexShader, fragmentShader);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		if (currentlyLoadedShaders[i].id == 0 && currentlyLoadedShaders[i].locs == 0)
		{
			currentlyLoadedShaders[i] = temp;
			break;
		}
	}

	return temp;
}

entt* world::make_desired_entt(entts inDesiredEntt)
{
	switch (inDesiredEntt)
	{
		case entts::cam:
		{
			entt_camera* cam = new entt_camera;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			cam->id = "entt_camera_" + std::to_string(totalMadeEntts);
			cam->containingWorld = this;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = cam;
					break;
				}
			}

			cam->on_make();

			return cam;

			break;
		}

		case entts::camSetCurrentlyRendering:
		{
			entt_camera* camS = new entt_camera;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			camS->id = "entt_camera_" + std::to_string(totalMadeEntts);
			camS->containingWorld = this;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = camS;
					break;
				}
			}

			currentlyRenderingCam = camS;
			camS->currentlyDrawing = true;

			camS->on_make();

			return camS;

			break;
		}

		case entts::mainCube:
		{
			entt_maincube* c = new entt_maincube;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			c->id = "entt_maincube_" + std::to_string(totalMadeEntts);
			c->containingWorld = this;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = c;
					break;
				}
			}

			c->on_make(); 
			
			return c;

			break;
		}

		case entts::light:
		{
			entt_light* l1 = new entt_light;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			l1->id = "entt_point_light_" + std::to_string(totalMadeEntts);
			l1->containingWorld = this;

			l1->on_make();

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = l1;
					break;
				}
			}

			return l1;

			break;
		}


		default:
		{
			return nullptr;
		}

	}
}

void world::update()
{
	assert(currentlyRenderingCam != nullptr);

	UpdateCamera((currentlyRenderingCam->rayCam));

	cameraSwitchedLastFrame = false;
	if (GetFrameTime() > 0)
	{
		//dynamicsWorld->stepSimulation(1.0f / 60.0f, 10);
		
		/*
		dSpaceCollide(physicsSpace, &collisionData, &nearCallback);
		dWorldQuickStep(physicsWorld, 1 / 60.0f);
		dJointGroupEmpty(collisionContactGroup);
		*/

		physicsSpace->Step();
	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_update();
		}
	}

	if (IsKeyPressed(KEY_F1))
	{
		#ifdef DEBUG 
		isInEditorMode = true;
		#else
		isInEditorMode = false;
		#endif
	}

	if(isInEditorMode) update_world_editor();

	run_script_on_update();


}

void world::draw_all()
{
	assert(currentlyRenderingCam != nullptr);

	BeginDrawing();

	BeginMode3D(*(currentlyRenderingCam->rayCam));
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_draw_3d();
		}
	}
	EndMode3D();

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_draw_2d();
		}
	}

	EndDrawing();

}

void world::on_destroy()
{
	run_script_on_destroy();
	
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_destroy();
		}
	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			delete entityArray[i];
		}
	}
	
	/*for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}*/


	// delete dynamics world
	//delete dynamicsWorld;

	// delete solver
	//delete solver;

	// delete broadphase
	//delete overlappingPairCache;

	// delete dispatcher
	//delete dispatcher;

	//delete collisionConfiguration;

	/*
	dSpaceDestroy(physicsSpace);
	dWorldDestroy(physicsWorld);
	dCloseODE();
	*/
}


/*
void nearCallback(void* inData, dGeomID inGeom1, dGeomID inGeom2)
{
	auto collisionData = static_cast<collision_data*>(inData);

	// Get the rigid bodies associated with the geometries
	dBodyID body1 = dGeomGetBody(inGeom1);
	dBodyID body2 = dGeomGetBody(inGeom2);

	// Maximum number of contacts to create between bodies (see ODE documentation)
	const int MAX_NUM_CONTACTS = 8;
	dContact contacts[MAX_NUM_CONTACTS];

	// Add collision joints
	int numc = dCollide(inGeom1, inGeom2, MAX_NUM_CONTACTS, &contacts[0].geom, sizeof(dContact));

	for (int i = 0; i < numc; ++i) {

		contacts[i].surface.mode = dContactMotion1;

		contacts[i].surface.mu = 50.0;
		contacts[i].surface.soft_erp = 0.96;
		contacts[i].surface.soft_cfm = 2.00;

		// struct dSurfaceParameters {
		//      int mode;
		//      dReal mu;
		//      dReal mu2;
		//      dReal rho;
		//      dReal rho2;
		//      dReal rhoN;
		//      dReal bounce;
		//      dReal bounce_vel;
		//      dReal soft_erp;
		//      dReal soft_cfm;
		//      dReal motion1, motion2, motionN;
		//      dReal slip1, slip2;
		// };

		dJointID contact = dJointCreateContact(collisionData->world, collisionData->contactGroup, &contacts[i]);

		dJointAttach(contact, body1, body2);
	}
}
*/
/*
void nearCallback(void* data, dGeomID o1, dGeomID o2)
{
	if (dGeomIsSpace(o1) || dGeomIsSpace(o2)) {
		// colliding a space with something :
		dSpaceCollide2(o1, o2, data, &nearCallback);
		// collide all geoms internal to the space(s)
		if (dGeomIsSpace(o1))
			dSpaceCollide((dSpaceID)o1, data, &nearCallback);
		if (dGeomIsSpace(o2))
			dSpaceCollide((dSpaceID)o2, data, &nearCallback);
	}
	else {
		// colliding two non-space geoms, so generate contact
		// points between o1 and o2
		const int MAX_NUM_CONTACTS = 8;
		dContact contacts[MAX_NUM_CONTACTS];

		int num_contact = dCollide(o1, o2, 8, &contacts[0].geom, sizeof(dContact));
		// add these contact points to the simulation ... 
	}
}*/

/*
* 
* 
* EDITOR FUNCTION DEFINITIONS
* 
* 
*/

#ifdef DEBUG

void world::update_world_editor()
{

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) editor_try_select_entt();

	//Will be removed once we got editor gui
	if (currentlySelectedEntt != NULL) DrawText(currentlySelectedEntt->id.c_str(), 10, 550, 20, RED);

	if (IsKeyPressed(KEY_TAB))
	{

		for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
		{
			if (entityArray[i] != NULL)
			{
				if (dynamic_cast<entt_camera*>(entityArray[i]) != nullptr)
				{
					entt_camera* foundCam = dynamic_cast<entt_camera*>(entityArray[i]);

					if (foundCam != currentlyRenderingCam)
					{
						currentlyRenderingCam->currentlyDrawing = false;
						foundCam->currentlyDrawing = false;

						entt_camera* prev = currentlyRenderingCam;

						currentlyRenderingCam = foundCam;
						currentlyRenderingCam->currentlyDrawing = true;

						prev->currentlyDrawing = false;

						cameraSwitchedLastFrame = true;

						break;
					}


				}
			}
		}

	}


}

void world::editor_try_select_entt()
{
	RayCollision collision = { 0 };
	collision.distance = FLT_MAX;
	collision.hit = false;

	cursorSelectionRay = GetMouseRay(GetMousePosition(), *(currentlyRenderingCam->rayCam));

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			if (dynamic_cast<entt_light*>(entityArray[i]) != nullptr) currentlySelectedEntt = dynamic_cast<entt_light*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_maincube*>(entityArray[i]) != nullptr) currentlySelectedEntt = dynamic_cast<entt_maincube*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_camera*>(entityArray[i]) != nullptr) currentlySelectedEntt = dynamic_cast<entt_camera*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			
			if (currentlySelectedEntt != nullptr) break;
			else continue;
		}
	}
};



#else

void world::update_world_editor() {};
void world::editor_try_select_entt() {};


#endif