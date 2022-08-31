#ifndef _H_ENTT_MAIN_CUBE_
#define _H_ENTT_MAIN_CUBE_

#include "world.h"

;
struct entt_maincube : entt
{
	Model cubeModel;
	Texture2D cubeTexture;
	Shader cubeShader;

	using entt::entt;

	entt_maincube() {};
	~entt_maincube() {};

	virtual void on_make() override;
	virtual void on_destroy() override;
	virtual void on_update() override;
	virtual void on_draw_2d() override;
	virtual void on_draw_3d() override;
	virtual void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRotation) override;
	virtual void update_spatial_props(entt_transform inNewEnttTransform) override;

	virtual entt* try_select(Ray inRay, RayCollision inRayCollision) override;
	BoundingBox selectionBoundingBox;
};



#else

#endif