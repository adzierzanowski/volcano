
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
