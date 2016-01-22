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

#ifndef FZL_INTERNAL_ERRORS_H
#define FZL_INTERNAL_ERRORS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file FzlError.h
 * @brief Provides assert and warning check functionalities.
 * 
 * An assert is a function which evaluates a condition and depending on 
 * success or failure will either let the program continue or halt. In the 
 * latter case, it will display a specified error message. This is useful
 * for critical error behaviour.
 * 
 * A warning is similar to an assert in its evaluatioon of a condition,
 * but in the event of a failure it will only write a message. The program
 * will not halt. This is useful for non-critical error behaviour.
 *
 */

/**
 * Tests an assertion and writes a message if _cond_ evaluates to false. Then, the
 * program crashes.
 * 
 * @param cond The condition to evaluate
 * @param msg The message to log if the condition evaluates to false. * 
 */
#define FzlAssert(cond, msg) FzlErrorAssertion((cond), FZL_LOG_MSG_INFO, (msg))

/**
 * Tests an assertion and writes a formatted message if _cond_ evaluates to false. Then,
 * the program crashes.
 *
 * @param cond The condition to evaluate.
 * @param fmt The format of the message to display if the condition returns false.
 * @param args The additional arguments for the message.
 */
#define FzlAssertFmt(cond, fmt, ...) FzlErrorAssertion((cond), FZL_LOG_MSG_INFO, (fmt), __VA_ARGS__)

/**
 * Tests an assertion and writes a message if _cond_ evaluates to false. 
 * 
 * @param cond The condition to evaluate.
 * @param msg The message to log if the condition evaluates to false.
 */
#define FzlWarn(cond, msg) FzlErrorWarning((cond), FZL_LOG_MSG_INFO, (msg))

/**
 * Tests an assertion and writes a formatted message if _cond_ evaluates to false.
 *
 * @param cond The condition to evaluate.
 * @param fmt The format of the message to display if the condition returns false.
 * @param args The additional arguments for the message.
 */
#define FzlWarnFmt(cond, fmt, ...) FzlErrorWarning((cond), FZL_LOG_MSG_INFO, (fmt), __VA_ARGS__)

/**
 * Evaluates a condition and upon failure will write a message to a log and crash the program.
 *
 * @param condEval The condition to evaluate.
 * @param func The name of the function where the assert is called.
 * @param file The file containing the function calling the assertion.
 * @param line The line in the file calling the assertion.
 * @param fmt The format of the assertion message
 * @param ... The arguments for the assertion message.
 */
void FzlErrorAssertion(int condEval, const char *func, const char *file, int line, const char *fmt, ...);

/**
 * Evaluates a condition and upon failure will write a message to a log.
 *
 * @param condEval The condition to evaluate.
 * @param func The name of the function where the warning is called.
 * @param file The file containing the function calling the warning.
 * @param line The line in the file calling the warning.
 * @param fmt The format of the warning message
 * @param ... The arguments for the warning message.
 */
void FzlErrorWarning(int condEval, const char *func, const char *file, int line, const char *fmt, ...);

#ifdef __cplusplus
}

#endif

#endif
