/*
 * This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Log.h"
#include "DatabaseEnv.h"
#include "Util.h"
#include "WardenCheckMgr.h"
#include "Warden.h"
#include "GameConfig.h"

WardenCheckMgr::WardenCheckMgr()
{}

WardenCheckMgr* WardenCheckMgr::instance()
{
    static WardenCheckMgr instance;
    return &instance;
}

void WardenCheckMgr::LoadWardenChecks()
{
    // Check if Warden is enabled by config before loading anything
    if (!sGameConfig->GetBoolConfig("Warden.Enabled"))
    {
        LOG_INFO("server", ">> Warden disabled, loading checks skipped.");
        LOG_INFO("server", "");
        return;
    }

    QueryResult result = WorldDatabase.Query("SELECT MAX(id) FROM warden_checks");

    if (!result)
    {
        LOG_INFO("server", ">> Loaded 0 Warden checks. DB table `warden_checks` is empty!");
        LOG_INFO("server", "");
        return;
    }

    Field* fields = result->Fetch();

    uint16 maxCheckId = fields[0].GetUInt16();

    CheckStore.resize(maxCheckId + 1);

    //                                    0    1     2     3        4       5      6      7
    result = WorldDatabase.Query("SELECT id, type, data, result, address, length, str, comment FROM warden_checks ORDER BY id ASC");

    uint32 count = 0;
    do
    {
        fields = result->Fetch();

        uint16 const id  = fields[0].GetUInt16();
        uint8 const type = fields[1].GetUInt8();

        wardenCheck.CheckId = id;
        wardenCheck.Type = WardenCheckType(type);

        // Initialize action with default action from config
        wardenCheck.Action = WardenActions(sGameConfig->GetIntConfig("Warden.ClientCheckFailAction"));

        if (type == PAGE_CHECK_A || type == PAGE_CHECK_B || type == DRIVER_CHECK)
            wardenCheck.Data = fields[2].GetBinary();

        if (type == MPQ_CHECK || type == MEM_CHECK)
            CheckResultStore.emplace(id, fields[3].GetBinary());

        if (type == MEM_CHECK || type == PAGE_CHECK_A || type == PAGE_CHECK_B || type == PROC_CHECK)
        {
            wardenCheck.Address = fields[4].GetUInt32();
            wardenCheck.Length = fields[5].GetUInt8();
        }

        // PROC_CHECK support missing
        if (type == MEM_CHECK || type == MPQ_CHECK || type == LUA_STR_CHECK || type == DRIVER_CHECK || type == MODULE_CHECK)
            wardenCheck.Str = fields[6].GetString();

        CheckStore[id] = wardenCheck;

        wardenCheck.Comment = fields[7].GetString();
        if (wardenCheck.Comment.empty())
            wardenCheck.Comment = "Undocumented Check";

        if (type == MEM_CHECK || type == MODULE_CHECK)
            MemChecksIdPool.push_back(id);
        else
            OtherChecksIdPool.push_back(id);

        ++count;
    }
    while (result->NextRow());

    LOG_INFO("server", ">> Loaded %u warden checks.", count);
    LOG_INFO("server", "");
}

void WardenCheckMgr::LoadWardenOverrides()
{
    // Check if Warden is enabled by config before loading anything
    if (!sGameConfig->GetBoolConfig("Warden.Enabled"))
    {
        LOG_INFO("server", ">> Warden disabled, loading check overrides skipped.");
        LOG_INFO("server", "");
        return;
    }

    //                                                      0        1
    QueryResult result = CharacterDatabase.Query("SELECT wardenId, action FROM warden_action");

    if (!result)
    {
        LOG_INFO("server", ">> Loaded 0 Warden action overrides. DB table `warden_action` is empty!");
        LOG_INFO("server", "");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint16 checkId = fields[0].GetUInt16();
        uint8  action  = fields[1].GetUInt8();

        // Check if action value is in range (0-2, see WardenActions enum)
        if (action > WARDEN_ACTION_BAN)
            LOG_ERROR("server", "Warden check override action out of range (ID: %u, action: %u)", checkId, action);
        // Check if check actually exists before accessing the CheckStore vector
        else if (checkId > CheckStore.size())
            LOG_ERROR("server", "Warden check action override for non-existing check (ID: %u, action: %u), skipped", checkId, action);
        else
        {
            CheckStore[checkId].Action = WardenActions(action);
            ++count;
        }
    }
    while (result->NextRow());

    LOG_INFO("server", ">> Loaded %u warden action overrides.", count);
    LOG_INFO("server", "");
}

WardenCheck const& WardenCheckMgr::GetCheckDataById(uint16 Id) const
{
    ASSERT(Id < CheckStore.size(), "Requested Warden data for invalid check ID %u", uint32(Id));
    return CheckStore[Id];
}

WardenCheckResult const& WardenCheckMgr::GetCheckResultById(uint16 Id) const
{
    auto it = CheckResultStore.find(Id);
    ASSERT(it != CheckResultStore.end(), "Requested Warden result for invalid check ID %u", uint32(Id));
    return it->second;
}
