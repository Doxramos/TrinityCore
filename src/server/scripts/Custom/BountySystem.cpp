//
// Created by Chubs on 11/12/2016.
//
#include "Chat.h"
#include "Config.h"
using namespace std;
class BountySystem : public PlayerScript {
public:
    BountySystem() : PlayerScript("BountySystem") {}
    /*
    int LevelDiff(Player * Killer, Player * Killed) {
        uint32 KillerLevel, KilledLevel, Diff;
        KillerLevel = Killer->getLevel();
        KilledLevel = Killed->getLevel();
        if(KillerLevel > KilledLevel) {
            return KillerLevel - KilledLevel;
        }
        if(KilledLevel > KillerLevel) {
            return KilledLevel - KillerLevel;
        }
        if(KilledLevel == KillerLevel) {
            return 1;
        }
    }
    int BountyType(Player * Killer, Player * Killed) {
        uint32 KillerLevel, KilledLevel,Diff, KillType;
        KillerLevel = Killer->getLevel();
        KilledLevel = Killed->getLevel();
        if(KillerLevel > KilledLevel) {
            Diff = KillerLevel - KilledLevel;
            if(Diff >= 2 && Diff <= 5) {
                return Diff*3;
            }
        }
        if(KillerLevel < KilledLevel) {
            Diff = KilledLevel - KillerLevel;
            if(Diff >=5) {
                return Diff*4;
            }
        }
        if(KillerLevel == KilledLevel) {
            Diff = 1;
            return Diff;
        }
    }
    int CheckExisting(uint32 guid) {
        QueryResult result = WorldDatabase.PQuery("SELECT amount FROM active_bounties WHERE guid = %u", guid);
        if(!result) {
            return 1;
        } else {
            return 0;
        }
    }
    void CreateBounty(uint32 guid, Player * Killer, Player * Killed) {
        if(CheckExisting(guid) == 1) {
            CreateNewBounty(guid, Killer, Killed);
        } else {
            //UpdateBounty(guid, Killer, Killed);
        };
    }
    void CreateNewBounty(uint32 guid, Player * Killer, Player * Killed) {
        //10000 Per Gold, Return*10000
        uint32 GrantGold, BountyGold, KilledGUID;
        KilledGUID = Killed->GetGUID().GetCounter();

        GrantGold = LevelDiff(Killer, Killed)*10000;
        SendReward(Killer, GrantGold);
        ChatHandler(Killer->GetSession()).PSendSysMessage("Check for bounty returns %u", CheckForBounty(KilledGUID));
        ChatHandler(Killer->GetSession()).PSendSysMessage("Create and send");
    }
    int CheckForBounty(uint32 guid) {
        QueryResult result;
        result = WorldDatabase.PQuery("SELECT amount FROM active_bounties WHERE guid = %u", guid);
        if(!result) {
            return 0;
        } else {
            Field *BountyAmount = result->Fetch();
            uint32 Granting = BountyAmount[0].GetUInt32();
            return Granting;
        }
    }
    void SendReward(Player * Killer, uint32 Amount) {
        Killer->ModifyMoney(Amount);
    }
     */
    int LevelDiff(uint32 KillerLevel, uint32 KilledLevel, uint32 Winner) {
        switch(Winner) {
            case 0: return KillerLevel - KilledLevel; break;
            case 1: return KilledLevel - KillerLevel; break;
        }
    }
    int IsHonorable(Player * Killer, Player * Killed) {
        uint32 KillerLevel, KilledLevel,LevelDifference, Honorable, Allowed;
        KillerLevel = Killer->getLevel();
        KilledLevel = Killed->getLevel();
        if(KilledLevel >= KillerLevel) { Honorable = 1; }
        if(KillerLevel < KilledLevel) {
            LevelDifference = LevelDiff(KillerLevel, KilledLevel, 0);
            if (LevelDifference > 4) {
                Honorable = 1;
            } else {
                Honorable = 0;
            }
        }
        return Honorable;
    }
    void OnPVPKill(Player * Killer, Player * Killed) {
        uint32 KillerGUID, KilledGUID,LevelDifference, KillerLevel, KilledLevel;
        KillerLevel = Killer->getLevel();
        KilledLevel = Killed->getLevel();
        std::ostringstream LowHigh;
        if(KillerLevel >= KilledLevel) {
            LevelDifference = KillerLevel - KilledLevel;
            ChatHandler(Killer->GetSession()).PSendSysMessage("Run Honorable");
        }
        else {
            LevelDifference = KilledLevel - KillerLevel;
            ChatHandler(Killer->GetSession()).PSendSysMessage("Check Honorable");
        }

       // ChatHandler(Killer->GetSession()).PSendSysMessage("Killed %s. %u Levels %s",Killed->GetName(), LevelDifference, lh.str().c_str());

    }
};



void AddSC_BountySystem()
{
    new BountySystem();
}


