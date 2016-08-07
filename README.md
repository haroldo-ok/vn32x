VN32X v0.2.0
============

This is a tool for creating graphic novels for the Sega 32X.
Work in progress.

So far, it contains:
* A C library for making visual novels;
* Another C library to interface the VN library with the 32X hardware;
* A tool to convert PNG images to a format compatible with the 32X;
* A transpiler that translates a subset of the Ren'py scripting language to a C file (plus a few includes) that uses the VN library;
* A Makefile that takes care of coordinating all of that.
 
In order to compile the library and the generated sources, you will need a C cross compiler for 32X; you can find one on http://sebastianmihai.com/main.php?t=106&n=Sega-32x-development-Eight-Queens

Implemented so far
------------------

* Scripts for converting images to the proper format;
* 32K color mode;
* Image rendering with cropping and optional translucency;
* Antialised variable width font text rendering;
* Word wrapping for the text;
* Joypad input;
* Word wrapping for the text boxes;
* Menus;
* Some image decompression caching to speed up image rendering;
* Create an easy to use library to implement the graphical novels;
* Implement a tool to automatically generate C source code from Ren'Py scripts.

In progress
-----------
* An easy to use installer;
* A visual editor for the scripts; see https://github.com/haroldo-ok/BlocklyVN32X;

Not implemented, but planned
----------------------------
* Better error handling;
* Digital audio playback.

