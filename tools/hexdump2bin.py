#!/usr/bin/env python3

import re
import sys

with open(sys.argv[1], 'r') as f:
  data = [int(l, 16) for l in re.split(r'\s', f.read())  if l != '']

with open(sys.argv[2], 'wb') as f:
  f.write(bytearray(data))
