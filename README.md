# Modecom Volcano LAN party macOS "driver"

This is an attempt to modify keyboard state (colors, macros) using macOS because
the only available drivers are made for Windows.

I'm not really sure but it should also work on Linux.

# Development status

The communication with the keyboard works just fine but there are many commands
left to decipher.

To sniff USB communication with the original driver I use [Device Monitoring
Studio](https://www.hhdsoftware.com/device-monitoring-studio).

Implemented things:

* setting single keys to a specified color in "Custom" mode
* setting the whole keyboard color in supported modes
* clearing the whole keyboard
* switching between color schemes
* GUI in Tkinter
* key mappings

As for now, you must switch keyboard to the specific mode in order to use
that mode's functions. In the future the driver will switch the modes
automatically.

To implement (and to check if it's possible):

* macros
* defining custom color schemes
* updating settings on PnP

# Building

Install `libusb` 1.0. For example on macOS:

```
$ brew install libusb
```

If you've installed `libusb` in another way, you might need to modify the
`Makefile` to link against this library.


Finally run `make`:

```
$ make
```

If you wish to use included tools, you'll need `python3` and install the
requirements:

```
$ python3 -m pip install -r requirements.txt
```

# Usage

```
OPTIONS:
    -B (0-4)       set brightness (0-4)
    -D (0-1)       set direction (0-1)
    -S (0-4)       set speed
    -R (0-1)       set rainbow
    -H (0-3)       set report rate (0-3; 125 Hz - 1000 Hz)

    -m MODE        select color mode ('list' to list all modes)
    -M FILE        remap keyboard keys
    -c             set color for entire keyboard
    -C CMD         pass a command
    -h             show this help message and exit
    -k KEY         select a key for the color change
    -r (0-255)     red value
    -g (0-255)     green value
    -b (0-255)     blue value

EXAMPLES:
    volcano -k BACKSPACE -r 255 -g 0 -b 0      # set backspace to red
    volcano -c -g 255              # set the entire keyboard to green
    volcano -C clear                      # clear the entire keyboard
    volcano -C clear-m1    # clear the entire keyboard in custom mode
```

## Key mapping

Create keymapping in yml file and convert it to binary file. Then load it
using `volcano`.

```
$ python3 tools/mkmap.py sample-keymap.yml kmap.dat
$ sudo volcano -M kmap.dat
```

# Packets

If you compile `packets_cli.c` (`make packets`) you'll get an executable which
takes filenames as arguments. Each file is a packet file.

Packets are just binary files (from 1 to 64 bytes in size) which are directly
sent to the keyboard as a control transfer.

To make such packet, you can click "export binary" in the DMS URB view or
use some hex editor.

eg.

```bash
$ packets pck/start pck/norm-on pck/end
```

---

Of course you can modify the code directly and use

```c
uint8_t start[64] = { 0x04, 0x01, 0x00, 0x01 };
kbd_send_and_recv(kbdh, start);
```

or

```c
kbd_va_send_and_recv(kbdh, 4, 0x04, 0x01, 0x00, 0x01);
```

# Other files

The repository contains `pck` folder which contains the packets sniffed during
the development. It's not needed for the driver to run properly. Some dev tools
in `tools` folder use it, though.

`map` folder contains packets which represents key mapping. These are in
plain ASCII hexadecimal format.

# Contributing

Please make a pull request if you've managed to get a grip of the commands.
I'll be grateful.

# Dependecies

* libusb 1.0
* pyyaml
