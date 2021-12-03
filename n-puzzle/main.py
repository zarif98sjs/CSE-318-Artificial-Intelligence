import tkinter as tk

H = 500
W = 500

def create_grid(event=None):
    w = c.winfo_width()  # Get current width of canvas
    h = c.winfo_height()  # Get current height of canvas
    c.delete('grid_line')  # Will only remove the grid_line

    # Creates all vertical lines at intevals of w/n
    n = 4
    for i in range(0, w, int(w / n)):
        c.create_line([(i, 0), (i, h)], tag='grid_line')

    # Creates all horizontal lines at intevals of h/n
    for i in range(0, h, int(h / n)):
        c.create_line([(0, i), (w, i)], tag='grid_line')


root = tk.Tk()

c = tk.Canvas(root, height=H, width=W, bg='white')
c.pack(fill=tk.BOTH, expand=True)

c.create_text(50, 50, fill="darkblue", font="Times 20 italic bold",
              text="1")

c.bind('<Configure>', create_grid)

root.mainloop()
