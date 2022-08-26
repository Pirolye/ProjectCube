#include "world.h"
#include <string>

;
std::string entt::get_id()
{
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (dynamic_cast<entt*>(containingWorld->entityArray[i]) == this)
		{
			return containingWorld->entityArray[i]->id;
		}
	}

}