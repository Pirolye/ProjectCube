#include "world.h"
#include "raymath.h"

model* load_model(char* path, entity* inEntity)
{
	model* newModel = new model;
	newModel->model = LoadModel(path);
	newModel->container = inEntity;
	engine_add_model_to_visibility_array(inEntity->containingWorld, newModel);

	return newModel;
}

void unload_model(model* inModel)
{
	UnloadModel(inModel->model);
	delete inModel;
}

void draw_model(model* inModel)
{
	DrawModel(inModel->model, Vector3Zero(), 1.0f, WHITE);

	if (inModel->container->containingWorld->worldEditor->currentlySelectedEntity == inModel->container)
	{
		DrawModelWires(inModel->model, Vector3Zero(), 1.0f, RED);

	}
}