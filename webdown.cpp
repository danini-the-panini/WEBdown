// The MIT License (MIT)

// Copyright (c) 2014 Daniel Smith

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "webdown.h"

using namespace webdown;

// @#$%@#$% C++ >:(
std::string SourceReference::
source(const Source& src) const
{
  if (!src.hasBlock(name))
  {
    std::cerr << "Cannot Find Source Block " << name << std::endl;

    return std::string();
  } else
  return src.getBlock(name).source(src);
}

Source::
Source()
  : block(NULL), raw(NULL)
{
  // nothing happened
}

Source::
Source(std::istream& in)
  : block(NULL), raw(NULL)
{
  read(in);
}

Source::
~Source()
{
  // nothing happened
}


bool Source::
read(const std::string& src)
{
  std::istringstream in(src);
  return read(in);
}

void Source::
_dumpWorkingBlock()
{
  if (block != NULL)
  {
    _dumpRawCode();
    blocks[block->name] = block->value;
    delete block;
    block = NULL;
  }
}

void Source::
_dumpRawCode()
{
  if (raw != NULL)
  {
    // std::cout << "CODE: " << raw->str() << std::endl;
    block->value.append( new RawSource(raw->str()) );
    delete raw;
    raw = NULL;
  }
}

void Source::
_newWorkingBlock(std::string line)
{
  _dumpWorkingBlock();

  size_t a = line.find("<{");
  size_t b = line.find("}>");
  if (a == std::string::npos || b == std::string::npos)
  {
    // not a new code block
    return;
  }

  block = new WorkingBlock();
  block->name = trim(line.substr(a+2, b-a-2));

  if (line.find("+=") != std::string::npos) // append code block
  {
    if (blocks.count(block->name) == 0)
    {
      // code block does not exist
      // TODO: say which line number.
      std::cerr << "Code block " << block->name << " does not exist at this point" << std::endl;
      delete block;
      block = NULL;
      return;
    } else
    {
      block->value = blocks[block->name];
    }
  } else if (line.find("=") != std::string::npos) // assign code block
  {
    block->value = SourceBlock();
  } else
  {
    // not a new code block
    delete block;
    block = NULL;
    return;
  }

  // std::cout << "NEW BLOCK: " << block->name << std::endl;
}


void Source::
_processCode(std::string line)
{
  // recursive base case
  if (line.empty()) return;

  size_t a = line.find("<{");
  size_t b = line.find("}>");
  if (b < a) b = line.find("}>", a);
  if (a == std::string::npos || b == std::string::npos)
  {
    // CASE: no valid source reference on this line
    // so we just accumulate it as raw source
    if (raw == NULL) raw = new std::stringstream();
    (*raw) << line << std::endl;
  } else
  {
    // CASE: source reference on this line 

    std::string before = line.substr(0,a); // grab the source before it
    if (!before.empty())
    {
      // CASE: raw code before the reference in this line
      // so we just accumulate it as raw source
      if (raw == NULL) raw = new std::stringstream();
      (*raw) << before; // insert a space just in case
    }

    // append the referenced block as SourceReference
    _dumpRawCode();
    std::string refname = trim(line.substr(a+2, b-a-2));
    // std::cout << "REF: " << refname << std::endl;
    block->value.append( new SourceReference(refname) );

    // recursively process the rest of the line
    _processCode( trim(line.substr(b+2)) );
  }
}

bool Source::
read(std::istream& in)
{
  std::string line;
  while (_getLine(in, line)) {
    if (!isBlank(line)) {
      // std::cout << line << std::endl;
      if (line.find("    ") == 0)
      {
        // process the code only if we are in a working block
        // (otherwise it's just a pretty code block not to be compiled)
        if (block != NULL)
          _processCode(line.substr(4));
      } else if (line.find("  ") == 0)
      {
        _newWorkingBlock(line);
      } else
      {
        _dumpWorkingBlock();
      }
    }
  }

  _dumpWorkingBlock();

  return true;
}

void Source::
write(std::ostream& out) const
{
  if (blocks.count("Main") == 0)
  {
    std::cerr << "No Main Source Block!" << std::endl;
  } else
  {
    out << blocks.at("Main").source(*this);
  }
}

bool Source::
hasBlock(std::string name) const
{
  return blocks.count(name) > 0;  
}

SourceBlock Source::
getBlock(std::string name) const
{
  return blocks.at(name);
}

bool Source::
_getLine(std::istream& in, std::string& line) {
  // Handles \n, \r, and \r\n (and even \n\r) on any system. Also does tab-
  // expansion, since this is the most efficient place for it.
  line.clear();

  bool initialWhitespace=true;
  char c;
  while (in.get(c)) {
    if (c=='\r') {
      if ((in.get(c)) && c!='\n') in.unget();
      return true;
    } else if (c=='\n') {
      if ((in.get(c)) && c!='\r') in.unget();
      return true;
    } else if (c=='\t') {
      size_t convert=4;
      line+=std::string(convert-(line.length()%convert), ' ');
    } else {
      line.push_back(c);
      if (c!=' ') initialWhitespace=false;
    }
  }
  return !line.empty();
}
