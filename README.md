# 🚇 Smart Tunnel for Maintenance and Security

An IoT-based smart tunnel monitoring and security system designed to provide **real-time environmental monitoring, lighting control, air-quality monitoring, and video surveillance**.

The system uses an **ESP32 microcontroller**, **BH1750 light sensor**, **MQ-135 air-quality sensor**, **ESP32-CAM**, **BLDC fan**, and **Blynk IoT platform** to monitor and control tunnel conditions remotely.

## 📌 Project Overview

Tunnels can be difficult to maintain and secure because of their length, complexity, and remote operating environment.

This project integrates multiple IoT-based features into a single system:

* 💡 Light-level monitoring and LED control
* 🌫️ Air-quality monitoring
* 📷 Video surveillance using ESP32-CAM
* 🌀 Automatic fan control
* 📱 Remote monitoring through Blynk
* ☁️ IoT-based real-time data communication
* 🔧 Modular and scalable architecture

The system collects sensor data using the ESP32, processes the information, and sends it to the Blynk dashboard for remote monitoring and control.

## 🎯 Objectives

* Monitor tunnel lighting conditions in real time.
* Detect poor air-quality conditions.
* Control tunnel ventilation using a fan.
* Provide video surveillance for improved security.
* Allow remote monitoring through the Blynk platform.
* Reduce maintenance effort and improve tunnel safety.
* Develop a low-cost and scalable IoT-based tunnel monitoring solution.

## 🛠️ Hardware Components

| Component       | Purpose                   |
| --------------- | ------------------------- |
| ESP32           | Main IoT microcontroller  |
| BH1750          | Ambient light measurement |
| MQ-135          | Air-quality monitoring    |
| ESP32-CAM       | Video surveillance        |
| L298N           | Motor/fan driver          |
| BLDC Fan        | Tunnel ventilation        |
| LEDs            | Tunnel lighting           |
| FTDI Programmer | ESP32-CAM programming     |

## 🧠 System Architecture


## ⚙️ How It Works

1. The **BH1750** measures the ambient light level inside the tunnel.
2. The **MQ-135** monitors air-quality conditions.
3. The **ESP32** receives and processes sensor data.
4. LEDs can be controlled according to lighting conditions.
5. When air quality becomes poor, the ventilation fan can be activated.
6. The **ESP32-CAM** provides video surveillance.
7. Sensor information is transmitted to the **Blynk dashboard**.
8. The system can be monitored and controlled remotely.

## 📊 Performance

The reported system evaluates the sensors using the following performance metrics:

| Sensor              | Reported Accuracy |
| ------------------- | ----------------: |
| BH1750 Light Sensor |               ±2% |
| MQ-135 Air Quality  |              ±5% |

## 📷 Project Hardware

The prototype combines the ESP32, ESP32-CAM, BH1750, MQ-135, motor driver, LEDs, and fan into a tunnel monitoring model.

## 🔌 Circuit Diagram

The circuit connects the ESP32 with the BH1750 light sensor, MQ-135 air-quality sensor, ESP32-CAM, LED lighting, and L298N motor driver for fan control.

## 📱 IoT Platform

The project uses **Blynk** as the IoT platform.

The Blynk dashboard is used to:

* Monitor sensor data
* View tunnel conditions
* Control the ventilation fan
* Remotely monitor the system

## 🚀 Future Improvements

Possible future improvements include:

* Predictive maintenance using machine learning
* Energy optimization
* Improved sensor accuracy and reliability
* Advanced security monitoring
* Automatic incident detection
* Real-world tunnel deployment
* Additional environmental sensors
* More advanced data analytics
* Improved software security



## 👨‍💻 Authors

* **Md. Akhtarujjaman Siddiquee**


*Department of Internet of Things and Robotics Engineering*
*University of Frontier Technology, Bangladesh*

