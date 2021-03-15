# ZESBEEN
Arduino project for walking six-legged robot. Pronounce "zesbeen" as "zesbane", it means of course sixlegs.

The project consists of the hardware and software for a robot that walks on six legs whereby each leg has two servo-motors, for DIJ (Thy) and KNIE (knee). The front and hind legs have also an extra servo for the SCHOUDER (shoulder), for making turns.

The robot has two Arduino Mega's: one for the walking movement, one for the sensors (plus the SCHOUDERS). The two arduino's communicate via opto-couplers.
The software consists of two main-programs plus a number of classes, the latter primarily to encapsulate hardware components.
Some of the main classes are:
  MP_GEWRICHT (joint)   - encapsulating the Servo's.
  MP_BEEST    (beast)   - encapsulating the six legs as a group

The sensor-controller receives real-world information and turns this into Events. The Events lead to state-changes in a state machine. Each state corresponds with a certain movement of ZESBEEN.
In this way, ZESBEEN is able to walk to a room and avoid obstacles.
