#include <iostream>
#include <sstream>
#include <stdint.h>
#include <SDL.h>

#include "Core.h"
#include "Obj.h"
#include "Boll.h"
#include "Player.h"

int main(int argc, char * argv[])
{
	try {
		std::unique_ptr<Core> core(new Core());

		std::unique_ptr<Obj> boll(new Boll((core->screen.w / 2 - 25 / 2), (core->screen.h / 2 - 25 / 2), 25, 25, 0xFFD600));
		std::unique_ptr<Obj> player1(new Player(25, (core->screen.h / 2 - 100 / 2), SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, 0xFFFFFF));
		std::unique_ptr<Obj> player2(new Player(core->screen.w - (25 + 15), (core->screen.h / 2 - 100 / 2), SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, 0xFFFFFF));

		core->add_new_obj(std::move(boll));
		core->add_new_obj(std::move(player1));
		core->add_new_obj(std::move(player2));
		core->refresh_player();

		while (true) {
			core->refresh_event();
			core->fps.tick(true);
			if (core->is_exit())
				break;
			core->clear_window();
			core->refresh_fild();
			core->refresh_obj();
			core->refresh_window();
		}
	}
	catch (const std::exception &err) {
		std::cerr << err.what() << std::endl;
		return (1);
	}
	return (0);
}
