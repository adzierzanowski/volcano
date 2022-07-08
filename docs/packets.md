# Modecom Volcano Lanparty RGB data format

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

These are followed by `00`s until 64-byte size is reached. The same notation
as above is used further.


# Set single key color (only in custom mode)

```
04 K1 00 11 03 K5 K6 00 R B G
```

Where `K1`, `K5` and `K6` define a key to be colored.
The keycodes are stored in `vlc_kcolor_keys` structure in
[keyboard.c](../src/keyboard.c) file in the following format

```c
.val = (K1 << 16) | (K5 << 8) | K6;
```

# Set color for the entire keyboard

```
04 0d 01 06 03 05 00 00 R B G
```

# Set rainbow mode

Some modes can display a rainbow instead of a still color.

```
04 0c 00 06 01 04 00 00 ON
```

Where `ON` is `0` or `1`.

# Set color mode

```
04 M1 00 06 01 00 00 00 M2
```

Where `M1` is the id of the mode and `M2` is `M1 - 7`.
Mode ids are available in `enum vlc_kbd_mode_t` in [keyboard.h](../src/keyboard.h).

# Remap keys

```
04 d4 02 08 38 00 00 00 ESC ESC ESC ~  ~  ~  TAB TAB TAB CAPS CAPS CAPS  LSHIFT LSHIFT LSHIFT LCTRL  LCTRL  LCTRL F1 F1    F1    K1    K1     K1     Q      Q   Q   A   A   A   02  02   64   META META META F2  F2 F2 K2 K2 K2 W  W  W  S  S   S     Z     Z     Z   LALT LALT LALT F3  F3
04 13 03 08 38 38 00 00 F3  K3  K3  K3 E  E  E   D   D   D    X    X     X      SPACE  SPACE  SPACE  F4     F4    F4 K4    K4    K4    R      R      R      F   F   F   C   C   C   RALT RALT RALT F5   F5   F5  K5 K5 K5 T  T  T  G  G  G  V   V     V     05    02  02   F7   F7   F7  K6
04 ce 03 08 28 70 00 00 K6  K6  Y   Y  Y  H  H   H   B   B    B    MENU  MENU   MENU   F7     F7     F7     K7    K7 K7    U     U     U      J      J      J   N   N   N   02  01  80   F8   F8   F8   K8   K8  K8 I  I  I  K  K  K  M  M  M   RCTRL RCTRL RCTRL F9  F9   F9   K9   K9  K9
04 eb 03 08 38 a8 00 00 O   O   O   L  L  L  ,   ,   ,   02   00   00    F10    F10    F10    K0     K0     K0    P  P     P     ;     ;      ;      .      .   .   02  00  00  F11 F11  F11  -    -    -    [   [  [  '  '  '  /  /  /  02 00  00    F12   F12   F12 +    +    +    ]   ]
04 c0 04 08 38 e0 00 00 ]   02  00  00 02 02 87  02  00  00   02   00    00     BCKSPC BCKSPC BCKSPC \      \     \  ENTER ENTER ENTER RSHIFT RSHIFT RSHIFT 02  00  00  PS  PS  PS  INS  INS  INS  DEL  DEL  DEL 02 00 00 02 00 00 ←  ←  ←  SL  SL    SL    HM    HM  HM   END  END  END 02
04 e2 04 08 38 18 01 00 00  00  ↑   ↑  ↑  ↓  ↓   ↓   PB  PB   PB   PU    PU     PU     PD     PD     PD     02    00 00    02    00    00     →      →      →   02  00  00  02  02  53   02   02   5f   02   02  5c 02 02 59 02 00 00 02 00 00  02    02    54    02  02   60   02   02  5d
04 65 05 08 2a 50 01 00 02  02  5a  02 02 62 02  00  00  02   02   55    02     02     61     02     02     5e    02 02    5b    02    02     63     02     00  00  02  02  56  02  02   57   02   00   00   02  02 58 02 00 00 00 00 02 00 00  02    02    54    02  02   60   02   02  5d
```

The first 8 bytes in each packet are command bytes. The remaining 56 bytes are
the keymapping payload. Bytes 5 and 6 are the offset of the current chunk of
the payload (multiple of 56, 16-bit, little-endian). `08 38` is the keymap
command, `08 2a` probably means that this is the last packet of the keymapping.

I have no idea what bytes 1 and 2 mean. If I send `00`s instead, this works
just fine so whatever.

Each key named here is the position of the original key. Each key is given
3 bytes:

```
MACRO MODIFIER KEYCODE
```

`MACRO` is equal to `05` if the key should fire the macro. `02` if it functions
as a normal key.

`MODIFIER` is equal to `01` if the key is a modifier key (like SHIFT or CTRL).
`02` if it's a normal key (like Q or F5). USB scancodes of modifier keys are
in some cases same as usual key's scancodes, hence the need for differentiation.

`KEYCODE` is a USB scancode.

If the key is set to a macro, the `MODIFIER` is equal to `01` and `KEYCODE` is
0-based number of the macro (more on that later).

In the packets that are being sent to the keyboard there are some padding bytes
`02 00 00` and some definitions for keys which are not present on my keyboard,
e.g. `02 02 53` - NUMLOCK. Probably the manufacturer uses the same chip in some
other keyboards. In my case, these can be left set to `00` but I present them
here in case someone finds it useful.

# Set brightness

```
04 L1 00 06 01 01 00 00 L2
```

Where `L2` is the level (0-4) and `L1` is `L2 + 8`.

# Set speed (of the animation)

```
04 L1 00 06 01 02 00 00 L2
```

Where `L2` is `4 - level` (level is in range 0-4) and `L1` is `13 - level`.

# Set USB report rate

```
04 R1 00 06 01 0f 00 00 R2
```

Where `R2` is the report rate (0 is 125 Hz, 1 is 250 Hz, 2 is 500 Hz and 3 is 1000 Hz)
and `R1` is `22 + R2`.
# Set direction

```
04 D1 00 06 01 03 00 00 D2
```

Where `D2` is the direction (0 or 1) and `D1` is `10 + D2`.

# Set gradient

```
04 G1 00 06 01 11 00 00 G2
```

`G2` is the gradient (0-3), `G1 = 0x18 + G2`.

# Macros

Macros are similar to keymapping in regard of the command-payload pattern.

```
04 b1 07 0a 38 00 00 00 PAYLOAD[56]
```
