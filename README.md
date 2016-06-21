# Buzz_for_RPI
A C code to interface the PS3 Buzz Controllers with Raspberry Pi

Requires joyutils:

  packer -S joyutils

Few modules should be loaded (just in case):

  sudo modprobe joydev usbhid

To test that your RPI is reading some inputs from the Buzz USB dongle, use:

  jstest /dev/input/js0
