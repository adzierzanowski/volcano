import os
import struct

keys = []
for fname in os.listdir('pck/keys'):
  with open(f'pck/keys/{fname}', 'rb') as f:
    data = f.read()

    d = struct.unpack('BBBBBBB', data[:7])

    k = (fname[:-6].upper(), d[0], d[1], d[2], d[3], d[4], d[5], d[6])
    (
      f'{fname[:-6].upper():>20}  ',
      f'{d[0]:02x}',
      f'{d[1]:02x}',
      f'{d[2]:02x}',
      f'{d[3]:02x}',
      f'{d[4]:02x}',
      f'{d[5]:02x}',
      f'{d[6]:02x}')
    keys.append(k)

for k in sorted(keys, key=lambda k: k[1]+k[-1]*1000):
  kname, d0, d1, d2, d3, d4, d5, d6 = k
  print(
    f'{kname:>20}  ',
    f'{d0:02x}',
    f'{d1:02x}',
    f'{d2:02x}',
    f'{d3:02x}',
    f'{d4:02x}',
    f'{d5:02x}',
    f'{d6:02x}')
  #print(f'#define KEY_{kname} 0x{d0:02x}{d1:02x}{d2:02x}')
