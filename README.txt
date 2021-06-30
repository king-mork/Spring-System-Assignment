# Spring System Assignment

This repo contains my submission for the second assignment from UVic's CSC473 w/ Brandon Haworth.

The base template of the assignment and sample input was provided by Brandon Haworth.

The files that highlight my work are:
- SpringSim.cpp/.h
- Particles.cpp/.h
- Plane.cpp/.h
- myScene.cpp (sections indicated)

## To Open: 
- Select the solution file named "cs174-2010.sln"

## To Run:
- Commands can be issued through the console during runtime or in the "./Build/start.tcl" file.
- One of the assignment's sample input files, "complex_cube.tcl", can be copied into into the "start.tcl" file for a more interesting output.

## Controls:
s - start/stop
q - quit
right mouse - rotate
middle mouse - zoom in/out

## Commands:
$ system <sys_name> dim <Number of Particles>
     Initializes the particle system to hold up to the given number of particles.
$ system <sys_name> particle <index> <mass> <x y z vx vy vz> 
     Sets a position, mass and velocity for a given particle.
$ system <sys_name> all_velocities  <vx vy vz> 
     Sets the velocity of all particles.
$ simulator <sim_name> link <sys name> <Number of Springs>
    Links the simulator to a particular particle system and initializes it to work with a given number of springs.
$ simulator <sim_name> spring <index1> <index2> <ks> <kd> <restlength>
    Sets up a given spring. If the rest length is a negative number, the system should automatically set the rest length of the spring to the distance between the corresponding particles at the time the command is given.
$ simulator <sim_name> fix <index>
    Nails particle <index> to its current position.
$ simulator <sim_name> integration <euler|symplectic|verlet> <time step>
    This changes the integration technique used by the given simulator and sets the time step of the integration. 
$ simulator <sim_name> ground <ks> <kd> 
    Sets the parameters of the penalty forces applied to particles that try to go underground.
$ simulator <sim_name> gravity <g>
    Sets the acceleration due to gravity, in unit length per unit time squared.
$ simulator <sim_name> drag <kdrag>
    Sets the global drag (friction) coefficient (Fi = -kdrag vi).  The command expects a positive number 



