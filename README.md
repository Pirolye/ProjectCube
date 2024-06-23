The goal of this project was to implement a simple game loop with raylib. It uses Nvidia Physx and a SIMD math library (graphene). I wrote the main engine loop in C, however it must be compiled with a C++ compiler, since it relies on PhysX.

It has an editor with some nice functionality, such as making and manipulating entities. It also has a very basic lighting implementation (based on the raylib example).

Known issues: Under some conditions, we leak about 1mb of memory every 30 seconds. Also physics.h and .cpp, the wrapper for PhysX, is currently written OOP style, will have to refactor that.

To be added: dynamic reflection, proper lighting (some kind of realtime global illumination), level saving/loading, and actual video game mechanics :)
