#ifndef _H_ENTT_MAIN_CUBE_
#define _H_ENTT_MAIN_CUBE_

#include "world.h"

;
struct entt_maincube : entt
{
	Model cubeModel;
	Texture2D cubeTexture;
	Vector3 pos;
	Shader cubeShader;

	using entt::entt;

	entt_maincube() {};
	~entt_maincube() {};

	virtual void on_make() override;
	virtual void on_destroy() override;
	virtual void on_update() override;
	virtual void on_draw_2d() override;
	virtual void on_draw_3d() override;

};



#else

#endif