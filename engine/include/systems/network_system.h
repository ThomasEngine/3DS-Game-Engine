#pragma once
#include <3ds.h>


namespace net {
	void init();
	void exit();
	bool host_game();
	bool find_and_join_host();
	void update();

	bool is_connected();
	bool is_host();
};