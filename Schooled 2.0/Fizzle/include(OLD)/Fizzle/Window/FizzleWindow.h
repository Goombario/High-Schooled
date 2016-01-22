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

#ifndef FIZZLE_WINDOW_H
#define FIZZLE_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file FizzleWindow.h
 * @brief Provides all functions necessary for interaction
 *	with the window Fizzle generates for you. 
 */

/**
 * Returns the status of a keyboard key *this frame*.
 * 1 represents down, 0 represents up.
 *
 * @param key A FzlKey enum value representing a particular key. Check FizzleDataTypes.h to see all key codes.
 * 
 * @return An integer representing the status of the keyboard key.
 */
int	FzlGetKey(enum FzlKey key);

/**	
 * Returns the status of a mouse key *this frame*. 
 * 1 represents down, 0 represents up.
 *
 * @param button A FzlMouseButton enum value representing a particular mouse button. Check FizzleDataTypes.h to see all mouse button codes.
 *
 * @return An integer representing the status of the keyboard key.
 */
int	FzlGetMouseButton(enum FzlMouseButton button);

/**
 * Retrieves the current mouse position in the screen.
 * NOTE: In Windows, the *TOP LEFT* of the screen is considered 0, 0. Therefore,
 * a cursor at the bottom of the window will return the height of the window in pixels.
 * 
 * @param x Pointer to a double variable to store the value of the mouse position in the X axis.
 * @param y Pointer to a double variable to store the value of the mouse position in the Y axis.
*/
void FzlGetMousePos(double *x, double *y);

/**
 * Returns the time in seconds since Fizzle started.
 *
 * @return A double representing the time in seconds since Fizzle started.
 *
 */
double FzlGetTimeSinceStart();

/**
 * Returns the time in seconds since the last frame.
 *
 * @return A double representing the time since in seconds since the last frame was rendered.
 */
double FzlGetDeltaTime();

/**
 * Sets the refresh rate of the window.
 *
 * @param frameRate The number of frames you want to display per second.
 */
void FzlSetFrameRate(int frameRate);

/**
 * Refreshes the screen and displays anything rendered this frame.
 * To display any rendered sprites, you *MUST* call this function.
 * Only call *ONCE* per frame.
 */
void FzlSwapBuffers();

#ifdef __cplusplus
}
#endif

#endif
