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

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include "webdown.h"

using std::cerr;
using std::cout;
using std::endl;

namespace {

  class Options {
    public:

    bool readOptions(int argc, char *argv[]);

    void showHelp();

    std::string inputFile() const { return mInputFile; }

    private:
    std::string mInputFile;
  };

  bool Options::readOptions(int argc, char *argv[]) {
    bool help=false, err=false;
    for (int x=1; x<argc && !help && !err; ++x) {
      if (argv[x][0]=='-' && argv[x][1]=='-' && argv[x][2]!=0) {
        // It's a full-word option.
        std::string opt(argv[x]+2);
        if (opt=="help") {
          help=true;
        } else {
          err=true;
          cerr << "Unknown option " << argv[x] << ", use -? for help."
            << endl;
        }
      } else if (argv[x][0]=='-') {
        // It's one or more option flags.
        const char *i=argv[x]+1;
        while (*i!=0 && !help && !err) {
          switch (*i) {
            case '?': help=true; break;

            default:
            err=true;
            cerr << "Uknown option flag '" << *i << "', use -? for "
              "help." << endl;
          }
          ++i;
        }
      } else {
        // It's a parameter.
        if (mInputFile.empty()) {
          mInputFile=argv[x];
        } else {
          err=true;
          cerr << "Too many parameters. Already had '" << mInputFile
            << "', found '" << argv[x] << "' too. Use -? for help."
            << endl;
        }
      }
    }

    if (help) {
      showHelp();
      return false;
    } else if (err) {
      return false;
    } else return true;
  }

  void Options::showHelp() {
    const char *cHelpScreen=
      "This program tangles input (from an input file or stdin) from WEBdown syntax\n"
      "to raw source code.\n"
      "\n"
      "Usage:\n"
      "    tangle [<option>...] [input-file]\n"
      "\n"
      "Available options are:\n"
      "    -?, --help      Show this screen.\n";
    cerr << endl << cHelpScreen << endl;
  }

} // namespace

int main(int argc, char *argv[]) {
  Options cfg;
  if (!cfg.readOptions(argc, argv)) return 1;

  std::ifstream ifile;

  std::istream *in=&std::cin;
  if (!cfg.inputFile().empty()) {
    cerr << "Reading file '" << cfg.inputFile() << "'..." << endl;
    ifile.open(cfg.inputFile().c_str());
    if (!ifile) {
      cerr << "Error: Can't open file." << endl;
      return 1;
    } else in=&ifile;
  } else cerr << "Reading standard input..." << endl;

  webdown::Source src;
  src.read(*in);

  src.write(cout);

  return 0;
}
