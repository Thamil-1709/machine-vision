#USE THIS SCRIPT TO DETECT INCOMING OBJECT AND START THE IDENTIFICATION ALGO!!!
#RUNS ON THE LAPTOP/DESKTOP/MAIN-CLUSTER
import serial

# Open Serial connection
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

while True:
    if ser.in_waiting > 0:
        command = ser.readline().decode('utf-8').strip()

        if command == "DETECT_IMAGE":
            print("Performing image detection...")

            # Placeholder for actual image detection logic
            result = "fresh"  # or "not fresh" depending on the detection result

            # Send result back to ESP32
            ser.write(result.encode('utf-8'))
