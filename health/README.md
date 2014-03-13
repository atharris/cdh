Health Monitoring.

This directory contains the source for our system's health monitoring module. This module will continously look for out 
of range values, or anything that indicates a system fault, and alert the system upon any event. It will have
to discern between different types of faults (temperature/voltage? location? severity?) and allow the rest of the system
to act accordingly (turn hot end away from sun? restart? self-destruct? cry?).
