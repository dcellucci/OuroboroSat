#The Ouroboros Modular Satellite

 > I was sitting, writing at my text-book; but the work did not progress; my thoughts were elsewhere. I turned my chair to the fire and dozed. Again the atoms were gamboling before my eyes. This time the smaller groups kept modestly in the background. My mental eye, rendered more acute by the repeated visions of the kind, could now distinguish larger structures of manifold conformation: long rows, sometimes more closely fitted together; all twining and twisting in snake-like motion. But look! What was that? One of the snakes had seized hold of its own tail, and the form whirled mockingly before my eyes. As if by a flash of lightning I awoke; and this time also I spent the rest of the night in working out the consequences of the hypothesis.
 > ([August Kekulé](https://en.wikipedia.org/wiki/Ouroboros#Chemistry))

Ouroboros is the modular satellite platform that combines communication, power-sharing, and the Arduino framework to create a scalable platform for orbital experiments. It's designed to be even smaller than a CubeSat (one sixth, to be exact), and to be flexible enough to conform to a wide variety of payload envelopes.

##What does it do?

The Ouroboros board (v. 1.0) can do alot of things. 

  1. It behaves like an Arduino, so it can run Arduino code and libraries
  2. It has the circuitry required to charge an on-board LiPo battery from a solar panel
  3. It can connect to a maximum of four neighbors, and can talk to these four neighbors using a protocol called the Asynchronous Packet Automaton (which you can learn more about [here](https://github.com/dcellucci/APA_Tutorial)).
  4. It can also use this connection to charge a neighbor's battery using its own on-board battery, for which there are plenty of reasons, detailed in a forthcoming paper. 
  5. It has a socket which allows it to be extended with experimental payloads.

Ouroboros is supposed to be alot like the Iron Man suit- it's the foundation upon which a more complex and specialized iterations can be built. My goal is to make the process of extending both the hardware and the software is as easy and transparent as possible.

##Using This Library
The base Ouroboros Board is a class that you extend. This class has all of the communication, routing, and power management code already implemented, all you have to do is add your functionality on top of it. You do this by making a subclass which inherits the Ouroboros class, and calls a few methods. Check out the **Getting Started** section for more information.

###Dependencies
There are a few libraries that Ouroboros uses to do its job, they are either included in this library or available for download. 

 1. **APA** provides the code for communicating between boards. It implements a software object called a port that handles IO and processing for a packet of data. The default hardware that the library implements is bit-banging over a two-wire interface. Each Ouroboros Board has four APA ports, labelled **N**orth, **S**outh, **E**ast, **W**est. 

 2. **I2C** [rev. 5](http://dsscircuits.com/articles/arduino-i2c-master-library) replaces the Wire library that is the default in Arduino with a version that supports timeouts. This becomes important when hardware on the I2C bus fails, since failed hardware effectively freezes the microcontroller if no timeouts are implemented.

 3. **Max17043** a lightly modified version of [this library](https://github.com/lucadentella/ArduinoLib_MAX17043), this allows the microcontroller to use the I2C bus to communicate with the Max17043 LiPo fuel gauge to read the current capacity of the battery. 

All three of these libraries are included here, inside the `Libs` folder. Just copy them to your Arduino libraries folder to install them.

##Getting Started
This section will show you how to create a version of the boards that can communicate over serial with your computer. It will go into the particulars of inheritance, overloading, and the protocol that Ouroboros uses to communicate. Alternatively, if you're the kind of person that just likes to dive in, I've included a few examples, including a board (written by Stephen Im and Greenfield Trinh) that communicates with an xBee module, and a board that talks to an example experimental payload (designed by Will Langford at MIT's Center for Bits and Atoms).

First things first, install the libraries in the manner described above. Then just copy the `Blank_Ouroboros` folder over to your sketch folder.









