This system can operate on both FPGA in Verilog and Raspberry Pi 4.  

In the field with plants there are soil moisture sensors (YL69), which signal the lack of water in the ground.
If it is necessary to water the soil, a signal is sent from the garden to the mobile robot using the Wi-Fi mechanism, which will mean that it is time to water. An ESP2866 nodeMCU microcontroller is used to send such a signal to the robot and the robot starts moving. The robot is powered by four DC motors.

There is an RFID tag in front of each plant and RFID reader on the robot which reads RFID tags.

The robot starts moving along a predefined path. IR sensors read the surface on which the robot moves, and depending on these inputs, the robot moves in a straight line or to the left/right until it reaches a certain RFID card. When the RFID reader on the robot reads the card, it is checked whether the given card is actually a pot that needs to be watered or not. In case it is not, the robot continues to move until it reads the card with the correct address. In case it is, the robot stops. After stopping, the pump located in the water tank is turned on, and the watering process begins.

List of used equipment:
1.	Cyclone II FPGA
2.	Raspberry Pi
3.	ESP8266 NodeMCU (3 pieces)
4.	YL-69 soil moisture sensor
5.	IR sensor
6.	RFID
7.	DC motors
8.  L298N Motor driver
9.  Water pump
10.  Breadbord
11.  Jumper wires
12.  Arudino Smart Robot Car Kit


