#include <cmath>
#include <time.h>

#include "bzToolkit.h"

/*---------------------------------------------------------------------------*/

bool bztk_getPlayerCount(bool observers = false)
{
    return (bz_getTeamCount(eRogueTeam) +
            bz_getTeamCount(eRedTeam) +
            bz_getTeamCount(eGreenTeam) +
            bz_getTeamCount(eBlueTeam) +
            bz_getTeamCount(ePurpleTeam) +
            bz_getTeamCount(eRabbitTeam) +
            bz_getTeamCount(eHunterTeam) +
            (observers ? bz_getTeamCount(eObservers) : 0));
}

/*---------------------------------------------------------------------------*/

bool bztk_anyPlayers(bool observers = false)
{
    return (bool)(bztk_getPlayerCount(observers));
}

/*---------------------------------------------------------------------------*/

std::string bztk_eTeamTypeLiteral(bz_eTeamType team)
{
    switch (team)
    {
        case eNoTeam:
            return "No";

        case eRogueTeam:
            return "Rogue";

        case eRedTeam:
            return "Red";

        case eGreenTeam:
            return "Green";

        case eBlueTeam:
            return "Blue";

        case ePurpleTeam:
            return "Purple";

        case eRabbitTeam:
            return "Rabbit";

        case eHunterTeam:
            return "Hunter";

        case eObservers:
            return "Observer";

        case eAdministrators:
            return "Administrator";

        default:
            return "No";
    }
}

/*---------------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------------*/

bool bztk_changeTeam(int playerID, bz_eTeamType team)
{
    GameKeeper::Player *playerData = GameKeeper::Player::getPlayerByIndex(playerID);

    if (!playerData)
    {
        bz_debugMessagef(2, "bzToolkit -> bztk_changeTeam() :: Player ID %i not found.", playerID);
        return false;
    }
    else if ((team != eRogueTeam) ||
             (team != eRedTeam) ||
             (team != eGreenTeam) ||
             (team != eBlueTeam) ||
             (team != ePurpleTeam) ||
             (team != eObservers))
    {
        bz_debugMessagef(2, "bzToolkit -> bztk_changeTeam() :: Warning! Players cannot be swapped to the %s team through this function.", bztk_eTeamTypeLiteral(team).c_str());
        return false;
    }

    removePlayer(playerID);

    switch (team)
    {
        case eRogueTeam:
            playerData->player.setTeam((TeamColor)RogueTeam);
            break;

        case eRedTeam:
            playerData->player.setTeam((TeamColor)RedTeam);
            break;

        case eGreenTeam:
            playerData->player.setTeam((TeamColor)GreenTeam);
            break;

        case eBlueTeam:
            playerData->player.setTeam((TeamColor)BlueTeam);
            break;

        case ePurpleTeam:
            playerData->player.setTeam((TeamColor)PurpleTeam);
            break;

        case eObservers:
            playerData->player.setTeam((TeamColor)ObserverTeam);
            break;

        default: // Should never happen
            break;
    }

    addPlayer(playerData,playerID);
    sendPlayerInfo();
    sendIPUpdate(-1, playerID);

    return true;
}

/*---------------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------------*/

int bztk_randomPlayer(bz_eTeamType team = eNoTeam)
{
    srand(time(NULL));

    if (team == eNoTeam)
    {
        if (bztk_anyPlayers())
        {
            bz_APIIntList *playerlist = bz_getPlayerIndexList();
            int picked = (*playerlist)[rand()%playerlist->size()];
            bz_deleteIntList(playerlist);

            return picked;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        if (bz_getTeamCount(team) > 0)
        {
            int picked = 0;
            bz_APIIntList* playerlist = bz_getPlayerIndexList();

            while (true)
            {
                picked = rand() % playerlist->size();

                if (bz_getPlayerTeam(picked) == team)
                {
                    break;
                }
            }

            bz_deleteIntList(playerlist);

            return picked;
        }
        else
        {
            return -1;
        }
    }
}

/*---------------------------------------------------------------------------*/

void bztk_registerCustomBoolBZDB(const char* bzdbVar, bool value, int perms = 0, bool persistent = false)
{
    if (!bz_BZDBItemExists(bzdbVar))
    {
        bz_setBZDBBool(bzdbVar, value, perms, persistent);
    }
}

/*---------------------------------------------------------------------------*/

void bztk_registerCustomDoubleBZDB(const char* bzdbVar, double value, int perms = 0, bool persistent = false)
{
    if (!bz_BZDBItemExists(bzdbVar))
    {
        bz_setBZDBDouble(bzdbVar, value, perms, persistent);
    }
}

/*---------------------------------------------------------------------------*/

void bztk_registerCustomIntBZDB(const char* bzdbVar, int value, int perms = 0, bool persistent = false)
{
    if (!bz_BZDBItemExists(bzdbVar))
    {
        bz_setBZDBInt(bzdbVar, value, perms, persistent);
    }
}

/*---------------------------------------------------------------------------*/

void bztk_registerCustomStringBZDB(const char* bzdbVar, const char* value, int perms = 0, bool persistent = false)
{
    if (!bz_BZDBItemExists(bzdbVar))
    {
        bz_setBZDBString(bzdbVar, value, perms, persistent);
    }
}