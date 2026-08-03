#include "main_scene.h"
#include "systems/network_system.h"
#include "input.h"

void MainScene::enter() {
	net::init();
}

void MainScene::exit() {
	net::exit();
}

void MainScene::update(float dt) {
	
	if (input::pressed(KEY_A)) {
		net::host_game();
	}
	if (input::pressed(KEY_B)) {
		net::find_and_join_host();
	}

	net::update();
}

void MainScene::renderBottom() {
	
	char buf[64];
	snprintf(buf, sizeof(buf), "Connected: %s Host: %s", net::is_connected() ? "yes" : "no", net::is_host() ? "yes" : "no");
	renderer->drawText(buf, 0, 0, C2D_Color32(255, 255, 255, 255), 1);

}