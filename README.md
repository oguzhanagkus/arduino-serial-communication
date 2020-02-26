You can communicate with Ardunio boards using C language. This is an example of it.

Upload the arduino_client to your board. Then keep it connected to your computer.
Update port_name variable in serial_com.c file. Then compile and run it.
Now, you can turn on/off the build-in led using your terminal.
Also you can send a number to board, then it returns its square.
It is restricted with 255 because of int type size.
