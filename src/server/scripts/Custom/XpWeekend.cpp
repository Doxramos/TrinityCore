//
// Created by Chubs on 11/11/2016.
//

#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include "World.h"
#include "/usr/include/boost/date_time.hpp"


class XpWeekend : public PlayerScript
{
public:
    XpWeekend() : PlayerScript("XpWeekend") { }
    void OnGiveXP(Player* player, uint32& amount, Unit* victim)override
    {
        boost::gregorian::date date(boost::gregorian::day_clock::local_day());
        auto day = date.day_of_week();
        if (day == boost::date_time::Friday ||
            day == boost::date_time::Saturday ||
            day == boost::date_time::Sunday) {

            amount = amount*2; }
    }
    void OnLogin(Player* player, bool firstLogin)
    {
        boost::gregorian::date date(boost::gregorian::day_clock::local_day());
        auto day = date.day_of_week();
        if (day == boost::date_time::Friday ||
            day == boost::date_time::Saturday ||
            day == boost::date_time::Sunday) {

            ChatHandler(player->GetSession()).PSendSysMessage("Double XP is going on now!");
        }
        else {
            //Make informational Script?
        }

    }

};

void AddSC_XpWeekend()
{
    new XpWeekend();
}