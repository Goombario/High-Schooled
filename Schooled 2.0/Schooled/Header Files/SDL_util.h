#ifndef SDL_UTIL_H
#define SDL_UTIL_H

#include <iostream>
#include <SDL.h>
#include <string>

namespace SDL_util
{
	/**
	* Log an SDL error with some error message to the output stream of our choice
	* @param os The output stream to write the message to
	* @param msg The error message to write, format will be msg error: SDL_GetError()
	*/
	void logSDLError(std::ostream &os, const std::string &msg);

	/**
	* Loads an image into a texture on the rendering device
	* @param file The image file to load
	* @param ren The renderer to load the texture onto
	* @return the loaded texture, or nullptr if something went wrong.
	*/
	SDL_Texture* loadTexture(const std::string &, SDL_Renderer *);

	/**
	* Draw an SDL_Texture to an SDL_Renderer at some destination rect
	* taking a clip of the texture if desired
	* @param tex The source texture we want to draw
	* @param ren The renderer we want to draw to
	* @param dst The destination rectangle to render the texture to
	* @param clip The sub-section of the texture to draw (clipping rect)
	*		default of nullptr draws the entire texture
	*/
	void renderTexture(SDL_Texture *, SDL_Renderer *, SDL_Rect, SDL_Rect * = nullptr);

	/**
	* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
	* the texture's width and height and taking a clip of the texture if desired
	* If a clip is passed, the clip's width and height will be used instead of
	*	the texture's
	* @param tex The source texture we want to draw
	* @param ren The renderer we want to draw to
	* @param x The x coordinate to draw to
	* @param y The y coordinate to draw to
	* @param clip The sub-section of the texture to draw (clipping rect)
	*		default of nullptr draws the entire texture
	*/
	void renderTexture(SDL_Texture *, SDL_Renderer *, int, int, SDL_Rect * = nullptr);

	/**
	* Render the message we want to display to a texture for drawing
	* @param message The message we want to display
	* @param fontFile The font we want to use to render the text
	* @param color The color we want the text to be
	* @param fontSize The size we want the font to be
	* @param renderer The renderer to load the texture in
	* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
	*/
	SDL_Texture* renderText(const std::string&, const std::string&, SDL_Color, int, SDL_Renderer *);

	/*
	* Get the resource path for resources located in res/subDir
	* It's assumed the project directory is structured like:
	* bin/
	*  the executable
	* res/
	*  Lesson1/
	*  Lesson2/
	*
	* Paths returned will be Lessons/res/subDir
	*/
	std::string getResourcePath(const std::string &subDir = "");
}

#endif