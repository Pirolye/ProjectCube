#ifndef _H_ENTT_LIGHT_
#define _H_ENTT_LIGHT_

#define RLIGHTS_IMPLEMENTATION

#include "entt.h"
#include "rlights.h"

;
struct entt_light : entt
{
	using entt::entt;

	entt_light() {};
	~entt_light() {};

	Light rayLight[360];

	Model debugModel;
	virtual entt* editor_try_select(Ray inRay, RayCollision inRayCollision) override;

	void update_light_props(int inType, Vector3 inPosition, Vector3 inTarget, Color inColor);
	
	void update_spatial_props(Vector3 inNewPosition, Vector3 inNewScale, Vector3 inNewRotation) override;

	virtual void on_make() override;
	virtual void on_destroy() override;
	virtual void on_update() override;
	virtual void on_draw_2d() override;
	virtual void on_draw_3d() override;

};





#else

#endif