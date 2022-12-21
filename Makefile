all:
	g++ -I src/include -L src/lib -o main main.cpp Minesweeper.cpp NormalGame.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image