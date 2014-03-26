WEBdown
=======

Literate Programming based on Markdown

What is Literate Programming?
-----------------------------

[Read this brilliant paper by Donal Knuth.](www.literateprogramming.com/knuthweb.pdf)

What is WEBdown?
----------------

WEBdown is an implementation of WEB that uses Markdown instead of TeX, and can work with any language off the bat.

How does it work?
-----------------

The original WEB program by Knuth provided the commands TANGLE and WEAVE, which given a .WEB file would produce source code and a TeX document respectivly. WEBdown also provides the TANGLE and WEAVE commands, the former extracting named code blocks from a special Markdown file, and the latter converting the special Markdown file into HTML.

WEBdown extends Markdown's syntax by adding the `<{ ... }>` syntax, which is used to refer to named source blocks. For example, the following code refers to a source block called `Hello World`.

      <{ Hello World }>

Source blocks can be assigned to using the `<{ ... }>=` syntax.

      <{ Hello World }>=

        printf("Hello World");

Source blocks can also be apended to using `<{ ... }>+=`, allowing you to extend pre-defined source blocks. This can only be done to source blocks that have previously been defined as above.

      <{ Hello World }>+=

        printf("\n");

Source blocks can then be referenced withing other source blocks.

      <{ Main }>=

        int main()
        {
          <{ Hello World }>

          return 0;
        }

With any WEBdown file, the source block with the name `Main` will be used as the entry point when using the `tangle` command. If there is no such source block, the `tangle` command will fail.

NB: Code block labels must be indented with two spaces, while the actual code must be indented four spaces (as per usual). Any unnamed code blocks will be ignored, as well as any named code blocks that are not directly or indirectly referenced by the Main code block.

The `weave` command is implemented as a simple bash script that does a bit of `sed` to replace the `<{ ... }>` syntax with a nicer looking &lang; &hellip; &rang; and ultimately just uses the original PERL implementation of Markdown to do all the real work.

How to use it
-------------

The `tangle` command will read from STDIN if no arguments are provided.

    $ tangle < foo.wd

Alternatively, a file name can be provided as an argument, and `tangle` will attempt to open the file and use its contents as input. Any arguments after the first will be ignored.

    $ tangle bar.wd

The weave command works simply by providing the shell script with an argument specifying the .wd file

    $ weave baz.wd

The output will be an HTML file with the same name as the given file.

How to install it
-----------------

It should be as simple as running `make`. If successful, it should produce the `tangle` executable. If not, tell me about the problem.

There are no dependencies, except for a C++11 standard compiler.

TO-DO
-----

  * Make a proper `weave` command that allows referenced code blocks to be clickable.
  * Maybe add macros
  * Re-write the `tangle` command as a literate program.

Legal stuff
-----------

This product is licensed under the MIT lisence. See LICENSE for the nitty-gritty details.