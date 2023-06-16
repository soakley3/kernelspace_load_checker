# kernelspace_load_checker

This is a test kernel module that quickly calculates the systems 1, 5, and 15 minute load average within kernel space.
This is achieved by creating a workqueue `loadavg_wq` and adding a timer to it that will execute the calculation 
every `msecs_between` milliseconds. 

Currently this does insert into the kernel correct and calculate the load averages properly.

The next step is to enable it to use the sysrq facility to either print task information to the console
or initiate a panic. 


Download the Makefile and k_loadd.c, and run `make`. 
Make sure you have the usual build packages installed.
