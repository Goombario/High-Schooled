#include "SDL_UTIL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

namespace SDL_util
{
	void logSDLError(std::ostream &os, const std::string &msg)
	{
		os << msg << " error: " << SDL_GetError() << std::endl;
	}

	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
	{
		SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
		if (texture == nullptr)
		{
			logSDLError(std::cout, "LoadTexture");
		}
		return texture;
	}

	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip)
	{
		SDL_RenderCopy(ren, tex, clip, &dst);
	}

	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip)
	{
		SDL_Rect dst;
		dst.x = x;
		dst.y = y;
		if (clip != nullptr)
		{
			dst.w = clip->w;
			dst.h = clip->h;
		}
		else
		{
			SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
		}
		renderTexture(tex, ren, dst, clip);
	}

	SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
		SDL_Color color, int fontSize, SDL_Renderer *renderer)
	{
		// Open the font
		TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
		if (font == nullptr)
		{
			logSDLError(std::cout, "TTF_OpenFont");
			return nullptr;
		}

		// We need to first render to a surface as that's what TTF_RenderText
		// returns, then load that surface into a texture
		SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
		if (surf == nullptr)
		{
			TTF_CloseFont(font);
			logSDLError(std::cout, "TTF_RenderText");
			return nullptr;
		}

		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
		if (texture == nullptr)
		{
			logSDLError(std::cout, "CreateTexture");
		}

		// Cleanup surface and font
		SDL_FreeSurface(surf);
		TTF_CloseFont(font);
		return texture;
	}

	std::string getResourcePath(const std::string &subDir){
		//We need to choose the path separator properly based on which
		//platform we're running on, since Windows uses a different
		//separator than most systems
#ifdef _WIN32
		const char PATH_SEP = '\\';
#else
		const char PATH_SEP = '/';
#endif
		//This will hold the base resource path: Lessons/res/
		//We give it static lifetime so that we'll only need to call
		//SDL_GetBasePath once to get the executable path
		static std::string baseRes;
		if (baseRes.empty()){
			//SDL_GetBasePath will return NULL if something went wrong in retrieving the path
			char *basePath = SDL_GetBasePath();
			if (basePath){
				baseRes = basePath;
				SDL_free(basePath);
			}
			else {
				std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
				return "";
			}
			//We replace the last bin/ with res/ to get the the resource path
			size_t pos = baseRes.rfind("Debug");
			baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
		}
		//If we want a specific subdirectory path in the resource directory
		//append it to the base path. This would be something like Lessons/res/Lesson0
		return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
	}

}