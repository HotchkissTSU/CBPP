#ifndef CBASSET_SOUND
#define CBASSET_SOUND

#include "cbpp/collection.h"
#include "SDL2/SDL_mixer.h"
#include "cbasset/sfx_asm.h"

#include <string>

namespace cbasset{
	struct Sound{	
		std::string path;
		Mix_Music* sample;
		SFX::ProgramBlock* sfx_prog;
		
		int16_t loops = 1;
		int8_t volume = 100;
		
		int16_t loops_elapsed = 0;
		
		void SetSFX();
	};
	
	class SoundCollection : public cbpp::BaseCollection<std::string, Sound>{
		void LoadSound(std::string path);
	};
}

#endif