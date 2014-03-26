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

The original WEB program by Knuth provided the commands TANGLE and WEAVE, which given a .WEB file would produce source code and a TeX document respectivly. WEBdown also provides a TANGLE command, extracting the named source blocks in a .wd file and producting the complete source code.

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

Instead of providing a WEAVE command, as in Knuth's original implementation, any standard Markdown converter will be able to interpret a .wd file as normal Markdown. The `<{ ... }>` syntax will be ignored, which is a bit of a bummer. However, you can simply pre-process this out using `sed` or something similar.

How to install it
-----------------

It should be as simple as running `make`. If successful, it should produce the `tangle` executable. If not, tell me about the problem.

There are no dependencies, except for a C++11 standard compiler.

Legal stuff
-----------

This product is licensed under the MIT lisence. See LICENSE for the nitty-gritty details.