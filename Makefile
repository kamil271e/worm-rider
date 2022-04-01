LIBS=-lGL -lglfw -lGLEW
HEADERS=lib/allmodels.h  lib/constants.h  lib/cube.h  lib/lodepng.h  lib/model.h  lib/shaderprogram.h  lib/sphere.h  lib/teapot.h  lib/torus.h
FILES=src/cube.cpp  src/lodepng.cpp  src/main_file.cpp  src/model.cpp  src/shaderprogram.cpp  src/sphere.cpp  src/teapot.cpp  src/torus.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main.out  $(FILES)  $(LIBS) -I.
