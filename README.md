# RakNet Manager
Plugin for SA:MP Server that allows you to work with RakNet in PAWN.

Адаптация плагина RakNet Manager для CRMP 0.3e

# Пример использования:
Этот пример ставит защиту от атаки флудом запроса информации о игроке (TAB), так же когда игрок вызовет окно просмотра таблицы игроков, будет вызвана OnPlayerTabUpdate
Фикс атаки "коннект в один слот", нововведение 0.3z-R4 с трейлерами (OnPlayerUpdateTrailer, тоже самое что и в 0.3z OnTrailerUpdate)
Так же пример античита на миниган, не кикая игрока
```pawn
new gLastTabUpdate[MAX_PLAYERS];

public OnPlayerConnect(playerid) {
	gLastTabUpdate[playerid] = 0;
	return true;
}

OnPlayerTabUpdate(playerid)
{
	printf("playerid %d press tab key!", playerid);
	return 1; // 1 - отправить игроку инфу о игроках, 0 - не отправлять
}

new RPC_ClientJoin = 81;
new RPC_UpdateScoresPingsIPs = 105;
public OnPlayerReceivedRPC(player_id, rpc_id, BitStream:bs)
{
    if(rpc_id == RPC_ClientJoin) 
    {
		if(IsPlayerConnected(player_id))
		{
			printf("Player ID [%d] attempt to connect to taken slot!", player_id);
			return 0;
		}
    }
	
	if(rpc_id == RPC_UpdateScoresPingsIPs) 
    {
		if(IsPlayerConnected(player_id))
		{
			if(gLastTabUpdate[player_id]+5000 > GetTickCount()) return 0; // не чаще раза в 5 секнуд, чтобы игрок не гадил серверу своими шайтан-программами
		
			if(!OnPlayerTabUpdate(player_id)) return 0; // отменяем по желанию в функции выше
		
			gLastTabUpdate[player_id] = GetTickCount();
			
			return 1;
		}
    }

    return 1;
} 

OnPlayerUpdateTrailer(playerid, trailerid)
{
	/*
	
	Пример:

	if(!gPlayerRentTruck[playerid]) return 0;
	
	Игрок угнал читами фуру, которую не арендовал, не даем читеру её синхронизировать
	
	*/
	
	
	return 1; // 1 - синхронизировать груз игрока, 0 - не синхронизировать
}

new ID_PLAYER_SYNC = 116;
new ID_TRAILER_SYNC = 114;
public OnPlayerReceivedPacket(player_id, packet_id, BitStream:bs)
{
	if(packet_id == ID_TRAILER_SYNC)
    {
		new trailerid;
		BS_IgnoreBits(bs, 8); // packet id
		BS_ReadValue(bs, RNM_UINT16, trailerid);
		
		if(GetPlayerState(player_id) != PLAYER_STATE_DRIVER) return 0;
		
		if(!OnPlayerUpdateTrailer(player_id, trailerid)) return 0; // отменяем по желанию в функции выше

		return 1;
	}

    if(packet_id == ID_PLAYER_SYNC)
    {
		new lrkeys, udkeys, sampkeys,
		Float:pos[3], Float:quaternion[4],
		health, armor, weaponid, specialaction,
		Float:speed[3], Float:surfingoffsets[3],
		surfingvehid, animationid, animflags;

		BS_IgnoreBits(bs, 8); // packet id
		BS_ReadValue(bs, RNM_UINT16, lrkeys,
						 RNM_UINT16, udkeys,
						 RNM_UINT16, sampkeys,
						 RNM_FLOAT, pos[0],
						 RNM_FLOAT, pos[1],
						 RNM_FLOAT, pos[2],
						 RNM_FLOAT, quaternion[0],
						 RNM_FLOAT, quaternion[1],
						 RNM_FLOAT, quaternion[2],
						 RNM_FLOAT, quaternion[3],
						 RNM_UINT8, health,
						 RNM_UINT8, armor,
						 RNM_UINT8, weaponid,
						 RNM_UINT8, specialaction,
						 RNM_FLOAT, speed[0],
						 RNM_FLOAT, speed[1],
						 RNM_FLOAT, speed[2],
						 RNM_FLOAT, surfingoffsets[0],
						 RNM_FLOAT, surfingoffsets[1],
						 RNM_FLOAT, surfingoffsets[2],
						 RNM_UINT16, surfingvehid,
						 RNM_INT16, animationid,
						 RNM_INT16, animflags);
						 
		if (weaponid == 38) weaponid = 0; 
		
		/*
		если игрок держит в руке миниган, просто не показоваем его другим игрокам,
		тем самым читер ничего плохого с помощью его не сделает
		*/
		
		BS_Reset(bs);		
		BS_WriteValue(bs, RNM_UINT8, packet_id,
						 RNM_UINT16, lrkeys,
						 RNM_UINT16, udkeys,
						 RNM_UINT16, sampkeys,
						 RNM_FLOAT, pos[0],
						 RNM_FLOAT, pos[1],
						 RNM_FLOAT, pos[2],
						 RNM_FLOAT, quaternion[0],
						 RNM_FLOAT, quaternion[1],
						 RNM_FLOAT, quaternion[2],
						 RNM_FLOAT, quaternion[3],
						 RNM_UINT8, health,
						 RNM_UINT8, armor,
						 RNM_UINT8, weaponid,
						 RNM_UINT8, specialaction,
						 RNM_FLOAT, speed[0],
						 RNM_FLOAT, speed[1],
						 RNM_FLOAT, speed[2],
						 RNM_FLOAT, surfingoffsets[0],
						 RNM_FLOAT, surfingoffsets[1],
						 RNM_FLOAT, surfingoffsets[2],
						 RNM_UINT16, surfingvehid,
						 RNM_INT16, animationid,
						 RNM_INT16, animflags);
    }
    return 1;
}
```

Автор плагина: urShadow
Подогнал под CRMP 0.3e: санек