all:
	gcc -Iinclude/ app.c camera.c main.c scene.c texture.c utils.c sound.c model_wrapper.c -o raptor.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -L. -lSDL2_ttf -lobj -lopengl32 -lm -Wall -Wextra -Wpedantic -lwinmm

linux:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/scene.c src/texture.c src/utils.c -lobj -lSDL2 -lSDL2_image -lGL -lm -o cube -Wall -Wextra -Wpedantic
