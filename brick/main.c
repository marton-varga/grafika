#include "brick.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    Brick brick;
    set_size(&brick, 5, 10, 8);

	double volume = calc_volume(&brick);
	double surface = calc_surface(&brick);
	int square_side = does_have_square_side(&brick);

	printf("Brick volume: %lf\n", volume);
	printf("Brick surface: %lf\n", surface);
	char b[6];
	if(square_side){
        strcpy(b, "True");
	}else{
        strcpy(b, "False");
	}
	printf("Has square side: %s.\n", b);

    return 0;
}
