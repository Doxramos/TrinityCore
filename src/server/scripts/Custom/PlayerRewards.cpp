#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include "World.h"
#include "Config.h"
//#include <iostream>
//#include <ctime>
//#include <cstdlib>

#define MOUNT_ENABLED 1
#define HORDE_MOUNT 55531 //Mechano Hog
#define ALLIANCE_MOUNT 60424 //Mechageer
#define INITIAL_REWARD 10 //How many are granted on first login
#define STANDARD_REWARD 1 //How many are granted each interval
#define REWARD_ITEM 29434 //Badge of Justice
#define MINUTES_TO_AWARD 30 //How many minutes it takes to get the reward
//Don't Change After this unless you know what you're doing
#define MINUTES_AWARD MINUTES_TO_AWARD * 60
using namespace std;

void ConfigSetting(uint32 arg) {

}

class PlayerRewards : public PlayerScript {
public:
    PlayerRewards() : PlayerScript("PlayerRewards") {}

    int GetPlayTime(uint32 guid) {
        uint32 PlayedTime;
        QueryResult PlayTime = CharacterDatabase.PQuery("SELECT totaltime FROM characters WHERE guid = %u", guid);
        Field *Played = PlayTime->Fetch();
        PlayedTime = Played[0].GetUInt32();
        return PlayedTime;
    }

    int GetLastTime(uint32 guid) {
        uint32 LastRewarded;
        QueryResult LastReward = CharacterDatabase.PQuery("SELECT last_reward FROM player_rewards WHERE guid = %u",
                                                          guid);
        Field *Rewarded = LastReward->Fetch();
        LastRewarded = Rewarded[0].GetUInt32();
        return LastRewarded;
    }

    void CheckLastReward(Player *player, uint32 guid) {
        QueryResult LastReward = CharacterDatabase.PQuery("SELECT last_reward FROM player_rewards WHERE guid = %u",
                                                          guid);
        if (!LastReward) {
            CreateRewardData(player, guid);
        } else {
            CheckTimer(player, guid);
        }
    }

    void CheckTimer(Player *player, uint32 guid) {
        uint32 LastRewardMinutes, PlayTimeMinutes, ElapsedTime;
        LastRewardMinutes = GetLastTime(guid) / 60;
        PlayTimeMinutes = GetPlayTime(guid) / 60;
        ElapsedTime = PlayTimeMinutes - LastRewardMinutes;
        if (ElapsedTime >= MINUTES_TO_AWARD) {
            uint32 Grantable = PlayTimeMinutes - LastRewardMinutes;
            uint32 Granting = Grantable / MINUTES_TO_AWARD;
            uint32 NewRewardTime = PlayTimeMinutes * 60;
            GrantReward(player, guid, 2, Granting);
        } else {
            //Do Nothing
        }

    }

    void NewPlayerWelcome(Player *player, uint32 guid) {
        if (MOUNT_ENABLED == 1) {
            if (player->GetTeam() == 469) {
                player->LearnSpell(ALLIANCE_MOUNT, false);
            }
            if (player->GetTeam() == 67) {
                player->LearnSpell(HORDE_MOUNT, false);
            }
        }
        CreateRewardData(player, guid);
    }
    void CreateRewardData(Player *player, uint32 guid) {
        uint32 PlayedTime;
        PlayedTime = GetPlayTime(guid);
        CharacterDatabase.PQuery("INSERT INTO player_rewards(guid, last_reward) VALUES(%u, %u)", guid, PlayedTime);
        GrantReward(player, guid, 1, 0);
    }

    void GrantReward(Player *player, uint32 guid, uint32 type, uint32 INCOMING) {
        switch (type) {
            case 1:
                player->AddItem(REWARD_ITEM, INITIAL_REWARD);
                break;
            case 2:
                uint32 Rewarded = STANDARD_REWARD * INCOMING;
                player->AddItem(REWARD_ITEM, Rewarded);
                UpdateLastReward(guid);
                break;
        }
    }

    void UpdateLastReward(uint32 guid) {
        uint32 PlayedTime = GetPlayTime(guid);
        CharacterDatabase.PQuery("UPDATE player_rewards SET last_reward = %u WHERE guid = %u", PlayedTime, guid);
    }

    void OnLogin(Player *player, bool firstLogin) {
            uint32 guid = player->GetGUID().GetCounter();
            if (firstLogin) {
                NewPlayerWelcome(player, guid);
            } else {
                uint32 PlayedTime;
                PlayedTime = GetPlayTime(guid);
                CheckLastReward(player, guid);
            }
        }
};


void AddSC_PlayerRewards()
{
	new PlayerRewards();
}