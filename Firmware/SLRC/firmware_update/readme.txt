This is a special release of the 3.3.1 firmware behaving different for SOCD
(Simultaneous Opposing Cardinal Directions): if left and right are
simultaneously activated, they cancel each other out (i.e. no signal is
sent to the host).

To still fit the firmware on the MCU, support for the 4th MAME mapping was
removed. It is still configurable, but you cannot switch to it.
