all:
	g++ -I src/include -L src/lib -o main main.cpp Minesweeper.cpp NormalGame.cpp TimedGame.cpp FourGame.cpp SQLHelpers.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lsqlite3