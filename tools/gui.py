import tkinter as tk

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
  def __init__(self, root):
    self.root = root
    self.root.wm_title('Volcano')
    self.root.geometry('1080x768')

    for kname, key in KEYS.items():
      def cmd(*args, name=None):
        print(name, self.r.get(), self.g.get(), self.b.get())
        subprocess.run((
          'sudo', VOLCANO_EXE,
          '-k', name,
          '-r', str(self.r.get()),
          '-g', str(self.g.get()),
          '-b', str(self.b.get())))


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

    self.clear_all_btn = tk.Button(text='Clear All', command=self.clear_all)
    self.clear_all_btn.place(x=35, y=480)

    self.set_color_btn = tk.Button(text='Set Color', command=self.set_color)
    self.set_color_btn.place(x=135, y=480)

    self.modes_var = tk.StringVar(value=COLOR_SCHEME_KEYS)
    self.color_mode_listbox = tk.Listbox(root, listvariable=self.modes_var)
    self.color_mode_listbox.place(x=465, y=500)
    self.color_mode_listbox.bind('<<ListboxSelect>>', self.on_listbox_select)

  def on_listbox_select(self, e):
    sel = self.color_mode_listbox.curselection()
    mode = COLOR_SCHEMES.get(COLOR_SCHEME_KEYS[sel[0]])
    subprocess.run(('sudo', VOLCANO_EXE, '-m', mode))

  def set_color(self):
    subprocess.run((
      'sudo', VOLCANO_EXE,
      '-c',
      '-r', str(self.r.get()),
      '-g', str(self.g.get()),
      '-b', str(self.b.get())))

  def clear_all(self):
    subprocess.run(('sudo', VOLCANO_EXE, '-C', 'clear-m1'))


root = tk.Tk()
gui = GUI(root)
root.mainloop()
