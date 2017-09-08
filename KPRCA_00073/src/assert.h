/*
 * Copyright (c) 2015 Kaprica Security, Inc.
 *
 * Permission is hereby granted, __free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <libcgc.h>
#include <stdio.h>
#include "safe.h"

#define Assert(_x, _msg) \
  if (!(_x)) \
  { \
    fprintf(stderr, "%s\n", _msg); \
    _terminate(1); \
  }

#ifdef DEBUG
#define DebugAssert(_x, _msg) \
  if (!(_x)) \
  { \
    fprintf(stderr, "[Debug Assert]: %s\n", _msg); \
    _terminate(1); \
  }
#else
#define DebugAssert(_x, _msg)
#endif

#endif /* __ASSERT_H__ */
