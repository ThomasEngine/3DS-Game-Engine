#pragma once
#include <3ds.h>


namespace network {
public:
	void networkInit();
	void networkShutdown();
	bool hostGame();
	bool findAndJoinGame();
	void update();

	bool isConnected() const;
	bool isHost() const;


private:
	bool connected;
	bool isHostFlag;
	udsBindContext bindctx;
	u32 tick = 0;
	u8 recvbuf[UDS_DATAFRAME_MAXSIZE];
};