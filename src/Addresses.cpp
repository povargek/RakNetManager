#include "Main.h"

urmem::address_t
Addresses::RAKSERVER{},
Addresses::FUNC_RAKSERVER__SEND{},
Addresses::FUNC_RAKSERVER__RPC{},
Addresses::FUNC_RAKSERVER__RECEIVE{},
Addresses::FUNC_RAKSERVER__REGISTER_AS_REMOTE_PROCEDURE_CALL{},
Addresses::FUNC_RAKSERVER__DEALLOCATE_PACKET{},
Addresses::FUNC_RAKSERVER__GET_INDEX_FROM_PLAYER_ID{},
Addresses::FUNC_RAKSERVER__GET_PLAYER_ID_FROM_INDEX{};

bool Addresses::Init(urmem::address_t rakserver)
{
	if (auto vmt = urmem::pointer(RAKSERVER = rakserver).field<urmem::address_t *>(0))
	{
#ifdef _WIN32
		FUNC_RAKSERVER__SEND = vmt[7];
		FUNC_RAKSERVER__RPC = vmt[31];
		FUNC_RAKSERVER__RECEIVE = vmt[9];
		FUNC_RAKSERVER__REGISTER_AS_REMOTE_PROCEDURE_CALL = vmt[28];
		FUNC_RAKSERVER__DEALLOCATE_PACKET = vmt[11];
		FUNC_RAKSERVER__GET_INDEX_FROM_PLAYER_ID = vmt[55];
		FUNC_RAKSERVER__GET_PLAYER_ID_FROM_INDEX = vmt[56];
#else
		FUNC_RAKSERVER__SEND = vmt[8];
		FUNC_RAKSERVER__RPC = vmt[34];
		FUNC_RAKSERVER__RECEIVE = vmt[10];
		FUNC_RAKSERVER__REGISTER_AS_REMOTE_PROCEDURE_CALL = vmt[29];
		FUNC_RAKSERVER__DEALLOCATE_PACKET = vmt[12];
		FUNC_RAKSERVER__GET_INDEX_FROM_PLAYER_ID = vmt[56];
		FUNC_RAKSERVER__GET_PLAYER_ID_FROM_INDEX = vmt[57];
#endif
		return logprintf("[RNM] Addresses found"), true;
	}

	return logprintf("[RNM] Addresses not found"), false;
}