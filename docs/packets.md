
# Sending commands

Each packet's data size is *usually* 64 bytes.

```
HOST -- control transfer (commands) --> DEVICE
DEVICE -- bulk transfer (acknowledgement) --> HOST
```

#### Control transfer setup packet

| Request type | Request | Value    | Index (iface number) |
|--------------|---------|----------|----------------------|
| `0x21`       | `0x09`  | `0x0204` | `0x01`               |

# Receiving data

The host must receive 64 bytes via USB bulk transfer (at endpoint `0x82`, interface `1`).
The received data is the same except the 8-th byte which is set to `1` to
acknlowledge the successful transfer.

# Data format

* `04 01 00 01` is the **start sequence** of each transmission (consisting of several commands)
* `04 02 00 02` is the **end sequence** of each transmission (consisting of several commands)

These are followed by `00`s until 64-byte size is reached.

## Setting "Normally on" LED scheme

```
04 0d 00 06 01 00 00 00 06
```

### Setting the color of the scheme (and some of the other schemes)

```
# notice the order
04 0d 01 06 03 05 00 00 R B G
```

## Brightness control

```
04 33 00 06 01 2b 00 00 BRIGHTNESS
```

where `BRIGHTNESS` is level from `0` to `4`

## Speed control

```
04 36 00 06 01 2c 00 00 SPEED
```

where `SPEED` is level from `0` to `4` (the lower level the higher the speed)

## Rainbow

TODO: check

```
04 36 00 06 01 2e 00 01 = on
04 35 00 06 01 2e       = off
```

## Paint a single key

TODO: fill

# Key mapping

There are 8 packets sent (not counting start and end)

The number immediately to the left of the keycode is the key mode.
E.g.: `LSHIFT` has `01` on the left and non-modal keys have `02`
instead.

```
START PACKET
04 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

KEYMAP COMMAND
04 2a 01 0a 10 00 00 00 aa 55 10 00 00 00 01 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

MAPPING 1 (with corresponding keys)
04  d4 02 08   38 00 00     00 02 02    ESC 02   02 BACKTICK 02 02
TAB 02 02 CAPS 02 01 LSHIFT 02 01 LCTRL 02  02   F1 02       02 K1
02  02 Q  02   02 A  02     02 64 02    01  META 02 02       F2 02
02  K2 02 02   W  02 02     S  02 02    Z   02   01 LALT     02 02

MAPPING 2
04 13 03 08 38 38 00 00 3c 02 02 20 02 02 08 02
02 07 02 02 1b 02 02 2c 02 02 3d 02 02 21 02 02
15 02 02 09 02 02 06 02 01 40 02 02 3e 02 02 22
02 02 17 02 02 0a 02 02 19 05 02 02 02 02 3f 02

MAPPING 3
04 ce 03 08 38 70 00 00 02 23 02 02 1c 02 02 0b
02 02 05 02 02 65 02 02 40 02 02 24 02 02 18 02
02 0d 02 02 11 02 01 80 02 02 41 02 02 25 02 02
0c 02 02 0e 02 02 10 02 01 10 02 02 42 02 02 26

MAPPING 4
04 eb 03 08 38 a8 00 00 02 02 12 02 02 0f 02 02
36 02 00 00 02 02 43 02 02 27 02 02 13 02 02 33
02 02 37 02 00 00 02 02 44 02 02 2d 02 02 2f 02
02 34 02 02 38 02 00 00 02 02 45 02 02 2e 02 02

MAPPING 6
04 c0 04 08 38 e0 00 00 30 02 00 00 02 02 87 02
00 00 02 00 00 02 02 2a 02 02 31 02 02 28 02 01
20 02 00 00 02 02 46 02 02 49 02 02 4c 02 00 00
02 00 00 02 02 50 02 02 47 02 02 4a 02 02 4d 02

MAPPING 7
04 e2 04 08 38 18 01 00 00 00 02 02 52 02 02 51
02 02 48 02 02 4b 02 02 4e 02 00 00 02 00 00 02
02 4f 02 00 00 02 02 53 02 02 5f 02 02 5c 02 02
59 02 00 00 02 00 00 02 02 54 02 02 60 02 02 5d

THIS PACKET DOESN'T CHANGE
04 65 05 08 2a 50 01 00 02 02 5a 02 02 62 02 00
00 02 02 55 02 02 61 02 02 5e 02 02 5b 02 02 63
02 00 00 02 02 56 02 02 57 02 00 00 02 02 58 02
00 00 00 00 02 00 00 02 02 54 02 02 60 02 02 5d

END PACKET
04 02 00 02 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
```
