import requests


class Motor():
    
    def __init__(self,motor,rpm,direction):
        self.url = ''
        self.motor=motor
        self.rpm =rpm
        self.direction = direction

    def set(self):
        response = requests.get(f"http://{self.url}/set?motor={self.motor}&rpm={self.rpm}&direction={self.direction}")
        if response:
            pass 
        else:
            print("Unable to connect with controller!!")
    
    def stop(self):
        response = requests.get(f"http://{self.url}/stop")
        if response :
            print(f"Motor{self.motor} is Stopped !")
        else:
            print(f"Unable to connect with controller!!")
