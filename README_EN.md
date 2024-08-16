# BrickGame Tetris

## Project Description

BrickGame Tetris is an implementation of the classic Tetris game in the C programming language. The project is developed using a structured approach and consists of two main parts:

1. A library implementing the game logic;
2. A terminal interface developed using the `ncurses` library.

## Features

- Full implementation of classic Tetris gameplay;
- Use of `ncurses` to create an interactive terminal interface;
- Score and level system;
- Display of the next figure;
- Saving and loading of high scores;
- Full coverage with unit tests using the `Check` library.

## Requirements

- `gcc` compiler;
- `ncurses` library;
- `Check` library (for running tests).

## Building and Running

The project uses a Makefile with the following targets:

- `make all`: Build the project;
- `make install`: Install the game;
- `make uninstall`: Remove the installed game;
- `make clean`: Clean build files;
- `make dvi`: Generate documentation;
- `make dist`: Create a source code archive;
- `make test`: Run unit tests;
- `make gcov_report`: Generate a code coverage report.

To run the game after building, execute:

```
./build/tetris
```

## License

This project is part of the educational program at [School21](https://21-school.ru/) - an educational project by Sber for developers.