# *while* language interpreter for [COMP11212](http://syllabus.cs.manchester.ac.uk/ugt/2017/COMP11212/)

A bare-bones interpreter for the *while* programming language as defined in the notes for COMP12111, written in C, with the help of Bison & Flex for lexer & parser generation.

Example usage:

    $ while test.while
    
    $ while --print-ast test.while

(Any file extension can be used for *while* source files).

Variables need not be initialized explicitly; referencing a variable without initializing it first will simply assume a value of 0.

At the end of the execution, all of the initialized variables will be printed out together with their corresponding values.

# Download

Please visit the [Releases](https://github.com/RequestForCoffee/while-language/releases) page to download the compiled binaries for UNIX and Windows.

# Building

On Windows, an installation of winflexbison3 and MinGW is required (it can be obtained using chocolatey or installed manually). The project can then be built and debugged through Visual Studio.

Alternatively, it can be built through Make (both on Windows and UNIX), given flex & bison binaries are available in system PATH.

**NOTE: the current Visual Studio project configuration looks for winflexbison3 under the default Chocolatey installation directory.**

**NOTE 2: if building on UNIX, the makefile needs to be changed to use flex & bison instead of win_flex and win_bison.**

# Disclaimer

The interpreter is currently very minimal; it does not (yet) support array extensions, or features such as comments or user defined "macros" (e.g. a macro of a > b would be shorthand for writing !(a <= b)). It may also contain bugs, and is in many ways inefficient (e.g. the super-simple and naive "memory" implementation). It can however be used to illustrate the principles of programming language constructs, Backus-Naur form and ASTs.

# References
* [Wikipedia's page on GNU Bison](https://en.wikipedia.org/wiki/GNU_bison)
* [Flex manual](https://westes.github.io/flex/manual/)
* [Bison manual](https://www.gnu.org/software/bison/manual/bison.html)

# License
All code that is not generated by Bison or Flex (generated code is exempt from their corresponding GPL and BSD licenses by exceptions) is available under the MIT license.
