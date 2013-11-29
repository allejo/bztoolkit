#include "bzfsAPI.h"
#include "bztoolkit.h"

#include <cmath>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include "../../src/bzfs/GameKeeper.h"
#include "../../src/bzfs/bzfs.h"
#include "../../src/bzfs/CmdLineOptions.h"

/*
    Private functions
*/
void fixTeamCount()
{
    int playerIndex, teamNum;

    for (teamNum = RogueTeam; teamNum < HunterTeam; teamNum++)
    {
        team[teamNum].team.size = 0;
    }

    for (playerIndex = 0; playerIndex < curMaxPlayers; playerIndex++)
    {
        GameKeeper::Player *p = GameKeeper::Player::getPlayerByIndex(playerIndex);

        if (p && p->player.isPlaying())
        {
            teamNum = p->player.getTeam();

            if (teamNum == HunterTeam)
                teamNum = RogueTeam;

            team[teamNum].team.size++;
        }
    }
}

void removePlayer(int playerIndex)
{
    GameKeeper::Player *playerData = GameKeeper::Player::getPlayerByIndex(playerIndex);

    if (!playerData)
        return;

    void *buf, *bufStart = getDirectMessageBuffer();
    buf = nboPackUByte(bufStart, playerIndex);

    broadcastMessage(MsgRemovePlayer, (char*)buf-(char*)bufStart, bufStart);

    int teamNum = int(playerData->player.getTeam());
    --team[teamNum].team.size;
    sendTeamUpdate(-1, teamNum);
    fixTeamCount();
}

void addPlayer(GameKeeper::Player *playerData, int index)
{
    void *bufStart = getDirectMessageBuffer();
    void *buf      = playerData->packPlayerUpdate(bufStart);

    broadcastMessage(MsgAddPlayer, (char*)buf - (char*)bufStart, bufStart);

    int teamNum = int(playerData->player.getTeam());
    team[teamNum].team.size++;
    sendTeamUpdate(-1, teamNum);
    fixTeamCount();
}

/*
    BZToolkit's API
*/

bool bztk_anyPlayers(bool observers = false)
{
    return (bool)(bz_getTeamCount(eRogueTeam) +
                  bz_getTeamCount(eRedTeam) +
                  bz_getTeamCount(eGreenTeam) +
                  bz_getTeamCount(eBlueTeam) +
                  bz_getTeamCount(ePurpleTeam) +
                  bz_getTeamCount(eRabbitTeam) +
                  bz_getTeamCount(eHunterTeam) +
                  (observers ? bz_getTeamCount(eObservers) : 0));
}

void bztk_foreachPlayer(void (*function)(int))
{
    bz_APIIntList *playerList = bz_newIntList();
    bz_getPlayerIndexList(playerList);

    for (unsigned int i = 0; i < playerList->size(); i++)
    {
        (*function)(playerList->get(i));
    }

    bz_deleteIntList(playerList);
}

bz_BasePlayerRecord* bztk_getPlayerByBZID(int BZID)
{
    bz_APIIntList *playerList = bz_newIntList();
    bz_getPlayerIndexList(playerList);

    for (unsigned int i = 0; i < playerList->size(); i++)
    {
        if (bz_getPlayerByIndex(playerList->get(i))->bzID == intToString(BZID))
        {
            bz_deleteIntList(playerList);
            return bz_getPlayerByIndex(playerList->get(i));
        }
    }

    bz_deleteIntList(playerList);
    return NULL;
}

bz_BasePlayerRecord* bztk_getPlayerByCallsign(const char* callsign)
{
    bz_APIIntList *playerList = bz_newIntList();
    bz_getPlayerIndexList(playerList);

    for (unsigned int i = 0; i < playerList->size(); i++)
    {
        if (bz_getPlayerByIndex(playerList->get(i))->callsign == callsign)
        {
            bz_deleteIntList(playerList);
            return bz_getPlayerByIndex(playerList->get(i));
        }
    }

    bz_deleteIntList(playerList);
    return NULL;
}

bool bztk_changeTeam(int playerID, bz_eTeamType team)
{
    GameKeeper::Player *playerData = GameKeeper::Player::getPlayerByIndex(playerID);

    if (!playerData)
    {
        return false;
    }

    removePlayer(playerID);

    if (team == eRogueTeam)
    {
        playerData->player.setTeam((TeamColor)RogueTeam);
    }

    if (team == eRedTeam)
    {
        playerData->player.setTeam((TeamColor)RedTeam);
    }

    if (team == eGreenTeam)
    {
        playerData->player.setTeam((TeamColor)GreenTeam);
    }

    if (team == eBlueTeam)
    {
        playerData->player.setTeam((TeamColor)BlueTeam);
    }

    if (team == ePurpleTeam)
    {
        playerData->player.setTeam((TeamColor)PurpleTeam);
    }

    if (team == eObservers)
    {
        playerData->player.setTeam((TeamColor)ObserverTeam);
    }

    addPlayer(playerData,playerID);
    sendPlayerInfo();
    sendIPUpdate(-1, playerID);

    return true;
}

bool bztk_isValidPlayerID(int playerID)
{
    bz_BasePlayerRecord *pr = bz_getPlayerByIndex(playerID);

    if (!pr)
    {
        return false;
    }

    bz_freePlayerRecord(pr);
    return true;
}

int bztk_randomPlayer(bz_eTeamType team = eNoTeam)
{
    srand(time(NULL));

    if (team == eNoTeam)
    {
        if (bz_anyPlayers())
        {
            bz_debugMessage(4,"There are players.");
            bz_APIIntList*playerlist=bz_getPlayerIndexList();
            int picked;
            picked=(*playerlist)[rand()%playerlist->size()];
            bz_deleteIntList(playerlist);

            return picked;
        }
        else
            return -1;
    }
    else
    {
        if (bz_getTeamCount(team) > 0)
        {
            int picked=0;
            bz_APIIntList* playerlist = bz_getPlayerIndexList();

            while (true)
            {
                picked = rand() % playerlist->size();
                if (bz_getPlayerTeam(picked) == team)
                    break;
            }

            bz_deleteIntList(playerlist);
            return picked;
        }
        else
            return -1;
    }
}