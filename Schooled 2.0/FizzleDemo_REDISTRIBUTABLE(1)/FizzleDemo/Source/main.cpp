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

#include <Fizzle\Fizzle.h>

int main()
{
	//The first thing we do is initialize the library with the function below
	//Provide it with a name for the window, in addition to a set of dimensions.
	//The trailing 0 means 'windowed' mode. 

	const int windowWidth = 640;
	const int windowHeight = 480;

	FzlInit("This is my Fizzle window.", 640, 480, 0);

	//Now that Fizzle is initialized, we can create some sprites.

	//Here's how we make a sprite with no animation.
	//The dimensions cover the _entire_ image, which makes sense since
	//this sprite doesn't have animations and has only one frame.
	FzlSpriteHandle staticDoge = FzlLoadSprite("../../../Assets/doge.png", 256, 277);

	//On the contrary, the dimensions in this function represent the size of the individual 
	//frame, since there are many frames in this animation.
	FzlSpriteHandle animatedPenguin = FzlLoadSprite("../../../Assets/penguin.png", 41, 42);

	bool shouldClose = false;

	double dogePosX = 0.0f;
	double dogePosY = 0.0f;

	int penguinAnim = 0;
	int penguinFrame = 0;
	float penguinAngle = 0;

	//Let's loop until we close
	while (!shouldClose)
	{
		//These two functions are useful for getting keyboard and mouse button state
		if (FzlGetKey(FzlKeySpace) && FzlGetMouseButton(FzlMouseButtonLeft))
		{
			FzlGetMousePos(&dogePosX, &dogePosY);

			dogePosY = static_cast<float>(windowHeight) - static_cast<float>(dogePosY);
		}

		//We can use this function to draw a static sprite at a given position and rotation
		FzlDrawSprite(staticDoge, static_cast<float>(dogePosX), static_cast<float>(dogePosY), 0.0f);

		//Likewise, this function lets you draw a sprite which has many frames.
		FzlDrawAnimatedSprite(animatedPenguin, penguinAnim, penguinFrame, 100.0f, 100.0f, penguinAngle);

		//With some manipulation of variables, we can add some nifty functionality.
		++penguinFrame;

		if (penguinFrame >= 8)
		{
			penguinFrame = 0;
			++penguinAnim;

			if (penguinAnim >= 8)
				penguinAnim = 0;
		}

		penguinAngle += 2.0f;

		if (penguinAngle > 360.0f)
		{
			penguinAngle = penguinAngle - 360.0f;
		}

		FzlSwapBuffers();

		//Until the user hits the escape key, keep looping
		shouldClose = FzlGetKey(FzlKeyEscape);
	}

	//Once we're done with our program, we should terminate it like so:
	FzlDestroy();
}