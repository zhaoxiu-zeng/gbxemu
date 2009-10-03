/*  VisualBoyAdvance 2
    Copyright (C) 2009  VBA development team

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


#include "cemugba.h"

#include "gba/GBA.h"
#include "gba/Sound.h"
#include <assert.h>

CEmuGBA::CEmuGBA()
{
    m_romLoaded = false;
    m_soundInitialized = false;
    m_soundDriverLoaded = false;
    m_graphicsDriverLoaded = false;
    m_snd = NULL;
    m_gfx = NULL;
}

CEmuGBA::~CEmuGBA()
{
}

bool CEmuGBA::loadROM( const u8 *const romData, const u32 romSize )
{
    if( !m_soundDriverLoaded ) return false;

    CPULoadRom( romData, romSize );
    if( m_soundInitialized ) {
        soundReset();
    } else {
        soundDriver = m_snd;
        soundInit();
        m_soundInitialized = true;
    }
    CPUInit();
    CPUReset();
    m_romLoaded = true;
    return true;
}

bool CEmuGBA::closeROM()
{
    if( m_romLoaded ) {
        CPUCleanUp();
        m_romLoaded = false;
        return true;
    }

    return false;
}

bool CEmuGBA::emulate()
{
    if( !m_romLoaded ) return false;

    CPULoop( cyclesPerFrame );
    return true;
}

bool CEmuGBA::setDriverSound( CDriver_Sound *drv )
{
    if( drv == NULL ) return false;
    m_snd = drv;
    m_soundDriverLoaded = true;
    return true;
}

bool CEmuGBA::setDriverGraphics( CDriver_Graphics *drv )
{
    if( drv == NULL ) return false;
    m_gfx = drv;
    m_graphicsDriverLoaded = true;
    return true;
}
