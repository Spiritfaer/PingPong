#include <iostream>
#include <sstream>
#include <stdint.h>
#include <SDL.h>
#include "Core.h"
#include "Obj.h"
#include "Boll.h"
#include "Player.h"
#include "Bot.h"

int main(int argc, char * argv[])
{
	try {
		std::unique_ptr<Core> core(new Core());
		core->run();
	}
	catch (const std::exception &err) {
		std::cerr << err.what() << std::endl;
		return (1);
	}
	return (0);
}
