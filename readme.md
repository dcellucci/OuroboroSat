#The Ouroboros Modular Satellite

Ouroboros is the modular satellite platform that combines communication, power-sharing, and the Arduino framework to create a scalable platform for orbital experiments. It's designed to be even smaller than a Cubesat (one sixth, to be exact)

##What does it do?

The Ouroboros board can do alot of things. 

  1. It behaves like an Arduino, so it can run Arduino code and libraries
  2. It has the circuitry required to charge an on-board LiPo battery from a solar panel
  3. It can connect to a maximum of four neighbors, and talks to these four neighbors using a protocol called the Asynchronous Packet Automaton (which you can learn more about here).
  4. It can also use this connection to charge a neighbor's battery using its on-board battery, for which there are plenty of reas

This library provides this basic functionality as an API. 

##Using this library
