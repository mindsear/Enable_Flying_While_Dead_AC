#include "Player.h"
#include "ScriptMgr.h"
#include "Config.h"
#include "World.h"
#include "Chat.h"

#define DeadlyGladiatorsFrostWyrm_ModelID1 25511 // http://wotlk.cavernoftime.com/npc=34225
#define DEFAULT_RUNNING_SPEED 1.0f
#define GROUND_SPEED_100percent 2.0f
#define FLYING_SPEED_310percent 4.1f
#define RemoveMount NULL

class enable_flying_while_dead : public PlayerScript
{
public:
    enable_flying_while_dead() : PlayerScript("enable_flying_while_dead") {}

    void LoadConfig()
    {
        config_EnableFlyingWhileDead = sConfigMgr->GetBoolDefault("FlyingWhileDead.Enable", true);
        config_Alliance_MountModel = sConfigMgr->GetIntDefault("FlyingWhileDead.Alliance.MountModelid", DeadlyGladiatorsFrostWyrm_ModelID1);
        config_Horde_MountModel = sConfigMgr->GetIntDefault("FlyingWhileDead.Horde.MountModelid", DeadlyGladiatorsFrostWyrm_ModelID1);
        config_DeadMountGroundSpeed = sConfigMgr->GetFloatDefault("FlyingWhileDead.DeadMountGroundSpeed", GROUND_SPEED_100percent);
        config_DeadMountFlyingSpeed = sConfigMgr->GetFloatDefault("FlyingWhileDead.DeadMountFlyingSpeed", FLYING_SPEED_310percent);
        config_EnableFlight = sConfigMgr->GetBoolDefault("FlyingWhileDead.EnableDeadMountFlight", true);
        config_EnableFlyingWhileDead_InBattleground = sConfigMgr->GetBoolDefault("FlyingWhileDead.EnableInBattleground", true);
        config_EnableFlyingWhileDead_InArena = sConfigMgr->GetBoolDefault("FlyingWhileDead.EnableInArena", true);
        config_MinLevel = sConfigMgr->GetIntDefault("FlyingWhileDead.PlayerMinLevel", 1);
        Disable_InAreas(sConfigMgr->GetStringDefault("FlyingWhileDead.Disable.InAreas", ""));
    }

    void EnableFlyingWhileDeadFor(Player* player)
    {
        LoadConfig();

        if (config_EnableFlyingWhileDead)
        {
            Tokenizer areas(_Disable_InAreas, ',');
            for (uint8 i = 0; i < areas.size(); )
            {
                if (player->GetAreaId() == (atoi(areas[i++])))
                {
                    // Print message to see if it's working
                    //ChatHandler(player->GetSession()).PSendSysMessage("FlyingWhileDead: Disabled in area id: %i", player->GetAreaId());
                    //printf("> FlyingWhileDead: Disabled in area id: %i", player->GetAreaId());
                    return;
                }
            }

            if (config_EnableFlyingWhileDead_InBattleground == false && player->InBattleground())
                return; 

            if (config_EnableFlyingWhileDead_InArena == false && player->InArena())
                return;

            if (player->getLevel() >= config_MinLevel)
            {
                if (player->GetTeamId() == TEAM_ALLIANCE)
                    player->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, config_Alliance_MountModel);
                else // Horde Mount
                    player->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, config_Horde_MountModel);
                player->SetCanFly(config_EnableFlight);
                player->SetSpeed(MOVE_FLIGHT, config_DeadMountFlyingSpeed, true);
                player->SetSpeed(MOVE_RUN, config_DeadMountGroundSpeed, true);
            }
        }
    }

    void DisableFlyingUponResurrectionFor(Player* player)
    {
        player->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, RemoveMount);
        player->SetCanFly(false);
        player->SetSpeed(MOVE_RUN, DEFAULT_RUNNING_SPEED, true);
    }

    void OnPlayerReleasedGhost(Player* player)
    {
        EnableFlyingWhileDeadFor(player);
    }

    // Called for Player::ResurrectPlayer
    virtual void OnBeforeResurrectPlayer(Player* player, float /*restore_percent*/, bool /*applySickness*/)
    {
        DisableFlyingUponResurrectionFor(player);
    }

private:

    bool config_EnableFlyingWhileDead;
    uint32 config_Alliance_MountModel;
    uint32 config_Horde_MountModel;
    float config_DeadMountGroundSpeed;
    float config_DeadMountFlyingSpeed;
    bool config_EnableFlight;
    bool config_EnableFlyingWhileDead_InBattleground;
    bool config_EnableFlyingWhileDead_InArena;
    uint8 config_MinLevel;

    std::string _Disable_InAreas;
    void Disable_InAreas(const std::string& disable_InAreas) { _Disable_InAreas = disable_InAreas; }

};

class enable_flying_while_dead_World : public WorldScript
{
public:
    enable_flying_while_dead_World() : WorldScript("enable_flying_while_dead_World") { }

    void OnStartup() override
    {
        // Show this On worldserver startup
        sLog->outString("");
        sLog->outString("=========================================");
        sLog->outString("== Loaded mod enable_flying_while_dead ==");
        sLog->outString("=========================================");
        sLog->outString("");
    }

    void OnAfterConfigLoad(bool reload) override
    {
        if (reload)
        {
            // Show this if ".reload config" command is used
            sLog->outString("");
            sLog->outString("============================================");
            sLog->outString("== Re-Loaded mod enable_flying_while_dead ==");
            sLog->outString("============================================");
            sLog->outString("");
        }
    }
};

void AddSC_enable_flying_while_dead()
{
    new enable_flying_while_dead();
    new enable_flying_while_dead_World();
}

