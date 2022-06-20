from socket import socket, AF_UNIX, SOCK_STREAM
import tkinter as tk
from pathlib import Path

from keys import KEYS
import subprocess

VOLCANO_EXE = './volcano'

COLOR_SCHEMES = {
  'Normal': 'norm',
  'Custom': 'custom',
  'Stream': 'stream',
  'Clouds': 'clouds',
  'Swirl': 'swirl',
  'Rainbow Breathing': 'rgb-breath',
  'Breathing': 'breath',
  'Hotmap': 'hotmap',
  'Ripple': 'ripple',
  'Ripple lines': 'ripple-lines',
  'Snow': 'snow',
  'Rainbow Dots': 'rgb-dots',
  'Rainbow Lines': 'rgb-lines',
  'Triangular Waves': 'triangular',
  'Drain': 'drain',
  'Matrix': 'matrix',
  'Scanline': 'scanline',
  'Gradient': 'gradient',
  'Rainbow Circles': 'rgb-circles'
}

COLOR_SCHEME_KEYS = tuple(COLOR_SCHEMES.keys())


class GUI:
  def __init__(self, root, config):
    self.root = root
    self.config = config
    self.root.wm_title('Volcano')
    self.root.geometry('1080x768')

    for kname, key in KEYS.items():
      def cmd(*args, name=None):
        self.send(f'kcolor {name} {self.r.get()} {self.g.get()} {self.b.get()}')

      label = key.get('label', kname)
      btn = tk.Button(root, text=label, command=lambda kname=kname: cmd(name=kname))
      w = key.get('width') if key.get('width') else 50
      btn.place(x=key['pos'][0]+35, y=key['pos'][1]+35, width=w, height=50)

    rl = tk.Label(text='Red')
    rl.place(x=35, y=400)
    self.r = tk.Scale(root, from_=0, to=255, orient=tk.HORIZONTAL)
    self.r.place(x=35, y=420, width=200)

    gl = tk.Label(text='Green')
    gl.place(x=250, y=400)
    self.g = tk.Scale(root, from_=0, to=255, orient=tk.HORIZONTAL)
    self.g.place(x=250, y=420, width=200)

    bl = tk.Label(text='Blue')
    bl.place(x=465, y=400)
    self.b = tk.Scale(root, from_=0, to=255, orient=tk.HORIZONTAL)
    self.b.place(x=465, y=420, width=200)

    tk.Label(text='Speed').place(x=35, y=480)
    self.speed = tk.Scale(root, from_=0, to=4, orient=tk.HORIZONTAL, command=self.on_speed_change)
    self.speed.place(x=35, y=500, width=200)

    tk.Label(text='Brightness').place(x=250, y=480)
    self.brightness = tk.Scale(root, from_=0, to=4, orient=tk.HORIZONTAL, command=self.on_brightness_change)
    self.brightness.place(x=250, y=500, width=200)

    tk.Label(text='Direction').place(x=465, y=480)

    self.set_color_btn = tk.Button(text='Set Color', command=self.set_color)
    self.direction = tk.Scale(root, from_=0, to=1, orient=tk.HORIZONTAL, command=self.on_dir_change)
    self.direction.place(x=465, y=500, width=200)

    self.set_color_btn.place(x=700, y=432)

    tk.Label(text='Color mode').place(x=860, y=470)
    self.modes_var = tk.StringVar(value=COLOR_SCHEME_KEYS)
    self.color_mode_listbox = tk.Listbox(root, listvariable=self.modes_var)
    self.color_mode_listbox.place(x=860, y=500)
    self.color_mode_listbox.bind('<<ListboxSelect>>', self.on_listbox_select)

  def on_speed_change(self, val):
    self.send(f'speed {val}')

  def on_brightness_change(self, val):
    self.send(f'brightness {val}')

  def on_dir_change(self, val):
    self.send(f'dir {val}')

  def send(self, cmd):
    s = socket(AF_UNIX, SOCK_STREAM)
    s.connect(self.config.get('SOCKET_FILE'))
    s.send(cmd.encode('ascii'))
    s.close()

  def on_listbox_select(self, e):
    sel = self.color_mode_listbox.curselection()
    mode = COLOR_SCHEMES.get(COLOR_SCHEME_KEYS[sel[0]])
    cmd = f'mode {mode}'
    self.send(cmd)

  def set_color(self):
    self.send(f'color {self.r.get()} {self.g.get()} {self.b.get()}')

if __name__ == '__main__':
  with open(Path('~/.volcanorc').expanduser()) as f:
    cfg = {l.split('=')[0]: l.split('=')[1] for l in f.read().split('\n') if l != ''}

  root = tk.Tk()
  gui = GUI(root, cfg)
  root.mainloop()
