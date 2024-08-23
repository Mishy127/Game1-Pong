
CFLAGS=-g -Wall -Wextra `pkg-config --cflags sdl2 SDL2_mixer`
LDLIBS=-lm `pkg-config --libs sdl2 SDL2_mixer`

Pong: .cache/main.o .cache/Game.o .cache/Paddle.o .cache/Ball.o
	cc .cache/main.o .cache/Game.o .cache/Paddle.o .cache/Ball.o ${LDLIBS} -o Pong -lstdc++

.cache/main.o: src/main.cpp src/Game.h
	cc ${CFLAGS} src/main.cpp -c -o .cache/main.o

.cache/Game.o: src/Game.cpp  src/Game.h
	cc ${CFLAGS} src/Game.cpp -c -o .cache/Game.o

.cache/Ball.o: src/Ball.cpp src/Ball.h src/Game.h
	cc ${CFLAGS} src/Ball.cpp -c -o .cache/Ball.o

.cache/Paddle.o: src/Paddle.cpp src/Paddle.h src/Game.h
	cc ${CFLAGS} src/Paddle.cpp -c -o .cache/Paddle.o

