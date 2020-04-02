# Compile Instructions

First you need to create build and output directories, this can be done executing into
the terminal, from the src directory

  > mkdir -p ../test/out ../build/obj ../build/objdbg

#### Compile

To be able to compile in linux you need to get into **src** directory in your terminal.
There you compile with

  > make

it will create the *program* executable file.

**Important**: The makefile suppose you are using [gcc](https://gcc.gnu.org) version 9.0 or later.

#### Compile and run

If you want to run it, write in your terminal

  > make run

It will compile and run the *program* executable.

#### Compile and run with arguments

If you want to compile, and run with arguments, then you put

  > make run ARGS="\<here your arguments\>"

It will run *program* executable with the arguments contained in the **ARGS** variable.

#### Compile and run for debugging

If you want to run it, write in your terminal

  > make debug

It will compile and run the *program* executable.

#### Compile and run with arguments for debugging

If you want to compile, and run with arguments, then you put

  > make debug ARGS="\<here your arguments\>"

It will run *program* executable with the arguments contained in the **ARGS** variable.

#### Help

For help run the next command

  > make ARGS="-?" run

Is not sure that all the programs will have this.

#### Compile and run the test cases

If you want to compile, and run with the test cases, then you put

  > make test

It will run *program* executable with the test cases given in the [test/data](./test/data) folder.
They are all ran by the [test.sh](./test/test.sh) script.

### Clean space

You can clean normal build, debug build and test execution with

  > make clean

  > make clean-debug

  > make clean-test

respectively.
