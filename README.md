scl011
======

Purpose
-------

The SCM Microsystem SCL011 is a contactless card reader very popular in Germany
(because it's cheap and given away for free with print magazines). SCM
Microsystem provided a linux driver right from the start to use it along with
pcsc-lite. This driver works within test scenarios, but for me it crashed to
often to be usable.

This code is based on monitoring the USB traffic exchanged with the host system
and the SCL011 reader when using the official driver on pcsc-lite. No
disassemling or decompiling was used.

Looking at USB traffic
----------------------

To look at USB traffic you can use usbmon and wireshark. There are several
tutorials out there, but the essentials are:

Loading usbmon module (if not part of the kernel)

``
$ modprobe usbmon
``

Figuring out the bus to monitor

``
$ lsusb | grep SCL011
Bus 007 Device 013: ID 04e6:5292 SCM Microsystems, Inc. SCL011 RFID reader
``

Launching wireshark and selecting the matching usbmon device (in this example
it is "usbmon7")

Interpreting the traffic is pretty easy as well. The small part of the USB
protocol we need are URB_BULK messages. The USB bus allows an active member
of the bus to send data to (URB_BULK out) and request data from a device
(URB_BULK in). These always come in pairs: the active part sends the message,
the passive part confirms it.

Each block of the protocol to the SCL011 looks like this:
* Active member sends URB_BULK out (URB_SUBMIT), containing data for the device
* Passive member confirms the URB_BULK out (URB_COMPLETE)
* Actuve member sends URB_BULK in (URB_SUBMIT)
* Passive member confirms URB_BULK in (URB_COMPLETE), containing data from the
  device

Building
--------

scl011 uses cmake to build. The only necessary dependency is libusb 1.0. Use
the following commands to build scl011.

``
mkdir build
cd build
cmake ..
make
``

Running
-------

Right now the only USB device ID supported is 04e6:5292. Next to that you need
to access the device. Usually devices cannot be accessed by every user, so I'd
recommend something like the following as /etc/udev/rules.d/61-usb.rules to
grant access to every member in the wheel group (or whichever group you prefer).

``
ACTION=="add|change", SUBSYSTEM=="usb", ATTRS{idVendor}=="04e6", ATTRS{idProduct}=="5292", GROUP="wheel"
``

Afterwards reload the udev rules (udevadm control --reload-rules) simply attach
the device and start the test program.

``
./scl011
``

Right now the program is *very* limited, only able to detect the card and
reporting if the protocol differs from the wireshark recordings. (Note: Right
now it will for sure report a difference when a card is detected, as the code
is hard wiring the ID of a test card of mine.)
