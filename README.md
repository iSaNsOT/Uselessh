# Uselessh
A shell (for **Linux**), but worse than existing solutions 🙃

## How to use it?
Download the code, compile and execute, as simple as that.

- ### With Make
Just `make` to compile and create the executable (the debug flag is already set). Also it's possible to do `make clean` to get rid of the executable.

- ### Other
Like any other C program, a simple way could be `gcc uselessh.c -o uselessh`.

The one presented in **Makefile** is `gcc -g -Wall uselessh.c -o uselessh`. (We use `-g` for produce debugging information in the operating system’s native format (stabs, COFF, XCOFF, or DWARF). GDB can work with this debugging information and `-Wall` to enable all the warnings about constructions).


## Limitations
It's not suppose to compete with any other shell, so...

- No history of commands.
- No autocomplete.
- No piping (could be implemented).
- Only supports build-in commands (more info using `help` in the shell) and the more basic ones. Also support commands interpreted through enviroment variables (PATH).
- Only for Linux (some system calls are POSIX, like `getcwd()`, and does not work on Windows (no UNIX), not even in macOS, [the table of syscalls for macOS](https://opensource.apple.com/source/xnu/xnu-6153.61.1/bsd/kern/syscalls.master.auto.html))
- Probably a lot more things... 😒.
