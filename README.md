# Maxwell Equations

This is a simulation of the Maxwell's equations.

## Setup

This project is written in both C and Python.

First build the C library:

```bash
./.vscode/build-swig.sh 
```

The project can be run with:

```bash
./main.py
```

## Controls

| Key | Control |
|-|-|
|w|move up|
|a|move left|
|s|move down|
|d|move right|
|*shift*|move to a lower layer|
|*space*|move to a higher layler|
|*escape*| reset view |
|e| view electric field |
|b| view magnetic field |
|i| tile selection screen |
|*number keys*| select tile |
