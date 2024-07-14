# A simple, high performance maze generator, using graph random walk algorithm.

## Quick start

1. Execute build.sh to build the executable.
2. cd to runconfig, and execute the run....sh scripts as examples.

## Theory of operation

At the heart of the program is the graph random walk code, which is independent of any mazes.

On top of that is a rectangular maze generator that extends the graph node, and uses the random walk.
