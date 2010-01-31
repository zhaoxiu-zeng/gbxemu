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


#ifndef SOUND_OUTPUT_QT_H
#define SOUND_OUTPUT_QT_H


#include "../gba2/common/cdriver_sound.h"


class sound_output_qt : public CDriver_Sound
{
public:
  sound_output_qt();
  ~sound_output_qt();
  bool init( long sampleRate );
  void pause();
  void reset();
  void resume();
  void write( u16 * finalWave, int length );
  void setThrottle( unsigned short throttle );
};


#endif // SOUND_OUTPUT_QT_H
