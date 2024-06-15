#ifndef _H_ENTT_LIGHT_
#define _H_ENTT_LIGHT_

#define RLIGHTS_IMPLEMENTATION

#include "entt.h"
#include "rlights.h"

;
struct entt_light : entt
{
	Light rayLight[360];

	Model debugModel;
	

};

void on_make(entt_light* inEntt);

void on_update(entt_light* inEntt);
void on_draw_2d(entt_light* inEntt);
void on_draw_3d(entt_light* inEntt);

void update_spatial_props(entt_light* inEntt, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation);
void update_spatial_props(entt_light* inEntt, Vector3 inNewPos, Vector3 inNewScale);
void update_light_props(entt_light* inEntt, int inType, Vector3 inPosition, Vector3 inTarget, Color inColor);
void on_destroy(entt_light* inEntt);

entt_light* editor_try_select(entt_light* inEntt);




#else

#endif