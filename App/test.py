import tkinter as tk
from PIL import Image, ImageTk

def load_image():
    # Open the image file
    image = Image.open("py\image.png")

    # Convert the image to a format compatible with tkinter
    photo = ImageTk.PhotoImage(image)

    # Display the image on a label widget
    label = tk.Label(root, image=photo)
    label.image = photo  # Keep a reference to avoid garbage collection
    label.pack()

# Create a tkinter window
root = tk.Tk()
root.title("Image Viewer")

# Load and display the image
load_image()

# Start the tkinter event loop
root.mainloop()