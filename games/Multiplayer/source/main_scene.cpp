#include "main_scene.h"
#include "systems/network_system.h"
#include "input.h"

void MainScene::enter() {
	networkSystem = new NetworkSystem();
	networkSystem->networkInit();
}

void MainScene::exit() {
	networkSystem->networkShutdown();
}

void MainScene::update(float dt) {
	
	if (input::pressed(KEY_A)) {
		networkSystem->hostGame();
	}
	if (input::pressed(KEY_B)) {
		networkSystem->findAndJoinGame();
	}

	networkSystem->update();
}

void MainScene::renderBottom() {
	
	char buf[64];
	snprintf(buf, sizeof(buf), "Connected: %s Host: %s", networkSystem->isConnected() ? "yes": "no", networkSystem->isHost() ? "yes" : "no");
	renderer->drawText(buf, 0, 0, C2D_Color32(255, 255, 255, 255), 1);

}