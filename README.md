# gcc-builds

**For microcontroller projects built with the GNU toolchain.**

*Contributors: Vaughn Kottler, Cooper Green*

Note: Try not to commit build changes that introduce errors or warnings! Sometimes `make clean` followed by `make` is necessary to see the output of each compiled source files as subsequent `make`s won't show errors from previously compiled sources.

## System Prerequisites

The current project assumes `make` is executed by `/bin/bash`. Many projects that leverage `make` contain boilerplating to ensure compatibility with other types of shells but this project does not yet have that support.

It's recommended to [setup an Ubuntu or other Linux development environment](https://www.youtube.com/playlist?list=PLTPrK33wiSskApHw-Tc647bs5f7fbaD24).

Some development boards can have their onboard debuggers reflashed to become [Segger JLink](https://www.digikey.com/product-detail/en/segger-microcontroller-systems/8.08.90-J-LINK-EDU/899-1008-ND/2263130) compatible. For ones that can't it may still be possible to implement functionality implied by make targets, so in the future **install** and **debug** may vary from board to board but for now the Segger JLink is the ideal/selected JTAG programmer (subject to change).

Their software can be downloaded [here](https://www.segger.com/downloads/jlink/). The *J-Link Software and Documentation Pack* and *Ozone* are recommended.

If a toolchain cannot be obtained with `apt-get` it should be made note of here. For ARM targets `arm-none-eabi-XXX` can be installed with `sudo apt-get install gcc-arm-none-eabi`. It can also be [downloaded from source](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) if necessary. Toolchain documentation can be obtained from the documentation repository.

## Documentation Repository

A Subversion repository is set up to contain all necessary documentation and should be added to when new documents are found. Go up one level from this repository and run `svn co http://svn.badgerloop.com/st32`. This will "check out" the Subversion repository into a folder named `st32` (you can give it a different name if you want). Subverion is similar to git in that it is version control software. When you need to make changes to the repository, simply add/stage your files with `svn add` and commit them with `svn ci`.

## Development Instructions

The ideal use of this project is to be able to edit source files and run `make install` which should automatically preprocess, compile, assemble and link the source into an ELF file that will be copied to a binary blob that gets flashed directly to the target device (typically ARM development board) using a number of scripts and utilities. A list of targets that are supported can be found below.

Without the hand-holding (and bottlenecking!!!) of a licensed IDE or Eclipse port it might be possible that changes to `Makefile` or `makefile.conf` will be necessary as development continues and this project matures. It is highly recommended to study these files to understand how the project is being built in the first place, here are some common cases where `Makefile` changes will be necessary:

  * A new source file is added
  * Support for a new board is being worked on
  * An additional argument passed to the toolchain is needed
  * A toolchain argument needs to be omitted
  * An additional target is being created

[Video tutorials](https://www.youtube.com/playlist?list=PLTPrK33wiSskApHw-Tc647bs5f7fbaD24) describing how this project was set up and demonstrating the workflow also exist.

## Development Boards Supported

  * [STM32 Nucleo 144](http://www.st.com/en/evaluation-tools/nucleo-f767zi.html) (STM32F767ZI)

## Make Targets

**clean**

  * Deletes build objects (all \*.o, \*.elf. \*.map, \*.bin and output.txt if it exists).

**install**

  * Runs `./$(PROC_DIR)/install.sh` which usually will be a Segger JLink command which connects to a [reflashed onboard JLink](https://www.youtube.com/watch?v=ezPou8W_Ntc) (i.e. STM32 Nucleos) or actual JLink and executes a script that flashes `$(PROJECT).bin` to the necessary physical address and exits.

**debug**

  * Essentially the same as **install** but instead of exiting *JLinkExe* will remain running (executable gets flashed). This works well with **dump** when doing debugging that requires stepping through instructions.

**dump**

  * Disassembles the project ELF file into *output.txt* and opens it in a new terminal where the default terminal is `gnome-terminal`.

**%\_config**

  * TODO: multi-board & processor support.

## Action Items

Tasks will be tracked and distributed using GitHub's issue system. Issues in most cases will be assigned to one or more individuals at a time but the system works best with active discussion and participation.

## Missing Build Features

Low priority or not planned.

  * Multi-target file structure with make `XXXXX_config` (or other) target configuration options
  * Thorough, multi-platform development instructions
  * Boilerplated Makefile to support Windows/Unix/OSX usage
  * Build timestamp with commit/tag String + Project name banner
  * Git commit conventions established
  * Commenting & code style conventions established

