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

## Notes

The zoom is clamped at the origin when zooming in.
The zoom is also only controlled by up and down scroll events.