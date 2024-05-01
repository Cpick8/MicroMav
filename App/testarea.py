import customtkinter
import requests
import threading

url = "http://192.168.4.1:80"

customtkinter.set_appearance_mode("System")  # Modes: system (default), light, dark
customtkinter.set_default_color_theme("blue")  # Themes: blue (default), dark-blue, green

app = customtkinter.CTk()  # create CTk window like you do with the Tk window
app.geometry("400x240")

def button_function():
    def worker():
        while True:
            try:
                response = requests.get(url)
                if response.status_code == 200:
                    json_data = response.json()
                    rotation_x = json_data.get('Rotation', {}).get('x', 'N/A')
                    label.configure(text=f"Rotation X: {rotation_x}")
                    app.update()  # Force update the GUI
                else:
                    print("Failed to retrieve data. Status code:", response.status_code)
            except requests.exceptions.RequestException as e:
                print("Error:", e)

    threading.Thread(target=worker, daemon=True).start()

# Use CTkButton instead of tkinter Button
button = customtkinter.CTkButton(master=app, text="CTkButton", command=button_function)
button.place(relx=0.5, rely=0.5, anchor=customtkinter.CENTER)

label = customtkinter.CTkLabel(master=app, text="")
label.place(relx=0.5, rely=0.2, anchor=customtkinter.CENTER)

app.mainloop()