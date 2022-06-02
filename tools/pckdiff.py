d1 = 'map-menu-to-alt'
d2 = 'map-menu-to-win'
PCK_PATH = 'pck/mappings/'


data1 = b''
data2 = b''
for i in range(1, 9):
  with open(f'{PCK_PATH}{d1}-{i}', 'rb') as f:
    data1 += f.read()

  with open(f'{PCK_PATH}{d2}-{i}', 'rb') as f:
    data2 += f.read()


for i, (b1, b2) in enumerate(zip(data1, data2)):
  if b1 != b2:
    print(i, b1, b2)
