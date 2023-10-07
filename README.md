# X Vertical Tiling

A simple program that allows you to move and resize the active window across multiple monitors in a vertical tiling fashion on X11.

## Build

```bash
# Install dependencies
sudo apt install gcc make libx11-dev libxrandr-dev

# Build
git clone https://github.com/f3rnnds/x-vertical-tiling.git
cd x-vertical-tiling
make
```

## Usage

```console
$ x-vertical-tiling
Usage: x-vertical-tiling [up|down]
Options:
  up    Move the active window up to the top half of the target monitor.
  down  Move the active window down to the bottom half of the target monitor.
```

## Deploy

On Ubuntu 22.04, you can deploy the program by adding a custom shortcut:

* Settings > Keyboard > View and Customize Shortcuts > Custom Shortcuts
* \+
  * Name: View split on top
  * Command: `/path/to/x-vertical-tiling up`
  * Shortcut: Ctrl+Super+Up
* \+
  * Name: View split on bottom
  * Command: `/path/to/x-vertical-tiling down`
  * Shortcut: Ctrl+Super+Down
