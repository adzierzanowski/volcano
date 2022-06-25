# This script filters out the busdog output into a hexdump
# containing only the outgoing messages

import sys


if len(sys.argv) < 3:
  print('Need 2 args.')
  sys.exit(1)

with open(sys.argv[1]) as f:
  data = [l for l in f.read().split('\n') if l!='']
  out = ''

  for line in data:
    sline = line.split('\t')
    print(sline)
    if sline[1].startswith('Out'):
      out += line.split('\t')[-2] + '\n'

  with open(sys.argv[2], 'w') as f:
    f.write(out)
