# kernelspace_load_checker

This is a test kernel module that quickly calculates the systems 1, 5, and 15 minute load average within kernel space.
This is achieved by creating a workqueue `loadavg_wq` and adding a timer to it that will execute the calculation 
every `msecs_between` milliseconds. 

Currently this does insert into the kernel correct and calculate the load averages properly.

The next step is to enable it to use the sysrq facility to either print task information to the console
or initiate a panic. 

Tested only on RHEL 7 kernel 3.10.0-1160.el7.x86_64 so far. 
Make sure you have the usual build packages installed.

Download the Makefile and k_loadd.c, and run `make`. 

... 

Sample output: 

  [Fri Jun 16 14:57:08 2023] Loading the kernel-space load average reader.
  [Fri Jun 16 14:57:08 2023] Creating workqueues.
  [Fri Jun 16 14:57:08 2023] Creating timer.
  [Fri Jun 16 14:57:08 2023] Attempting to read the load averages, iteration: 0
  [Fri Jun 16 14:57:08 2023] Printing loads.
  [Fri Jun 16 14:57:08 2023] 1 Avg: 0.00
  [Fri Jun 16 14:57:08 2023] 5 Avg: 0.01
  [Fri Jun 16 14:57:08 2023] 15 Avg: 0.01
  [Fri Jun 16 14:57:09 2023] Attempting to read the load averages, iteration: 1
  [Fri Jun 16 14:57:09 2023] Printing loads.
  [Fri Jun 16 14:57:09 2023] 1 Avg: 0.00
  [Fri Jun 16 14:57:09 2023] 5 Avg: 0.01
  [Fri Jun 16 14:57:09 2023] 15 Avg: 0.01
  [Fri Jun 16 14:57:10 2023] Attempting to read the load averages, iteration: 2
  [Fri Jun 16 14:57:10 2023] Printing loads.
  [Fri Jun 16 14:57:10 2023] 1 Avg: 0.00
  [Fri Jun 16 14:57:10 2023] 5 Avg: 0.01
  [Fri Jun 16 14:57:10 2023] 15 Avg: 0.01
  ... snip ...
  [Fri Jun 16 15:00:02 2023] Attempting to read the load averages, iteration: 174
  [Fri Jun 16 15:00:02 2023] Printing loads.
  [Fri Jun 16 15:00:02 2023] 1 Avg: 6.45
  [Fri Jun 16 15:00:02 2023] 5 Avg: 1.54
  [Fri Jun 16 15:00:02 2023] 15 Avg: 0.51

