#The Ouroboros Modular Satellite

 > I was sitting, writing at my text-book; but the work did not progress; my thoughts were elsewhere. I turned my chair to the fire and dozed. Again the atoms were gamboling before my eyes. This time the smaller groups kept modestly in the background. My mental eye, rendered more acute by the repeated visions of the kind, could now distinguish larger structures of manifold conformation: long rows, sometimes more closely fitted together; all twining and twisting in snake-like motion. But look! What was that? One of the snakes had seized hold of its own tail, and the form whirled mockingly before my eyes. As if by a flash of lightning I awoke; and this time also I spent the rest of the night in working out the consequences of the hypothesis.
 > ([August Kekulé](https://en.wikipedia.org/wiki/Ouroboros#Chemistry))

Ouroboros is the modular satellite platform that combines communication, power-sharing, and the Arduino framework to create a scalable platform for orbital experiments. It's designed to be even smaller than a CubeSat (one sixth, to be exact)

##What does it do?

The Ouroboros board (v. 1.0) can do alot of things. 

  1. It behaves like an Arduino, so it can run Arduino code and libraries
  2. It has the circuitry required to charge an on-board LiPo battery from a solar panel
  3. It can connect to a maximum of four neighbors, and can talk to these four neighbors using a protocol called the Asynchronous Packet Automaton (which you can learn more about [here](https://github.com/dcellucci/APA_Tutorial)).
  4. It can also use this connection to charge a neighbor's battery using its own on-board battery, for which there are plenty of reasons, detailed in a forthcoming paper. 

Ouroboros is supposed to be alot like the Iron Man suit- it's the foundation upon which a more complex and specialized iterations can be built. My goal is to make the process of extending both the hardware and the software is as easy and transparent as possible.

##Using this library
The base Ouroboros Board is a class that you extend. In the examples I have a blank version that allows you to write an arduino sketch that extends the existing Ouroboros framework. 