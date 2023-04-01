#ifndef _H_ENTT_MAIN_CUBE_
#define _H_ENTT_MAIN_CUBE_

#include "world.h"

;
#include "physics.h"

;
struct entt_maincube : entt
{
	Model cubeModel;
	Texture2D cubeTexture;
	Shader cubeShader;

	dynamic_body* collisionBox;

	using entt::entt;

	entt_maincube() {};
	~entt_maincube() {};

	virtual void on_make() override;
	virtual void on_destroy() override;
	virtual void on_update() override;
	virtual void on_draw_2d() override;
	virtual void on_draw_3d() override;
	virtual void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation) override;
	virtual void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale) override;

	virtual entt* editor_try_select(Ray inRay, RayCollision inRayCollision) override;
};


struct entt_maincube_static : entt
{
	Model cubeModel;
	Texture2D cubeTexture;
	Shader cubeShader;

	static_body* collisionBox;

	using entt::entt;

	entt_maincube_static() {};
	~entt_maincube_static() {};

	virtual void on_make() override;
	virtual void on_destroy() override;
	virtual void on_update() override;
	virtual void on_draw_2d() override;
	virtual void on_draw_3d() override;
	virtual void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation) override;
	virtual void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale) override;

	virtual entt* editor_try_select(Ray inRay, RayCollision inRayCollision) override;
};



#else

#endif