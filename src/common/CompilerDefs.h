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

#ifndef ACORE_COMPILERDEFS_H
#define ACORE_COMPILERDEFS_H

#define WH_PLATFORM_WINDOWS 0
#define WH_PLATFORM_UNIX    1
#define WH_PLATFORM_APPLE   2
#define WH_PLATFORM_INTEL   3

// must be first (win 64 also define _WIN32)
#if defined( _WIN64 )
#define WH_PLATFORM WH_PLATFORM_WINDOWS
#elif defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#define WH_PLATFORM WH_PLATFORM_WINDOWS
#elif defined( __APPLE_CC__ )
#define WH_PLATFORM WH_PLATFORM_APPLE
#elif defined( __INTEL_COMPILER )
#define WH_PLATFORM WH_PLATFORM_INTEL
#else
#define WH_PLATFORM WH_PLATFORM_UNIX
#endif

#define WH_COMPILER_MICROSOFT 0
#define WH_COMPILER_GNU       1
#define WH_COMPILER_BORLAND   2
#define WH_COMPILER_INTEL     3

#ifdef _MSC_VER
#define WH_COMPILER WH_COMPILER_MICROSOFT
#elif defined( __BORLANDC__ )
#define WH_COMPILER WH_COMPILER_BORLAND
#elif defined( __INTEL_COMPILER )
#define WH_COMPILER WH_COMPILER_INTEL
#elif defined( __GNUC__ )
#define WH_COMPILER WH_COMPILER_GNU
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#  error "FATAL ERROR: Unknown compiler."
#endif

#endif
