# Modecom Volcano LAN party macOS "driver"

This is an attempt to modify keyboard state (colors, macros) using macOS because
the only available drivers are made for Windows.

I'm not really sure but it should also work on Linux.

# Development status

The communication with the keyboard works just fine but there are many commands
to decipher. As for now I've managed to change the backlight color in the
simplest "Normally on" LED sequence but it seems that the commands are dependent
on current keyboard state which makes it harder to reverse engineer.

To sniff USB communication with the original driver I use [Device Monitoring
Studio](https://www.hhdsoftware.com/device-monitoring-studio).

# Usage

The arguments to the main program are filenames with binary packets.

```bash
# Set "Normally on" theme
$ sudo ./main pck/start pck/norm-on pck/end

# Set predefined color
$ sudo ./main pck/start pck/col0 pck/col1 pck/end
```

Of course this is just for the development stage.

# Contributing

Please make a pull request if you've managed to get a grip of the commands.
I'll be grateful.

# Dependecies

* libusb 1.0
