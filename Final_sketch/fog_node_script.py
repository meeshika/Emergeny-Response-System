import socket
import numpy as np
import tensorflow as tf
from tensorflow import keras
import smtplib
from email.mime.text import MIMEText

data_entry = []
model = keras.models.load_model("C:/Users/Vikas Singla/Desktop/thapar/SEMESTER 7/Capstone/model2.h5")

def send_email(subject, body, sender, recipients, pas):
    msg = MIMEText(body, "html")
    msg['Subject'] = subject
    msg['From'] = sender
    msg['To'] = ', '.join(recipients)
    with smtplib.SMTP_SSL('smtp.gmail.com', 465) as smtp_server:
       smtp_server.login(sender, pas)
       smtp_server.sendmail(sender, recipients, msg.as_string())
    print("Message sent!")

def process_data(data, latitude, longitude, car_model, number):
    data_tensor = tf.convert_to_tensor(data, dtype=tf.float32)  # Convert to TensorFlow tensor
    predi = model.predict(data_tensor)
    if predi[0] > 0.5:
        print("Prediction: Accident")
        subject = "Accident Detected"
        body = f"""
                <!DOCTYPE html>
                <html lang="en">
                <head>
                <meta charset="UTF-8">
                <title>Emergency Request</title>
                </head>
                <body>
                <div>
                    <p>I am writing to urgently request medical assistance for a detected road accident through our Accident Detection System.</p>
                    <p><strong>Address:</strong> Adarsh Nagar, Prem Nagar, Patiala, Punjab</p>
                    <p><strong>Exact Accident Location:</strong> <a href="http://www.google.com/maps/place/{latitude},{longitude}" target="_blank">View Location</a></p>
                    <p><strong>Car Model:</strong> {car_model}</p>
                    <p><strong>Registration Plate:</strong> {number}</p>
                    <p><strong>Immediate medical attention is required.</strong></p>
                </div>
                </body>
                </html>
                """
        sender = "accidentdetectionsystem2023@gmail.com"
        recipients = ["sahilpython741@gmail.com", "ssingla_be20@thapar.edu"]
        pas = "nnsp ynzm ymbp iibi"
        send_email(subject, body, sender, recipients, pas)

    else:
        print("Prediction: Not Accident")


def receive_data():

    global data_entry  # Declare data_entry as a global variable
    HOST = "0.0.0.0"  # Bind to all interfaces
    PORT = 8080

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Bind the socket to the HOST and PORT
        s.bind((HOST, PORT))

        # Listen for incoming connections
        s.listen(5)
        print("Started Listening")
        # Accept incoming connections
        while True:
            # Accept a connection and return a new socket object
            client_socket, address = s.accept()

            # Receive data from the client
            data = client_socket.recv(1024).decode('utf-8')
            print(f"Received data from client: {data}")

            client_socket.close()

            # Process received data
            if data:
                values = data.split(",")  # Split each line into values
                if values[9]=="" or values[10]=="":  # Skip lines with incorrect number of values
                        continue
                else:
                    latitude=values[9]
                    longitude=values[10]
                    car_model=values[11]
                    number=values[12]
                    values1 = np.array(values[0:9])
                    try:
                        values1 = values1.astype(float)
                    except ValueError:
                        continue
                    data_entry.append(values1)  # Append values to data_entry
                    if len(data_entry) == 50:
                        data_array = np.array(data_entry)
                        process_data(data_array.reshape((1, 50, 9)), latitude, longitude, car_model, number)
                        data_entry = []  # Reset data_entry after processing

if __name__ == '__main__':
    receive_data()


