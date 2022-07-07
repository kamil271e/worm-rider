#ifndef CUBE_H
#define CUBE_H

#include "model.h"

namespace Models {
	namespace CubeInternal {
		extern float vertices[];
		extern float normals[];
		extern float vertexNormals[];
		extern float texCoords[];
		extern float colors[];
		extern unsigned int vertexCount;
	}

	class Cube: public Model {
		public:
			Cube();
			virtual ~Cube();
			virtual void drawSolid(bool smooth=false);
	};

	extern Cube cube;
}

#endif
