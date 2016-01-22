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

#ifndef FIZZLE_H
#define FIZZLE_H

#include "DataTypes\FizzleDataTypes.h"
#include "Rendering\FizzleRendering.h"
#include "Sound\FzlAudio.h"
#include "Utils\FzlLogging.h"
#include "Utils\FzlError.h"
#include "Window\FizzleWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \mainpage
 * Fizzle is a simple sprite loading and rendering library
 * with windowing functionality. It is intended to provide
 * a bare-minimum set of tools so that programmers new to
 * video game development can worry less about the tools
 * they use to make games, and more on how to use them
 * to their full effectiveness.
 */

/**
 * @file Fizzle.h
 * 
 * Check the header files above for the actual functionality. 
 * Below you'll find the two functions you _need_ to call at
 * the beginning/end of the program.
 */

/**
 * Initializes all systems.
 *
 * @param windowName Title of the window
 * @param windowWidth Width of the window, in pixels
 * @param windowHeight Height of the window, in pixels
 * @param fullscreen Toggle for fullscreen. 1 - Full, 0 - Windowed
 */
void FzlInit(const char *windowName, int windowWidth, int windowHeight, int fullscreen);

/**
 * Destroys generated objects and shuts down Fizzle. Should be called
 * prior to termination of your program.
*/
void FzlDestroy();

#ifdef __cplusplus
}
#endif

#endif
