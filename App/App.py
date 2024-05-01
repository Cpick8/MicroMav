import customtkinter
from PIL import Image, ImageTk
import requests
import threading
url = "http://192.168.4.1:80"
connected = False
packet_loss = 0 

customtkinter.set_appearance_mode("System")  # Modes: system (default), light, dark
customtkinter.set_default_color_theme("blue")
project_font = 'Trebuchet MS'

app = customtkinter.CTk()  
app.geometry("1000x600")
app.title("MiniMav Downlink")

def Start_Stream():
    global connected
    
    if connected == True:
        return
    print("STARTING DOWNLINK ON: "+url)
    connected = True
    def link():
        global packet_loss
        button.configure(text=f"SEARCHING", fg_color="green", bg_color="green")
        app.update()
        while True:
            try:
                response = requests.get(url)#, timeout=0.1)
                if response.status_code == 200:
                    print("PACKET RECIEVED!")
                    json_data = response.json()
                    ## SET ROTATION ##
                    rotation_x = json_data.get('Rotation', {}).get('x', 'N/A')
                    rotation_y = json_data.get('Rotation', {}).get('y', 'N/A')
                    rotation_z = json_data.get('Rotation', {}).get('z', 'N/A')

                    rotation_x = f"{rotation_x:.9f}" if rotation_x != 'N/A' else rotation_x
                    rotation_y = f"{rotation_y:.9f}" if rotation_y != 'N/A' else rotation_y
                    rotation_z = f"{rotation_z:.9f}" if rotation_z != 'N/A' else rotation_z

                    rotX.configure(text=f"X: {rotation_x}")
                    rotY.configure(text=f"Y: {rotation_y}")
                    rotZ.configure(text=f"Z: {rotation_z}")
                    ## SET ACCEL ##
                    acceleration_x = json_data.get('Acceleration', {}).get('x', 'N/A')
                    acceleration_y = json_data.get('Acceleration', {}).get('y', 'N/A')
                    acceleration_z = json_data.get('Acceleration', {}).get('z', 'N/A')

                    acceleration_x = f"{acceleration_x:.9f}" if acceleration_x != 'N/A' else acceleration_x
                    acceleration_y = f"{acceleration_y:.9f}" if acceleration_y != 'N/A' else acceleration_y
                    acceleration_z = f"{acceleration_z:.9f}" if acceleration_z != 'N/A' else acceleration_z

                    aclX.configure(text=f"X: {acceleration_x}")
                    aclY.configure(text=f"Y: {acceleration_y}")
                    aclZ.configure(text=f"Z: {acceleration_z}")
                    ## SET MOTORS ##
                    MotorStatus = {}
                    MotorStatus["FR"] = json_data.get('MotorStatus', {}).get('FR', {})
                    MotorStatus["FL"] = json_data.get('MotorStatus', {}).get('FL', {})
                    MotorStatus["ML"] = json_data.get('MotorStatus', {}).get('ML', {})
                    MotorStatus["MR"] = json_data.get('MotorStatus', {}).get('MR', {})
                    MotorStatus["BL"] = json_data.get('MotorStatus', {}).get('BL', {})
                    MotorStatus["BR"] = json_data.get('MotorStatus', {}).get('BR', {})

                    FrontRight.configure(text=f"{MotorStatus['FR'].get('Speed', 'N/A')}")
                    FrontLeft.configure(text=f"{MotorStatus['FL'].get('Speed', 'N/A')}")
                    MiddleLeft.configure(text=f"{MotorStatus['ML'].get('Speed', 'N/A')}")
                    MiddleRight.configure(text=f"{MotorStatus['MR'].get('Speed', 'N/A')}")
                    BackLeft.configure(text=f"{MotorStatus['BL'].get('Speed', 'N/A')}")
                    BackRight.configure(text=f"{MotorStatus['BR'].get('Speed', 'N/A')}")
                    
                    ## SET EXTRAS ##
                    temperature = json_data.get('Temperature', 'N/A')
                    temperature = f"{temperature:.1f}" if temperature != 'N/A' else temperature
                    temperature = float(temperature)  # Convert temperature to float
                    if temperature < 37.7:
                        tempWarn = "green"
                    else:
                        tempWarn = "red"
                    Temp.configure(text=f"Temp: {temperature} °C", bg_color=tempWarn)

                    app.update()  # Force update the GUI
                else:
                    packet_loss += 1
                    print("Lost Packet! Status code:", response.status_code)
            except requests.exceptions.Timeout:
                packet_loss += 1
                print("Lost Packet! #"+str(packet_loss)+" !!Status code: Access Point Not Reachable!!")
                button.configure(text=f"DOWNLINK LOST", fg_color="red", bg_color="red")
                app.update()
            except requests.exceptions.RequestException as e:
                print("An error occurred:", e)

    threading.Thread(target=link, daemon=True).start()


# Load and display the image
#image_path = "py/image.png"
#image = Image.open(image_path)
#photo = ImageTk.PhotoImage(image)
#image_label = customtkinter.CTkLabel(master=app, image=photo, text="MINI-MAV")
#image_label.place(relx=0.5, rely=0.5, anchor=customtkinter.CENTER)

# CONNECT BUTTON
button = customtkinter.CTkButton(master=app, text="CONNECT", font=('OCR A',25), command=Start_Stream)
button.place(relx=0.5, rely=0.7, anchor=customtkinter.CENTER)

#### TELEM LABELS ####
rotTitle = customtkinter.CTkLabel(master=app, text="Rotation:", bg_color="transparent", font=(project_font,30))
rotTitle.place(relx=0.07, rely=0.04, anchor=customtkinter.CENTER)
rotX = customtkinter.CTkLabel(master=app, text="X: 0", width=700, bg_color="transparent", font=(project_font,20))
rotX.place(relx=0.1, rely=0.1, anchor=customtkinter.CENTER)
rotY = customtkinter.CTkLabel(master=app, text="Y: 0", bg_color="transparent", font=(project_font,20))
rotY.place(relx=0.1, rely=0.15, anchor=customtkinter.CENTER)
rotZ = customtkinter.CTkLabel(master=app, text="Z: 0", bg_color="transparent", font=(project_font,20))
rotZ.place(relx=0.1, rely=0.2, anchor=customtkinter.CENTER)

aclTitle = customtkinter.CTkLabel(master=app, text="Acceleration:", bg_color="transparent", font=(project_font,25))
aclTitle.place(relx=0.9, rely=0.05, anchor=customtkinter.CENTER)
aclX = customtkinter.CTkLabel(master=app, text="X: 0", width=700, bg_color="transparent", font=(project_font,20))
aclX.place(relx=0.8, rely=0.1, anchor=customtkinter.CENTER)
aclY = customtkinter.CTkLabel(master=app, text="Y: 0", bg_color="transparent", font=(project_font,20))
aclY.place(relx=0.8, rely=0.15, anchor=customtkinter.CENTER)
aclZ = customtkinter.CTkLabel(master=app, text="Z: 0", bg_color="transparent", font=(project_font,20))
aclZ.place(relx=0.8, rely=0.2, anchor=customtkinter.CENTER)
#### MOTOR LABELS ####
FrontRight = customtkinter.CTkLabel(master=app, text="0", bg_color="red", font=(project_font,25))
FrontRight.place(relx=0.605, rely=0.198, anchor=customtkinter.CENTER)
FrontLeft = customtkinter.CTkLabel(master=app, text="0", bg_color="red", font=(project_font,25))
FrontLeft.place(relx=0.395, rely=0.198, anchor=customtkinter.CENTER)

MiddleRight = customtkinter.CTkLabel(master=app, text="0", bg_color="red", font=(project_font,25))
MiddleRight.place(relx=0.71, rely=0.503, anchor=customtkinter.CENTER)
MiddleLeft = customtkinter.CTkLabel(master=app, text="0", bg_color="red", font=(project_font,25))
MiddleLeft.place(relx=0.29, rely=0.503, anchor=customtkinter.CENTER)

BackRight = customtkinter.CTkLabel(master=app, text="0", bg_color="red", font=(project_font,25))
BackRight.place(relx=0.605, rely=0.81, anchor=customtkinter.CENTER)
BackLeft = customtkinter.CTkLabel(master=app, text="0", bg_color="red", font=(project_font,25))
BackLeft.place(relx=0.395, rely=0.81, anchor=customtkinter.CENTER)

#### EXTRAS ####
Temp = customtkinter.CTkLabel(master=app, text="Temp: N/A", bg_color="red", font=(project_font,25))
Temp.place(relx=0.5, rely=0.9, anchor=customtkinter.CENTER)

app.resizable(False,False)
app.mainloop()
