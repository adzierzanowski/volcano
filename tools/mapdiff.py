import sys

d1, d2 = sys.argv[1:3]

with open(d1, 'r') as f:
  data1 = f.read().split('\n')

with open(d2, 'r') as f:
  data2 = f.read().split('\n')

for l1, l2 in zip(data1, data2):
  for p, q in zip(l1.split(' '), l2.split(' ')):
    if p!=q:
      print(f'\x1b[38;5;1m\x1b[7m{p}\x1b[0m', end=' ')
    else:
      print(f'{p}', end=' ')
  print()
print()

for l1, l2 in zip(data1, data2):
  for p, q in zip(l1.split(' '), l2.split(' ')):
    if p!=q:
      print(f'\x1b[38;5;1m\x1b[7m{q}\x1b[0m', end=' ')
    else:
      print(f'{q}', end=' ')
  print()
