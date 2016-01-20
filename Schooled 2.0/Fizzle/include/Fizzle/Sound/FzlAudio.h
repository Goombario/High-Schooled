// The MIT License (MIT)
// 
// Copyright (c) 2016 Anastasios Stamadianos
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file FzlAudio.h
 * @brief Provides a simple interface for sound loading and playback.
 */

/**
 * Loads a WAV sound from a file.
 *
 * @param file The file to load the sound from.
 * @param isLooping Should this sound loop when played?
 * 
 * @return A \p FzlSoundHandle to the sound.
 */
FzlSoundHandle FzlLoadSound(const char* file, int isLooping);

/**
 * Deletes a sound at a given handle.
 *
 * @param handle The handle of the sound to delete.
 */
void FzlDeleteSound(FzlSoundHandle handle);

/**
 * Plays a sound at a given handle.
 * 
 * @param handle The handle of the sound to play.
 */
void FzlPlaySound(FzlSoundHandle handle);

/**
 * Pauses a sound at a given handle.
 * 
 * @param handle The handle of the sound to pause.
 */
void FzlPauseSound(FzlSoundHandle handle);

/**
 * Stops a sound at a given handle.
 *
 * @param handle The handle of the sound to stop.
 */
void FzlStopSound(FzlSoundHandle handle);

#ifdef __cplusplus
}
#endif