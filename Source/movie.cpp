/**
 * @file movie.cpp
 *
 * Implementation of video playback.
 */

#include "diablo.h"
#include "effects.h"
#include "storm/storm_svid.h"
#include "utils/display.h"

#ifndef NOSOUND
#include "sound.h"
#endif

namespace devilution {

/** Should the movie continue playing. */
bool movie_playing;
/** Should the movie play in a loop. */
bool loop_movie;

/**
 * @brief Start playback of a given video.
 * @param pszMovie The file name of the video
 * @param user_can_close Set to false to make the video unskippable.
 */
void play_movie(const char *pszMovie, bool user_can_close)
{
	HANDLE video_stream;

	movie_playing = true;

#ifndef NOSOUND
	sound_disable_music(true);
	stream_stop();
	effects_play_sound("Sfx\\Misc\\blank.wav");
#endif

	if (SVidPlayBegin(pszMovie, loop_movie ? 0x100C0808 : 0x10280808, &video_stream)) {
		tagMSG Msg;
		while (movie_playing) {
			while (movie_playing && FetchMessage(&Msg)) {
				switch (Msg.message) {
				case DVL_WM_KEYDOWN:
				case DVL_WM_LBUTTONDOWN:
				case DVL_WM_RBUTTONDOWN:
					if (user_can_close || (Msg.message == DVL_WM_KEYDOWN && Msg.wParam == DVL_VK_ESCAPE))
						movie_playing = false;
					break;
				case DVL_WM_QUIT:
					SVidPlayEnd(video_stream);
					diablo_quit(0);
					break;
				}
			}
			if (!SVidPlayContinue())
				break;
		}
		SVidPlayEnd(video_stream);
	}

#ifndef NOSOUND
	sound_disable_music(false);
#endif

	movie_playing = false;
	SDL_GetMouseState(&MouseX, &MouseY);
	OutputToLogical(&MouseX, &MouseY);
}

} // namespace devilution
