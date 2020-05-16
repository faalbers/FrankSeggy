# Segway Robot

This Segway system was developped using a [TinyTile](https://cdn.sparkfun.com/assets/home_page_posts/2/4/1/1/tinytile-product-brief-feb2017.pdf) controller. It has built in 6-axis combo sensor with
accelerometer and gyroscope.
For motors I used [Geeetech 3D Printer RepRap 1.8-Degree 42YB Stepper Motors](https://www.dx.com/p/geeetech-3d-printer-reprap-1-8-degree-anti-slip-42yb-stepper-motor-black-2049588.html#.Xr80BhNKi-s)
For StepperMotor drivers I used the [DRV8825](https://reprapworld.com/datasheets/datasheet%20drv8825.pdf)
And finally for control I used the Frsky Taranis with the [Frsky D4R-II](https://www.frsky-rc.com/product/d4r-ii/)

## How it works

The accelerometer and gyroscope are used to determin the upwards angle of the segway. Bu slowly tilting it up, it will start working when the upward position is found.
The angel change while loosing balance is fed into a PID system which counter rotates using the stepper motors to counter the balance loss. This is how the robot stays upright and balanced.
The Controller sticks will add an angle to the tilt so that it starts moving forward and backward. The turn sticks will add a speed difference between booth motors so that the segways can turn.

## Example videos

[Balancing example](https://www.youtube.com/watch?v=Dd7d--UJCoA) .
[Pitch example](https://www.youtube.com/watch?v=g9bDURan0Ac) .
[Pitch and Turn example](https://www.youtube.com/watch?v=KHHvi9PKs44) .

### Proprietary Code
All the coding from Sensor handling to Stepper controlling , PPM stick control reading and PID feedback was created by Frank Aalbers. I started this project to get better at coding microcontrollers. The [Arduino IDE](https://www.arduino.cc/en/Main/Software) with [PlatformIO](https://platformio.org/) under [VSCODE](https://code.visualstudio.com/) environment was used to develop this code.

### Code setup and upload

Under construction.

### Hardware setup and upload

Under construction.

