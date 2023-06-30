#include <cstdlib>
#include <cstdio>
#include <ctime>

#include <physfs.h>

#include "game.hpp"

int main(int argc, char * argv[])
{
	srand(time(NULL));

	int rc = PHYSFS_init(argv[0]);
	if (rc == 0)
	{
		printf("PhysicsFS failed to start.\n");
		return 1;
	}

	duckhacker::Game game;
	game.Run();
	return 0;
}
