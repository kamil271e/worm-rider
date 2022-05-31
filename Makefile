LIBS=-lGL -lglfw -lGLEW
HEADERS=lib/constants.h  lib/cube.h  lib/lodepng.h  lib/model.h  lib/shaderprogram.h
FILES=src/cube.cpp  src/lodepng.cpp  src/main_file.cpp  src/model.cpp  src/shaderprogram.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main.out  $(FILES)  $(LIBS) -I.
