/*  VisualBoyAdvance 2
    Copyright (C) 2009-2010  VBA development team

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


#include "backupmedia.h"


BackupMedia::BackupMedia()
{
}


u32 stringToValue( const char s[5] ) {
  // maximum string length: 4 characters + zero byte
  u64 result = 0;
  for( int i = 0; i < 4; i++ ) {
    if( s[i] == 0 ) break;
    result |= ( s[i] << (i*8) );
  }
  return result;
}


BackupMedia::BACKUPMEDIATYPE BackupMedia::findBackupMediaType
    ( u32 *romData, u32 romSize )
{
  /* possible strings are:
     - "EEPROM_V"
     - "SRAM_V"
     - "SRAM_F_V"
     - "FLASH_V"
     - "FLASH512_V"
     - "FLASH1M_V"
  */

  // first part
  const u32 _EEPR = stringToValue( "EEPR" );
  const u32 _SRAM = stringToValue( "SRAM" );
  const u32 _FLAS = stringToValue( "FLAS" );

  // following parts
  const u32 _OM_V = stringToValue( "OM_V" );
  const u32 __Vxx = stringToValue(   "_V" );
  const u32 __F_V = stringToValue( "_F_V" );
  const u32 _H_Vx = stringToValue(  "H_V" );
  const u32 _H512 = stringToValue( "H512" );
  const u32 _H1M_ = stringToValue( "H1M_" );

  const u32 endAdress = ( romSize / 4 ) - 2;
  bool saveTypeFound = false;
  BACKUPMEDIATYPE result = NONE;

  for( u32 a/*ddress*/ = 0; a < endAdress; a++ ) {
    const u32 block = romData[a];

    if( block == _EEPR ) { // EEPROM
      if( romData[a+1] == _OM_V ) {
        result = EEPROM;
        saveTypeFound = true;
      }
    } else if( block == _SRAM ) { // SRAM
      const u32 next32 = romData[a+1];
      const u32 next16 = next32 & 0xFFFF;
      if( ( next32 == __F_V ) ||
          ( next16 == __Vxx ) ) {
        result = SRAM;
        saveTypeFound = true;
      }
    } else if( block == _FLAS ) { // FLASH
      const u32 next32 = romData[a+1];
      const u32 next24 = next32 & 0xFFFFFF;
      if( next32 == _H1M_ ) {
        result = FLASH128KiB;
        saveTypeFound = true;
      } else if( ( next32 == _H512 ) ||
                 ( next24 == _H_Vx ) ) {
        result = FLASH64KiB;
        saveTypeFound = true;
      }
    }
    if( saveTypeFound ) break; // finished
  }

  return result;
}