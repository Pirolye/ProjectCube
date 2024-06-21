#ifndef _H_RAYLIB_WRAPPER_H_
#define _H_RAYLIB_WRAPPER_H_


#include "raylib.h"
#include "entity.h"

struct model
{
	Model model;
	entity* container;
};

model* load_model(char* path, entity* inEntity);
void unload_model(model* inModel);

void draw_model(model* inModel);
void draw_model(model* inModel, Color tint);

#else

#endif
