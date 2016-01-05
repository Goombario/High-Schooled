#ifndef CLEANUP_H
#define CLEANUP_H

#include <utility>
#include <SDL.h>

/*
* Recurse through the list of arguments to clean up, cleaning up
* the first one in the list each iteration.
*/
template<typename T, typename... Args>
void cleanup(T *t, Args&&... args){
	//Cleanup the first item in the list
	cleanup(t);
	//Recurse to clean up the remaining arguments
	cleanup(std::forward<Args>(args)...);
}

template<>
void cleanup<SDL_Window>(SDL_Window *win);

template<>
void cleanup<SDL_Renderer>(SDL_Renderer *ren);

template<>
void cleanup<SDL_Texture>(SDL_Texture *tex);

template<>
void cleanup<SDL_Surface>(SDL_Surface *surf);

#endif