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

#ifndef FIZZLE_RENDERING_H
#define FIZZLE_RENDERING_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file FizzleRendering.h
 * @brief Contains all functions necessary to render sprites.
 */

/**
 *	Loads a sprite at a given filepath given some additional information.
 *
 *	@param file The file where the image is located
 *	@param frameWidth The width of a SINGLE individual sprite frame, NOT the width of the sheet.
 *	@param frameHeight The height of a SINGLE individual sprite frame, NOT the height of the sheet.
 *
 *	@return A \p FzlSpriteHandle representing the sprite.
*/

FzlSpriteHandle FzlLoadSprite(const char *file, int frameWidth, int frameHeight);

/**
 * Deletes a sprite with a given handle, and frees that handle for later use.
 * 
 * @param handle The sprite handle to delete.
 */
void FzlDeleteSprite(FzlSpriteHandle handle);

/**
 * Draws a sprite at a particular position and rotation.
 * This function is suitable for rendering sprites that have no animation.
 * It will render the entire sprite image, not individual frames.
 *
 * @param handle The handle of the sprite to draw.
 * @param x The position in pixels on the X-axis at which to draw the sprite.
 * @param y The position in pixels on the Y-axis at which to draw the sprite.
 * @param angle An angle in _degrees_ to rotate the sprite.
 *
 *
 */

void FzlDrawSprite(FzlSpriteHandle handle, float x, float y, float angle);


/**
 *	Draws a sprite at a particular animation and frame, in addition to drawing
 *	at a specific position and rotation. Assuming your animations are all in one row
 *	and there is one animation per row, the 'animation' parameter will be one of any 
 *	number of rows moving _down_ the sheet (since the top-left is 0,0), and the 'frame' 
 *	parameter will be the particular sprite moving _along_ the specified row.
 *
 *	@param handle The handle of the sprite to draw.
 *	@param anim The animation to draw (which row in the image).
 *	@param frame The specific frame in that animation to draw (which frame in that row).
 *	@param x The position in pixels on the X-axis to draw the sprite
 *	@param y The position in pixels on the Y-axis to draw the sprite.
 *	@param angle An angle in DEGREES to rotate the sprite.
*/
void FzlDrawAnimatedSprite(FzlSpriteHandle handle, int anim, int frame, float x, float y, float angle);

/**
 * Every time the window is told to swap the buffer, it will clear 
 * everything behind the scenes to ensure a clean canvas for the new frame. 
 * This function lets you control what colour it paints this canvas.
 * 
 * Note that these values are from 0 - 1, where 0 is 0 and 1 is 255.
 *
 * r The red value to paint the window. 
 * g The green value to paint the window.
 * b The blue value to paint the window.
 * a The alpha value, or opacity, to paint the window.
*/
void FzlSetClearColor(float r, float g, float b, float a);

#ifdef __cplusplus
}
#endif

#endif
