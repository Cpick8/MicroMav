import requests

url = "http://192.168.4.1:80"

while True:
    try:
        response = requests.get(url)
        if response.status_code == 200:
            json_data = response.json()
            print(json_data)
        else:
            print("Failed to retrieve data. Status code:", response.status_code)
    except requests.exceptions.RequestException as e:
        print("Error:", e)


