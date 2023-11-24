all:
	g++ -Isrc/include -Lsrc/lib -o run main.cpp -lmingw32 -lSDL2main -lSDL2 -fopenmp -lSDL2_image -lSDL2_mixer