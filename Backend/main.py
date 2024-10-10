import requests 
from conveyor import Motor


#configuration variables
conveyor_ip = ''

# intializing the motor configuartion

main_conveyor = Motor(1,10,1)
sorting_conveyor = Motor(2,100,1)


# ultils 
def  control_conveyor(motor,rpm=100,direction=1):
    print(f"Starting Converyor {motor.number} ......")
    response =requests.get(f"http://{conveyor_ip}/start?motor={motor}&rpm={rpm}&direction={direction}")
    if response.status_code =="200":
        print("conveyor is running....! ")
    else:
        print("Failed..!")


def load_model():
    pass


def ocr_detection():
    '''
    Extraxt text and return it as json object 

    object format :
    {
        product_name :str,
        brand_name:str,
        product_count: integer,
        is_expired:boolean,
    }

    '''
    pass 


def freshness_index():
    '''
    fresh Index of fruits and vegetable
    based on hyperspectral visuals
    '''
    pass 


def segementation():
    '''segementation using YOLO V11'''
    pass




def main():
    print("Starting System ....")

    
        











