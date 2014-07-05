/*
bzToolkit
    Copyright (C) 2013 - 2014 Vladimir "allejo" Jimenez

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cmath>
#include <time.h>

#include "bzToolkit.h"

/*---------------------------------------------------------------------------*/

bool bztk_isTeamFlag (std::string flagAbbr)
{
    return (flagAbbr == "R*" || flagAbbr == "G*" || flagAbbr == "B*" || flagAbbr == "P*");
}

/*---------------------------------------------------------------------------*/

bz_eTeamType bztk_getTeamFromFlag(std::string flagAbbr)
{
    if (bztk_isTeamFlag(flagAbbr))
    {
        if      (flagAbbr == "R*") return eRedTeam;
        else if (flagAbbr == "G*") return eGreenTeam;
        else if (flagAbbr == "B*") return eBlueTeam;
        else if (flagAbbr == "P*") return ePurpleTeam;
    }

    return eNoTeam;
}

/*---------------------------------------------------------------------------*/

void bztk_killAll(bz_eTeamType team = eNoTeam, bool spawnOnBase = false, int killerID = -1, std::string flagID = NULL)
{
    // Create a list of players
    std::unique_ptr<bz_APIIntList> playerList(bz_getPlayerIndexList());

    // Be sure the playerlist exists
    if (playerList)
    {
        // Loop through all of the players' callsigns
        for (unsigned int i = 0; i < playerList->size(); i++)
        {
            // If the team isn't specified, then kill all of the players
            if (team == eNoTeam)
            {
                bz_killPlayer(playerList->get(i), spawnOnBase, killerID, flagID.c_str());
            }
            // Kill only the players belonging to the specified team
            else if (bz_getPlayerTeam(playerList->get(i)) == team)
            {
                bz_killPlayer(playerList->get(i), spawnOnBase, killerID, flagID.c_str());
            }
        }
    }
}

/*---------------------------------------------------------------------------*/

int bztk_getPlayerCount(bool observers = false)
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

bz_eTeamType bztk_eTeamType(std::string teamColor)
{
    teamColor = bz_tolower(teamColor.c_str());

    if (teamColor == "rogue")
    {
        return eRogueTeam;
    }
    else if (teamColor == "red")
    {
        return eRedTeam;
    }
    else if (teamColor == "green")
    {
        return eGreenTeam;
    }
    else if (teamColor == "blue")
    {
        return eBlueTeam;
    }
    else if (teamColor == "purple")
    {
        return ePurpleTeam;
    }
    else if (teamColor == "rabbit")
    {
        return eRabbitTeam;
    }
    else if (teamColor == "hunter")
    {
        return eHunterTeam;
    }
    else if (teamColor == "observer")
    {
        return eObservers;
    }
    else if (teamColor == "administrator")
    {
        return eAdministrators;
    }
    else
    {
        return eNoTeam;
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
            int playerID = playerList->get(i);
            bz_deleteIntList(playerList);

            return bz_getPlayerByIndex(playerID);
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
            int playerID = playerList->get(i);
            bz_deleteIntList(playerList);

            return bz_getPlayerByIndex(playerID);
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
        bz_debugMessagef(2, "bzToolkit -> bztk_changeTeam() :: Player ID %d not found.", playerID);
        return false;
    }
    else if ((team != eRogueTeam)  && (team != eRedTeam)  &&
             (team != eGreenTeam)  && (team != eBlueTeam) &&
             (team != ePurpleTeam) && (team != eObservers))
    {
        bz_debugMessagef(2, "bzToolkit -> bztk_changeTeam() :: Warning! Players cannot be swapped to the %s team through this function.", bztk_eTeamTypeLiteral(team).c_str());
        return false;
    }
    else if (bz_getTeamPlayerLimit(team) <= 0)
    {
        bz_debugMessagef(2, "bzToolkit -> bztk_changeTeam() :: Warning! The %s team does not exist on this server.");
        return false;
    }

    // If the player is being moved to the observer team, we need to kill them so they can't pause/shoot while in observer
    if (team == eObservers)
    {
        bz_killPlayer(playerID, false);
        playerData->player.setDead();
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

bz_APIIntList* bztk_getTeamPlayerIndexList(bz_eTeamType team)
{
    bz_APIIntList* playerlist = bz_getPlayerIndexList();
    bz_APIIntList* resp = bz_newIntList();

    for (unsigned int i = 0; i < playerlist->size(); i++)
    {
        if (bz_getPlayerTeam(playerlist->get(i)) == team)
        {
          resp->push_back(playerlist->get(i));
        }
    }

    bz_deleteIntList(playerlist);
    return resp;
}

/*-------------------------------------------------------------------------*-*/

bool bztk_isValidPlayerID(int playerID)
{
    // Use another smart pointer so we don't forget about freeing up memory
    std::unique_ptr<bz_BasePlayerRecord> playerData(bz_getPlayerByIndex(playerID));

    // If the pointer doesn't exist, that means the playerID does not exist
    return (playerData) ? true : false;
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
