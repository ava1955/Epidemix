# Epidemix

Epidemix is a small epidemic simulation written twice: once in C++ with classic object-oriented design (OOP) and once in C with data-oriented design (DOD). Both versions simulate the same world, so you can switch between them while the program runs and compare how fast they are and how much memory they use.

The world is a window full of moving circles. They bounce off the window edges, off a few fixed square obstacles, and off each other. When the virus is turned on, one circle starts out infected. Infection spreads on contact and through surfaces: obstacles accumulate viral load from collisions and can become infectious themselves. Infected circles die five seconds after catching it.

Graphics are drawn with [raylib](https://www.raylib.com/).

## Building

You need CMake 3.31 or newer, a C/C++ compiler with C++20 and OpenMP support, and an internet connection for the first build. The project was developed in CLion with its bundled MinGW toolchain.

raylib 5.5 is downloaded and built automatically by CMake the first time you configure the project, so the first build takes a minute or two.

In CLion, open the folder and run the `Epidemix` target. From a terminal:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target Epidemix
```

Use a Release build when you want to compare performance. Debug builds are several times slower and the numbers are not representative.

The program uses OpenMP, so it needs `libgomp-1.dll` at runtime on Windows. CLion finds it automatically. If you run the exe somewhere else, copy that DLL from your MinGW `bin` folder next to it.

## Running it

The program starts in OOP mode. Press **Space** to switch between OOP and DOD. The top-left corner shows the current mode, the frame rate and the memory used by that mode.

While it runs, you can type commands into the terminal and press Enter:

| Command | What it does |
|---|---|
| `entities 5000` | Restarts both simulations with 5000 circles |
| `speed 250` | Sets how fast the circles move, in pixels per second |
| `virus on` | Restarts with one infected circle |
| `virus off` | Restarts with no virus |

`speed` takes effect immediately. The other commands restart the simulation. The commands have no input checking, so type them carefully.

## Configuration

Starting values live in `config.c`:

| Setting | Meaning |
|---|---|
| `WINDOW_WIDTH`, `WINDOW_HEIGHT` | Window size in pixels |
| `CircleRadius` | Radius of every circle |
| `CircleSpeed` | Movement speed in pixels per second |
| `EntityCount` | Number of circles at startup |
| `TargetFPS` | Frame rate cap |
| `ObstacleSize` | Side length of the square obstacles |
| `ObstacleCount` | How many entries of `Obstacles` to use |
| `Obstacles` | Top-left corner of each obstacle, up to 32 |
| `Virus` | 1 to start with the virus on, 0 for off |
| `VirusLifetime` | Seconds an infected circle lives |
| `InfectionThreshold` | Number of collisions needed for an obstacle to become infectious |
| `InfectionChance` | Probability (1-100) an infectious obstacle infects a healthy circle |

If you add or remove obstacles, update `ObstacleCount` to match.

## Project layout

```
main.cpp            window, main loop, mode switching, terminal commands
config.h / .c       shared settings used by both versions
oop/                C++ version
  entity.h/.cpp     Entity, Shape, Circle and Square classes
  main_oop.cpp      one OOP simulation step
  benchmark.*       FPS counter and memory tracking for OOP
dod/                C version
  entity.h/.c       DODState: one array per property
  main_dod.c        one DOD simulation step
  benchmark.*       FPS counter and memory tracking for DOD
```

## How a frame works

Each frame, the active version runs the same steps in the same order:

1. Remove circles whose infection has run its course.
2. Sort every circle into a grid of small cells, each as wide as the collision distance.
3. Check each circle against the circles in its own cell and the eight cells around it. Colliding circles swap directions, and infection passes between them.
4. Push circles out of any obstacle they overlap and bounce them off it. Obstacles also accumulate collisions here, eventually becoming infectious and spreading the virus to circles.
5. Move every circle and bounce it off the window edges.
6. Draw the circles and obstacles.

Steps 4 and 5 run on all CPU cores through OpenMP, since each circle only changes its own data there. The other steps run on the main thread. Collisions change two circles at once, and raylib only allows drawing from the thread that opened the window.

The grid keeps collision checks fast. Checking every pair of circles would mean about 50 million checks per frame at 10,000 circles. With the grid, each circle only looks at a handful of neighbours.

## OOP and DOD

In the OOP version every circle is its own object on the heap. The simulation keeps a list of pointers to them and calls virtual methods such as `movement` and `checkColision` on each one.

In the DOD version there are no circle objects. Positions, directions, infection flags and timers each live in their own array, and circle number `i` is simply index `i` in every array. The loops run straight through those arrays, which is much friendlier to the CPU cache.

The two versions also handle memory differently, which shows up on screen when the virus is on. OOP deletes each circle when it dies, so its memory reading goes down. DOD allocates its arrays once at their full size. When a circle dies, the last living circle is copied into its slot and the count shrinks, but no memory is given back until the next restart, so the reading stays flat.

The memory numbers are not counted the same way either. OOP tracks every C++ allocation in the program, including containers and strings. DOD only tracks the arrays it allocates itself.

## Tasks

Alecse Victor-Alin: FPS Counter and Memory Usage, OOP/DOD Switch, Concurrency

Peiculeasa Sergiu-Ștefan: Graphics, CLI Runtime Modifications, Obstacles

Mînăscurtă Victor: Virus spread logic, Readme, Additional aid

Cooordinated using git and CLion Code With Me.
