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

#ifndef WEBDOWN_H_INCLUDED
#define WEBDOWN_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start
static inline std::string ltrim(std::string s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string rtrim(std::string s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string trim(std::string s)
{
    return ltrim(rtrim(s));
}


static inline bool isBlank(std::string line)
{
  return trim(line).empty();
  //return line.find_first_not_of(" \t\n\v\f\r") != std::string::npos;
}

namespace webdown
{

  // @#$%@#$% C++ >:(
  class Source;

  // Base class interface for Source Tokens
  class SourceToken
  {
    public:
      // Retrieve the actual source code for the token
      virtual std::string source(const Source& src) const = 0;
  };

  // Raw Source token.
  class RawSource : public SourceToken
  {
    public:
      RawSource(std::string s) : data(s) {}

      // Retrieve the raw source as-is.
      virtual std::string source(const Source& src) const
      {
        return data;
      }

    private:
      std::string data;
  };

  // A reference to a source block
  class SourceReference : public SourceToken
  {
    public:
      SourceReference(std::string s) : name(s) {}

      // Retrieve the source from the referenced block
      virtual std::string source(const Source& src) const;

    private:
      std::string name;
  };

  // A Block of source, aggregates a list of tokens
  class SourceBlock : public SourceToken
  {
    public:
      SourceBlock() {}

      // Retrieve and concatenate all the source in the tokens
      virtual std::string source(const Source& src) const
      {
        std::stringstream ss;
        for (auto t : tokens)
        {
          ss << t->source(src);
        }
        return ss.str();
      }

      void append(SourceToken* token)
      {
        tokens.push_back(token);
      }

    private:
      std::vector<SourceToken*> tokens;
  };

  // Represents a block that is being worked on.
  struct WorkingBlock
  {
    std::string name;
    SourceBlock value;
  };

  // Represents a parsed WEBdown source file.
  class Source
  {
    public:
      Source();
      Source(std::istream& in);
      ~Source();

      bool read(const std::string&);
      bool read(std::istream&);
      void write(std::ostream&) const;

      bool hasBlock(std::string) const;
      SourceBlock getBlock(std::string) const;

    private:
      void _processCode(std::string line);
      void _dumpRawCode();
      void _dumpWorkingBlock();
      void _newWorkingBlock(std::string);
      bool _getLine(std::istream& in, std::string& line);

      std::map<std::string, SourceBlock> blocks;
      WorkingBlock* block;
      std::stringstream* raw;
  };

} // namespace webdown

#endif