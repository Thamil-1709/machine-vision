import requests 
import cv2

# cam configuration
cap1 = cv2.VideoCapture(0)
cap2 = cv2.VideoCapture(1)
cap3 = cv2.VideoCapture(2)

#configuration variabls
conveyor_ip = ''

# Motor class
# intializing the motor configuartion
'''
    Motor1 - 0
    Motor2 -1
'''

main_conveyor = Motor(1,10,1)
sorting_conveyor = Motor(2,100,1)


# ultils 
def control_conveyor(motor,rpm=100,direction=1):
    print(f"Starting Converyor {motor.number} ......")
    response =requests.get(f"http://{conveyor_ip}/motor/set?motor={motor}&rpm={rpm}&direction={direction}")
    if response.status_code =="200":
        print("conveyor is running....! ")
    else:
        print("Failed..!")



def detail_extraction():
    '''
    Extraxt text and return it as json object 

    object format :
    {
        product_name :str,
        brand_name:str,
        product_count: integer,
        product_quantity:float
        is_expired:boolean,
    }

    '''
    pass 


def freshness_model():
    '''
    
    fresh Index of fruits and vegetable
    - > future approaches use the spectrol data to predict the freshness of the product
    

    '''

    pass 


def segementation_model():

    '''

    segementation using YOLO V11
    - > segementation of the organic and inorganic product
    

    '''
    pass


def main():
    print("Starting System ....")

    res = requests.get(f"http://{conveyor_ip}/motor/set?motor={main_conveyor}&rpm={100}&direction={1}")
    
    if res.status_code == 200:
        print("Main conveyor is running....! ")
    else:
        print("Failed..!")
        


    while True:
        ret1, frame1 = cap1.read()
        ret2, frame2 = cap2.read()
        ret3, frame3 = cap3.read()
    


    '''
    thread 1
        -ocr 
    thread -2 
    - barcode 
    
    data - json (gemini)

    based ocr we have say the product is good to use or not

    notify the conveyor
    
    json -> log

    '''


