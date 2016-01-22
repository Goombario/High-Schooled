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

#ifndef FZL_LOGGING_H
#define FZL_LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file FzlLogging.h
 * @brief Handy logging functionality.
*/

/**
 * These enums represent the type of message to log.
 */
enum FzlLogMsgType
{
	FzlLogMsgTypeError,
	FzlLogMsgTypeWarning,
	FzlLogMsgTypeDebug
};

#define FZL_LOG_MSG_INFO __FUNCTION__, __FILE__, __LINE__

/**
 * Logs a message of type error.
 * @param msg The message to log.
 */
#define FzlLogError(msg) FzlLogWrite(FzlLogMsgTypeError, FZL_LOG_MSG_INFO, (msg))

/**
 * Logs a formatted message of type error.
 * @param fmt The format of the error message
 * @param args The arguments of the formatted error message
 */
#define FzlLogErrorFmt(fmt, args) FzlLogWrite(FzlLogMsgTypeError, FZL_LOG_MSG_INFO, (fmt), (args))

/**
 * Logs a message of type warning.
 * @param msg The message to log.
 */
#define FzlLogWarning(msg) FzlLogWrite(FzlLogMsgTypeWarning, FZL_LOG_MSG_INFO, (msg));

/**
 * Logs a formatted message of type warning.
 * @param fmt The format of the warning message.
 * @param args The arguments for the formatted warning message.
 */
#define FzlLogWarningFmt(fmt, args) FzlLogWrite(FzlLogMsgTypeWarning, FZL_LOG_MSG_INFO, (fmt), (args))

/**
 * Logs a message of type debug.
 * 
 * @param msg The message to log
 */
#define FzlLogDebug(msg) FzlLogWrite(FzlLogMsgTypeDebug, FZL_LOG_MSG_INFO, (msg))

/**
 * Logs a formatted message of type debug.
 * @param fmt The format of the error message.
 * @param args The arguments for the formatted error message.
 */
#define FzlLogDebugFmt(fmt, args) FzlLogWrite(FzlLogMsgTypeDebug, FZL_LOG_MSG_INFO, (fmt), (args))

/**
 * Writes a message to the log.
 * 
 * @param msgType The type of message to send.
 * @param func The name of the function currently logging the message.
 * @param file The file containing the function currently logging a message.
 * @param line The line where the log is called.
 * @param msgFormat The format of the message.
 * @param ... The additional parameters of the message.
 */
void FzlLogWrite(enum FzlLogMsgType msgType, const char *func, const char *file, int line, const char *msgFormat, ...);

#ifdef __cplusplus
}
#endif

#endif
