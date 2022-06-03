import sys
import yaml
from keycodes import KEYCODES


with open(sys.argv[1], 'r') as f:
  usermap = yaml.load(f, Loader=yaml.SafeLoader)

print(usermap)

print(KEYCODES.keys())

keys = (
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
  'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'K1', 'K2', 'K3', 'K4',
  'K5', 'K6', 'K7', 'K8', 'K9', 'K0', 'ENTER', 'ESC', 'BACKSPACE', 'TAB',
  'SPACE', 'MINUS', 'PLUS', 'LBRACKET', 'RBRACKET', 'BACKSLASH', 'COLON', 'AP',
  'TILDE', 'COMMA', 'DOT', 'SLASH', 'CAPS', 'F1', 'F2', 'F3', 'F4', 'F5', 'F6',
  'F7', 'F8', 'F9', 'F10', 'F11', 'F12', 'PS', 'SL', 'PB', 'INS', 'HM', 'PU',
  'DEL', 'END', 'PD', 'RIGHT', 'LEFT', 'DOWN', 'UP', 'LSHIFT', 'RSHIFT', 'LALT',
  'RALT', 'LCTRL', 'RCTRL', 'META', 'MENU')

modal_keys = ('LSHIFT', 'RSHIFT', 'LALT', 'RALT', 'LCTRL', 'RCTRL', 'META')

data = b''
for key in keys:
  if key in usermap:
    if usermap[key] in KEYCODES:
      data += KEYCODES[usermap[key]].to_bytes(1, 'little')
    else:
      data += usermap[key].to_bytes(1, 'little')
  else:
    data += KEYCODES[key].to_bytes(1, 'little')

for key in keys:
  k = None

  if key in usermap:
    k = usermap[key]
  else:
    k = key

  if k in modal_keys:
    data += b'\x01'
  else:
    data += b'\x02'


with open(sys.argv[2], 'wb') as f:
  f.write(data)
