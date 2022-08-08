#ifndef _ENTT_H_
#define _ENTT_H_

enum entts { cameraDefConfig, mainCubeDefConfig };

struct entt
{
	entt();
	virtual ~entt();

	virtual void on_make();
	virtual void on_destroy();
	virtual void on_update();
	virtual void on_draw();
};



#endif