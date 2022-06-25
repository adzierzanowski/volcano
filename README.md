# Modecom Volcano Lanparty RGB keyboard UNIX userland driver

This is a driver for the Modecom Volcano Lanparty RGB keyboard. I made it
because the only available drivers are made for Windows.

The driver also works on Linux (tested on Kubuntu 20.04).

It operates in the userland so the only requirement is to have `sudo`
permissions.

The driver consists of:

* The keyboard handling library: `src/keyboard.h`
* The CLI frontend: `src/main.c` -> `volcano`
* The daemon which is capable of listening to the hotplug events: `src/daemon.c` -> `volcanod`
* The web control panel for communicating with the daemon: `src/srv.c` -> `volcanosrv`

The daemon and the web panel are of course optional and you can control the
keyboard using just the main binary `volcano`. Nevertheless, the daemon is
a little bit more powerful and easier to use.

#### Screenshot of the web control panel

![](https://i.imgur.com/cwjAgDg.png)

**Table of contents**

- [Modecom Volcano Lanparty RGB keyboard UNIX userland driver](#modecom-volcano-lanparty-rgb-keyboard-unix-userland-driver)
      - [Screenshot of the web control panel](#screenshot-of-the-web-control-panel)
- [Building](#building)
  - [macOS](#macos)
  - [Ubuntu](#ubuntu)
  - [Tools](#tools)
- [Creating keymaps](#creating-keymaps)
- [Daemon](#daemon)
  - [Installation](#installation)
    - [Build](#build)
    - [Configuration file](#configuration-file)
    - [Installing the daemon: macOS launchctl](#installing-the-daemon-macos-launchctl)
    - [Installing the daemon: Linux](#installing-the-daemon-linux)
  - [Daemon Socket Commands](#daemon-socket-commands)
- [Main executable usage](#main-executable-usage)
- [Development](#development)
  - [Dependecies](#dependecies)
  - [Development status](#development-status)
    - [Implemented things](#implemented-things)
    - [TODO](#todo)
  - [Other files](#other-files)
  - [Contributing](#contributing)

# Building

## macOS

Install `libusb` 1.0. For example:

```bash
$ brew install libusb
```

If you've installed `libusb` in another way, you might need to modify the
`Makefile` to link against this library.


Finally run `make`:

```bash
$ make
```

## Ubuntu

Install `libusb` 1.0.

```bash
$ sudo apt-get install libusb-1.0-0 libusb-1.0-0-dev
```

Run `make`:

```bash
$ make
```

## Tools

If you wish to use included tools, you'll need `python3` and install the
requirements:

```bash
$ python3 -m pip install -r requirements.txt
```


# Creating keymaps

You can easily create keyboard mappings through the web control panel of the
daemon.

Another way to create a keymapping is to convert a YAML file to binary file and
then load it using `volcano` or `volcanod` (see *Daemon* section).

```bash
$ python3 tools/mkmap.py sample-keymap.yml kmap.dat
$ sudo volcano -M kmap.dat
```

# Daemon

To make the driver's work seamless it's best to use the daemon which listens
for commands in background and reacts to hotplugging of the keyboard.

## Installation

### Build

Clone the repository and build it:

```bash
$ git clone https://github.com/adzierzanowski/volcano.git /Users/user/volcano
$ cd /Users/user/volcano
$ make
```

The resulting executables will be created in the `./bin` folder:
* `volcano` - CLI for controlling the keyboard without the daemon running
* `volcanod` - the daemon executable
* `volcanosrv` - the web control panel server communicating with the daemon
* `volcanoctl` - a shell script which opens the control panel (assuming it uses the default port `65226`)

### Configuration file

Create `.volcanorc` file with basic configuration:

```conf
LOG_LEVEL=1
LOG_COLOR=0
SOCKET_FILE=/Users/user/.volcano.sock
SOCKET_UID=501
SOCKET_GID=20
KMAP_FILE=/Users/user/kmap.dat
INIT_MODE=ripple
INIT_COLOR=00ffff
SRV_ENABLE=1
SRV_PORT=65226
SRV_DATA=/Users/user/volcano/www
SRV_EXE=/Users/user/volcano/bin/volcanosrv
```

| Key           | Description                                                  | Default                         |
|---------------|--------------------------------------------------------------|---------------------------------|
| `LOG_LEVEL`   | Logging verbosity (`0`=ERROR, `1`=WARN, `2`=INFO, `3`=DEBUG, `4`=SILLY) | `3`                  |
| `LOG_COLOR`   | Boolean flag. If set, the logs are colored with ANSI codes   | `0`                             |
| `SOCKET_FILE` | Socket file path for inter-process communication             | `$HOME/.volcano.sock`           |
| `SOCKET_UID`  | Socket owner user ID (should be you)                         | `501` on macOS, `1000` on Linux |
| `SOCKET_GID`  | Socket owner group ID                                        | `20` on macOS, `1000` on Linux  |
| `KMAP_FILE`   | Keymap file loaded on keyboard hotplug                       | None                            |
| `INIT_MODE`   | Initial color scheme after plugging                          | `norm`                          |
| `INIT_COLOR`  | Initial color                                                | `0xffffff`                      |
| `SRV_ENABLE`  | Enable locally hosted www control panel                      | `1`                             |
| `SRV_PORT`    | Port at which the control panel is server                    | `65226`                         |
| `SRV_DATA`    | Path to the control panel assets                             | `$BINPATH/www`*                 |

\* The path of the running executable

### Installing the daemon: macOS launchctl

Next, create `volcanod.plist` file in `/Library/LaunchDaemons` folder:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>Label</key>
  <string>volcanod</string>
  <key>ProgramArguments</key>
  <array>
    <string><!--PATH TO VOLCANOD EXECUTABLE--></string>
    <string><!--PATH TO .VOLCANORC FILE--></string>
  </array>
  <key>KeepAlive</key>
  <true />
  <key>StandardOutPath</key>
  <string>/var/log/volcanod.log</string>
  <key>StandardErrorPath</key>
  <string>/var/log/volcanod.log</string>
  <key>Debug</key>
  <true />
  <key>RunAtLoad</key>
  <true />
</dict>
</plist>
```

To enable the daemon

```bash
$ sudo launchctl load /Library/LaunchDaemons/volcanod.plist
```

To disable

```bash
$ sudo launchctl unload /Library/LaunchDaemons/volcanod.plist
```

### Installing the daemon: Linux

Well, you'll have to find out how to install daemons on Linux yourself.

## Daemon Socket Commands

| Command                  | Description         | Argument range                                                      |
|--------------------------|---------------------|---------------------------------------------------------------------|
| `kmap KMAP[172]`         | map keys            | 172 numbers from 0 to 255                                           |
| `fkmap [FILE]`           | map keys            | a kmap file path                                                    |
| `mode MODE`              | set color mode      | mode name, see `volcano -m list`                                    |
| `color [R [G [B]]]`      | set color           | 0 <= `R`, `G`, `B` <= 255                                           |
| `kcolor KEY [R [G [B]]]` | set color           | KEY - see `sample-kmap.yml` for reference, 0 <= `R`, `G`, `B` < 256 |
| `speed LEVEL`            | animation speed     | 0 <= `LEVEL` <= 4                                                   |
| `brightness LEVEL`       | brightness level    | 0 <= `LEVEL` <= 4                                                   |
| `dir DIR`                | animation direction | 0 <= `DIR` <= 1                                                     |
| `rate RATE`              | USB report rate     | `RATE` in (125, 250, 500, 1000)                                     |

# Main executable usage

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

# Development

## Dependecies

* libusb 1.0
* pyyaml

## Development status

It's almost complete.

Some refactoring is certainly needed.

To sniff USB communication between the keyboard and the original driver I used
[Device Monitoring Studio](https://www.hhdsoftware.com/device-monitoring-studio).

### Implemented things

* setting single keys to a specified color in "Custom" mode
* setting the whole keyboard color in supported modes
* clearing the whole keyboard
* switching between color schemes
* self-served driver control panel
* key mappings
* updating settings on PnP

### TODO

* defining macros
* setting the gradient in one of the color modes

## Other files

The repository contains `pck` folder which contains the packets sniffed during
the development. It's not needed for the driver to run properly. Some dev tools
in `tools` folder use it, though.

`map` folder contains packets which represents key mapping. These are in
plain ASCII hexadecimal format.

## Contributing

Please make a pull request if you've managed to get a grip of the commands.
I'll be grateful.
