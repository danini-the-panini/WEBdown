Hello World
===========

The Meat of It
--------------

All we really want to do it wrtie something to the screen, so we're going to do just that!

  &lang; Print to Screen &rang;=

    printf("%s\n", message);

What do we need?
----------------

In order to print to the screen we need to include some library that can enable us to do that. Fortunately, C has such a library built into it, and all we have to do is include the standard I/O header file.

  &lang; Includes &rang;=

    #include <stdio.h>

What are we printing
--------------------

We are going to need a message to print out. This will be stored in a global variable.

  &lang; Global Variables &rang;=

    const char* message = "Hello World";

Putting it all together
-----------------------

Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat facer possim assum. Typi non habent claritatem insitam; est usus legentis in iis qui facit eorum claritatem. Investigationes demonstraverunt lectores legere me lius quod ii legunt saepius. Claritas est etiam processus dynamicus, qui sequitur mutationem consuetudium lectorum. Mirum est notare quam littera gothica, quam nunc putamus parum claram, anteposuerit litterarum formas humanitatis per seacula quarta decima et quinta decima. Eodem modo typi, qui nunc nobis videntur parum clari, fiant sollemnes in futurum.

  &lang; Main &rang;=

    &lang; Includes &rang;
    &lang; Global Variables &rang;

    int main()
    {
      &lang; Print to Screen &rang;

      return 0;
    }
