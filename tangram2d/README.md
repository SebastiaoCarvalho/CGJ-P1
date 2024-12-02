# Tangram 2D

## Install dependencies

For Ubuntu (other linux distributions have similar comands):

```bash
sudo apt-get install libassimp-dev
sudo apt-get install libglm-dev
sudo apt-get install libglew-dev
sudo apt-get install libglfw3-dev
```

## Build

### Using CMake

```bash
mkdir build
cd build
cmake ..
make run
```

### Using Makefile

```bash
make clean debug run
```
Or
```bash
make clean release run
```