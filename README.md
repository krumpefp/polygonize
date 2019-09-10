# Polygonize

This very small C++ app assembles a proper polygon  arrangement from a set of
line segments in 2d.

A **Polygon** may be a _Single Point_ or a _Polyline_ (a sequence of 2d points)
or a _closed Polygon_ (a closed polyline). The later two are guaranteed to be
free of intersections.
A _closed Polygon_ might contain other Polygons.

A **Polygon Arrangement** is an arrangement of the polygons such that the 
containeds relations are properly defined.

## Input

A set of line segments (a line is defined by its start and end point)
See [the format description](./format.txt) for the input definition

If the input contains line intersections, the corresponding edges will be
splitted and a new point will be introduced at the intersection point.
Hence the arrangement may contain more points than the original input.

## Output

A proper representation of the polygon arrangement.
See [the format description](./format.txt) for the output definition


# Installation

The following process is valid for Linux, unchecked for other OSses.

## Requirements

Requires git, a C++ Compiler and CMake.
The following packages need to be installed on the system:

* CGAL
* GMP
* MPFR

```
    $ git clone https://github.com/krumpefp/polygonize.git
    $ cd mkdir polygonize/build && cd polygonize/build
    $ cmake ..
    $ make install
```

This creates the binary in the subfolder polygonize/build/bin


# Run example

After successfull _make install_ the binary can be used to convert a collection
of line segments from the command line.

```
    $ ./bin/polygonize < ../data/test.txt > out.txt
    $ less out.txt
```

In the given example the input is streamed from the file _../data/test.txt_.
The output is streamed into the file _out.txt_ which is visualized by less in
the last line.


# Disclaimer:

This is a very simple script. Wrong input (e.g. invalid input for the point
coordinates will probably lead to program crashes). 