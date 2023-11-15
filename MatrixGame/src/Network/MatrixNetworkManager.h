#pragma once

#if defined(CLIENT_ON) && defined(SERVER_ON)
	#error CLIENT_ON and SERVER_ON variables cannot be defined at the same time
#endif

enum GAME_NETWORK_MODE {
	SINGLEPLAYER, // Original
	CLIENT, // Client for connecting to the server
	SERVER,
};