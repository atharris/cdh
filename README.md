cdh
===

This is the source for the Command and Data Handling system for the University at Buffalo Department of Mechanical
and Aerospace Engineering's Glint Anyalyzing Nanosatellite, known as GLADOS. We are competing in the Air Force Research Lab's University Nanosatellite Program. We are also part of NASA's CubeSat Launch Initiative. 

System's responsibilities are to interface with all devices and provide architecture to support the safe and concurrent execution of attitude determination, attitude control, and science data acquisition software.

Runs on top of Linux on space grade ARM board. Uses standard Linux processes and threads for concurrency, and standard Linux message queues and signals for inter-process communication. 

Authors are Michael Ferris, Andrew Tennenbaum, Brian Bezanson, and Nicholas Cellino.

Questions can be sent to mrferris@buffalo.edu.
