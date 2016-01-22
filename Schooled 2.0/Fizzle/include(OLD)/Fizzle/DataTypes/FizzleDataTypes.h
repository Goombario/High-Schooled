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

#ifndef FIZZLE_DATA_TYPES_H
#define FIZZLE_DATA_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @file FizzleDataTypes.h
 * @brief Contains data types used by Fizzle.
 */

/**
 * A sprite returned from FzlLoadSprite().
 */
typedef int FzlSpriteHandle;

/**
 * A sound returned from FzlLoadSound().
 */
typedef int FzlSoundHandle;

/**
 * These enums are used to query keyboard state with FzlGetKey().
 */
enum FzlKey
{
	FzlKeySpace,
	FzlKeyApostrophe,
	FzlKeyComma,
	FzlKeyMinus,
	FzlKeyPeriod,
	FzlKeySlash,

	FzlKeyNumRow0,
	FzlKeyNumRow1,
	FzlKeyNumRow2,
	FzlKeyNumRow3,
	FzlKeyNumRow4,
	FzlKeyNumRow5,
	FzlKeyNumRow6,
	FzlKeyNumRow7,
	FzlKeyNumRow8,
	FzlKeyNumRow9,

	FzlKeySemicolon,
	FzlKeyEquals,

	FzlKeyA, FzlKeyB, FzlKeyC, FzlKeyD,
	FzlKeyE, FzlKeyF, FzlKeyG, FzlKeyH,
	FzlKeyI, FzlKeyJ, FzlKeyK, FzlKeyL,
	FzlKeyM, FzlKeyN, FzlKeyO, FzlKeyP,
	FzlKeyQ, FzlKeyR, FzlKeyS, FzlKeyT,
	FzlKeyU, FzlKeyV, FzlKeyW, FzlKeyX,
	FzlKeyY, FzlKeyZ,

	FzlKeyLeftBracket,
	FzlKeyBackslash,
	FzlKeyRightBracket,
	FzlKeyGraveAccent,

	FzlKeyEscape,
	FzlKeyEnter,
	FzlKeyTab,
	FzlKeyBackspace,
	FzlKeyInsert,
	FzlKeyDelete,
	FzlKeyRightArrow,
	FzlKeyLeftArrow,
	FzlKeyDownArrow,
	FzlKeyUpArrow,
	FzlKeyPageUp,
	FzlKeyPageDown,
	FzlKeyHome,
	FzlKeyEnd,
	FzlKeyCapsLock,
	FzlKeyScrollLock,
	FzlKeyNumLock,
	FzlKeyPrintScreen,
	FzlKeyPause,

	FzlKeyF1,
	FzlKeyF2,
	FzlKeyF3,
	FzlKeyF4,
	FzlKeyF5,
	FzlKeyF6,
	FzlKeyF7,
	FzlKeyF8,
	FzlKeyF9,
	FzlKeyF10,
	FzlKeyF11,
	FzlKeyF12,

	FzlKeyNumPad0,
	FzlKeyNumPad1,
	FzlKeyNumPad2,
	FzlKeyNumPad3,
	FzlKeyNumPad4,
	FzlKeyNumPad5,
	FzlKeyNumPad6,
	FzlKeyNumPad7,
	FzlKeyNumPad8,
	FzlKeyNumPad9,
	FzlKeyNumPadDecimal,
	FzlKeyNumPadDivide,
	FzlKeyNumPadMultiply,
	FzlKeyNumPadSubtract,
	FzlKeyNumPadAdd,
	FzlKeyNumPadEnter,
	FzlKeyNumPadEquals,
	FzlKeyLeftShift,
	FzlKeyLeftCtrl,
	FzlKeyLeftAlt,
	FzlKeyRightShift,
	FzlKeyRightCtrl,
	FzlKeyRightAlt
};

/**
 * These enums are used to query mouse state with FzlGetMouseButton().
 */
enum FzlMouseButton
{
	FzlMouseButtonLeft = 0,
	FzlMouseButtonRight,
	FzlMouseButtonMiddle
};

#ifdef __cplusplus
}
#endif

#endif
