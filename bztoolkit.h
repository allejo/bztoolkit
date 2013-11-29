#ifndef bztoolkit_h
#define bztoolkit_h

bool bztk_anyPlayers(bool observers = false);
bz_BasePlayerRecord* bztk_getPlayerByBZID(int BZID);
bz_BasePlayerRecord* bztk_getPlayerByCallsign(const char* callsign);
bool bztk_changeTeam(int playerID, bz_eTeamType team);
bool bztk_isValidPlayerID(int playerID);
int bztk_randomPlayer(bz_eTeamType team = eNoTeam);

#endif