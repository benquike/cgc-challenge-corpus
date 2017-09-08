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

#include "file.h"

BaseFile::~BaseFile() {}

File::File(const char* name, FileType type, size_t size, File* parent)
{
  __strcpy(this->name, name);
  this->type = type;
  this->size = size;
  this->parent = parent;
  this->opened = false;
  this->info.content = 0;
  if (this->type == FT_DIR)
    this->info.files = new List<File *>();
}

File::~File()
{
  if (type == FT_REG && info.content)
    __free(info.content);
  else if (type == FT_DIR && info.files)
    delete info.files;
}

int File::Open()
{
  if (opened)
    return 1;
  opened = true;
  return 0;
}

int File::Close()
{
  if (!opened)
    return 1;
  opened = false;
  return 0;
}

int File::Read(size_t pos, size_t len, char **outBuf)
{
  if (type != FT_REG)
    return -10;
  if (!outBuf)
    return -20;
  if (!opened)
    return -30;
  if (size == 0)
    return -40;
  if (pos >= size)
    return -50;
  if (len == 0)
    len = size;
  len = (len > size - pos) ? (size - pos) : len;
  *outBuf = (char *) __calloc(len, sizeof(char));
  if (!*outBuf)
    return -20;
  __memcpy(*outBuf, &info.content[pos], len);
  return len;
}

int File::Write(size_t pos, char *inBuf, size_t len)
{
  if (type != FT_REG)
    return -10;
  if (!inBuf && len > 0)
    return -20;
  if (!opened)
    return -30;
  if (len == 0)
    return 0;
  if (pos >= size || pos + len > size)
  {
    info.content = (char *) __realloc(info.content, pos + len);
    if (info.content == NULL)
    {
      size = 0;
      return -40;
    }
    size = pos + len;
  }
  __memcpy(&info.content[pos], inBuf, len);
  return len;
}

void File::PrintFileInfo()
{
  if (type == FT_REG)
  {
    __printf("<__FILE> ");
    __printf("%s ", (opened ? "O" : "C"));
  }
  else
  {
    __printf("<DIR> D ");
  }
  __printf("%d ", size);
  __printf("%s", name);
  __printf("\n");
}