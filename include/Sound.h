#ifndef SOUND_H
#define SOUND_H

#include "SDL_mixer.h"

#include <memory>
#include <string>

using std::string;
using std::shared_ptr;

class Sound{
private:
  shared_ptr<Mix_Chunk> sound;
  int channel;

public:
  Sound();
  Sound(string file);

  void play(int times);
  void stop();
  void open(string file);
  bool is_open();

};

#endif
