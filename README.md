# Low level 2048 game implementation with SDL2

![Demo](./demo.gif)

## Try it out

### Prerequisites

Build tools:
- gcc ≥ `10.2`
- make ≥ `4.3`
- cmake ≥ `3.18`

Libraries:
- sdl2 ≥ `2.0.12`
- sdl2_gfx ≥ `1.0.4`
- sdl2_ttf ≥ `2.0.15`

### Compilation
This project uses `cmake` for build proccess. To compile the source code, run following commands:
```bash
git submodule update --init --recursive
cmake CMakeLists.txt
make
```

### Executing
This project is targeted for `x86_64` *GNU/Linux*. While C code should be portable, parts written in assembly are not.
```bash
./asm2048
```