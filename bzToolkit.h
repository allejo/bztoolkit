/*
bzToolkit
    Copyright (C) 2013 Vladimir Jimenez

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

#include <sstream>

#include "../../../src/bzfs/GameKeeper.h"
#include "../../../src/bzfs/bzfs.h"
#include "../../../src/bzfs/CmdLineOptions.h"

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

void forcePlayerSpawn(int playerID)
{
    playerAlive(playerID);
}

std::string intToString(int number)
{
    std::stringstream string;
    string << number;

    return string.str();
}
