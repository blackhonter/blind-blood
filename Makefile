build:
	g++ -g -Wall -std=c++17 ./src/*.c ./src/*.cpp -o game `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lm;

run:
	./game

clean:
	rm game
