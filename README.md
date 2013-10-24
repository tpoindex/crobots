CROBOTS
=======

This is the source code of the original CROBOTS game that I wrote in 1985, now
released under GPLv2.   

I probably will **not** be updating CROBOTS, so feel free to fork the code.

Original Readme
---------------

CROBOTS ("see-robots") is a game based on computer programming.
Unlike arcade type games which require human inputs controlling
some object, all strategy in CROBOTS must be complete before the
actual game begins.  Game strategy is condensed into a C language
program that you design and write.  Your program controls a robot
whose mission is to seek out, track, and destroy other robots,
each running different programs.  Each robot is equally equipped,
and up to four robots may compete at once.  CROBOTS is best
played among several people, each refining their own robot
program, then matching program against program.

CROBOTS consists of a C compiler, a virtual computer, and
battlefield display (text graphics only, monochrome or color).
The CROBOTS compiler accepts a limited (but useful) subset of
the C language.  The C robot programs are aided by hardware
functions to scan for opponents, start and stop drive mechanisms,
fire cannons, etc.  After the programs are compiled and loaded
into separate robots, the battle is observed.  Robots moving,
missiles flying and exploding, and certain status information are
displayed on the screen, in real-time.

CROBOTS is distributed under terms of the GNU General Public
License, version 2.



Running under DOSBox
--------------------

You can run the original MS-DOS executable using [DOSBox](http://www.dosbox.com/).
DOSBox is ported to Linux, FreeBSD, Mac OSX, and a few other O/Ses.
DOSBox can be tweaked to run executables as experienced on a 1980's era
[4.77 MHz Intel 8088](http://www.dosbox.com/wiki/4.77_MHz).  

 * Download and install [Dosbox](http://www.dosbox.com/download.php?main=1).  
   For Linux users, you can probably use your package manager to install,
   _sudo apt-get install dosbox_, etc.
 * **cd bin**
 * Tweak the crobots_dosbox.conf as needed, especially the _cycles_ value.
   The default cycles value is 245, which should be close for modern CPUs.
 * Run **crobots_dosbox.sh** or **crobots_dosbox.bat**   
 * **Ignore** the banner that says CROBOTS is 
   [Shareware](https://en.wikipedia.org/wiki/Shareware),  I don't want your
   money and I don't live in Illinois any longer.
 
This should start up CROBOTS with the four demo robots programs.  Programs are
listed to the screen as they compile.  Once all four are compiled, the match 
begins.  

Documentation for CROBOTS is in [docs/crobots_manual.html](http://htmlpreview.github.io/?https://github.com/tpoindex/crobots/blob/master/docs/crobots_manual.html)

History
-------

CROBOTS was my first adventure into the world of compilers and virtual 
machines, and not all that long after I learned C.

In early 1985, I found Jeff Lee's posting of a Yacc grammar for ANSI C, 
on the USENET group 
[net.sources](https://groups.google.com/forum/#!search/net.sources$20yacc$20ansi$20c/net.sources/3gmx4As0aSM/F--W3xnQlEsJ).  Taking Jeff's advice in his post 
_"(Y)ou can sit and crank your own output through it to amuse yourself if 
you have the personality of a cumquat(sp?)"_, 
I played with Yacc/Lex and the C grammar to the point of moderately 
understanding how it all worked.

I had previously played the Apple \]\[ game, 
[RobotWar](https://en.wikipedia.org/wiki/RobotWar)
that a friend owned in the early 1980's.  RobotWar, and to some
degree [CoreWar](https://en.wikipedia.org/wiki/Core_War), was the 
inspiration for writing CROBOTS.  _IMHO_, these are the ultimate games 
for programmers.

The CROBOTS virtual machine evolved while writing the
compiler, making up instructions and constructs as I progressed.
I ended up with a stacked-based CPU that had 10 instructions.  Both
compiler and virtual machine are fairly crude, but obviously usable.

CROBOTS was developed on a couple of Unix machines I had access to, 
[Venix/86](https://en.wikipedia.org/wiki/Venix) and
[Xenix/286](https://en.wikipedia.org/wiki/Xenix), mostly for the _yacc_ and
_lex_ programs found on those systems..  A good deal of the 
development was done with the 
[DeSmet C compiler](http://desmet-c.com/).  Final compilation
was done with [Lattice C](https://en.wikipedia.org/wiki/Lattice_C), as
I recall the performance was slightly better. 



