import tkinter as tk

from keys import KEYS
import subprocess

VOLCANO_EXE = './volcano'

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
