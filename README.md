# bspc [![Build Status](https://travis-ci.org/sago007/bspc.svg?branch=master)](https://travis-ci.org/sago007/bspc)

This is the [Quake III: Arena](http://www.idsoftware.com/games/quake/quake3-arena/) BSP-to-AAS compiler.
This is a slight variation that combines a couple of different versions from github into one.

Another difference is that a lot of legacy support has been removed. Q1, Q2, Sin, Half-Life are no longer suported.
This has reduced the number of code lines significantly.
The basic source files has also been converted to C++11, to make future enhancement easier.
The code is still a bit of a mine field but I am working on changing that.

## Downloading

You can download the latest version [here](https://github.com/sago007/bspc).

## Compiling

Provided that you have CMake, A C compiler (clang or gcc) and a C++11 compiler (clang or gcc based) do:

	cmake . && make

The Windows version is cross compiled using MXE (www.mxe.cc): 

  i686-w64-mingw32.static-cmake . && make

## Usage

Straight from the source:

	Usage:   bspc [-<switch> [-<switch> ...]]
	Example 1: bspc -optimize -forcesidesvisible -bsp2aas /quake3/baseq3/maps/mymap?.bsp
	Example 2: bspc -optimize -forcesidesvisible -bsp2aas /quake3/baseq3/pak0.pk3/maps/q3dm*.bsp

	Switches:
	   bsp2aas  <[pakfilter/]filter.bsp>    = convert BSP to AAS
	   reach    <filter.bsp>                = compute reachability & clusters
	   cluster  <filter.aas>                = compute clusters
	   aasopt   <filter.aas>                = optimize aas file
	   aasinfo  <filter.aas>                = show AAS file info
	   output   <output path>               = set output path
	   threads  <X>                         = set number of threads to X
	   cfg      <filename>                  = use this cfg file
	   optimize                             = enable optimization
	   noverbose                            = disable verbose output
	   breadthfirst                         = breadth first bsp building
	   nobrushmerge                         = don't merge brushes
	   noliquids                            = don't write liquids to map
	   freetree                             = free the bsp tree
	   nocsg                                = disables brush chopping
	   forcesidesvisible                    = force all sides to be visible
	   grapplereach                         = calculate grapple reachabilities
	   logpatches                           = log per-patch stats during -bsp2aas

If bots are just standing still then you might be missing "-forcesidesvisible"

## Works on

This version is primarily targeted [OpenArena](http://www.openarena.ws). It is tested on http://files.poulsander.com/~poul19/public_files/intooa.pk3 and http://files.poulsander.com/~poul19/public_files/islandctf4a3.pk3

## Support

[File a bug report](https://github.com/sago007/bspc/issues) if you run into issues.

## License

This program is licensed under the GNU Public License v2.0 and any later version.
