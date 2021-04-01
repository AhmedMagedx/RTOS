# Project 1 
contain a 3 simple tasks:
## UserInput :
receive the student data from the uart.
at first, it receive the student name, then its mark.
when the new student data is fully recieved, a msg with the student struct is send through the queue the the higher priority UserOutput task.

## UserOutput :
when a new msg is ready in the queue, the UserOutput (which is higher priorty) state become READY instead of being WAITING for the msg, then the running (Lower priority) task is preemted.
now, the UserOutput is RUNNING and print the new student data on the uart vertual terminal.

## Indication :
it has a simple task of show indication on LED every second that the system is running.


# Simulation Video 

![Project1Video](./video/project1_Queue.mp4)


# Used
protues

FreeRtos

Eclipse

AVR