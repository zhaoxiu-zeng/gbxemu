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


#include "GBA.h"
#include "Flash.h"
#include "Sram.h"

u8 sramRead(u32 address)
{
  return flashSaveMemory[address & 0xFFFF];
}
void sramDelayedWrite(u32 address, u8 byte)
{
  saveType = 1;
  cpuSaveGameFunc = sramWrite;
  sramWrite(address, byte);
}

void sramWrite(u32 address, u8 byte)
{
  flashSaveMemory[address & 0xFFFF] = byte;
  saveDataChanged = true;
}
