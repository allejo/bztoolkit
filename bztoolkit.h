#include <sstream>

#include "../../src/bzfs/GameKeeper.h"
#include "../../src/bzfs/bzfs.h"
#include "../../src/bzfs/CmdLineOptions.h"

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

std::string intToString(int number)
{
    std::stringstream string;
    string << number;

    return string.str();
}