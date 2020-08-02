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

#ifndef DEF_THE_EYE_H
#define DEF_THE_EYE_H

#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "GridNotifiers.h"

enum EyeData
{
    DATA_ALAR               = 0,
    DATA_ASTROMANCER        = 1,
    DATA_REAVER             = 2,
    DATA_KAELTHAS           = 3,
    MAX_ENCOUNTER           = 4,

    DATA_KAEL_ADVISOR1      = 10,
    DATA_KAEL_ADVISOR2      = 11,
    DATA_KAEL_ADVISOR3      = 12,
    DATA_KAEL_ADVISOR4      = 13
};

enum EyeNPCs
{
    NPC_ALAR                = 19514,
    NPC_KAELTHAS            = 19622,
    NPC_THALADRED           = 20064,
    NPC_LORD_SANGUINAR      = 20060,
    NPC_CAPERNIAN           = 20062,
    NPC_TELONICUS           = 20063
};

enum EyeGOs
{
    GO_BRIDGE_WINDOW        = 184069,
    GO_KAEL_STATUE_RIGHT    = 184596,
    GO_KAEL_STATUE_LEFT     = 184597
};

#endif
