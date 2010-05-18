The files "ds.hid" and "upcb.hid" are files for the "HID Descriptor Tool"
<http://www.usb.org/developers/hidpage#HID%20Descriptor%20Tool>.
Because of a bug in the HID Descriptor Tool the last usage page is wrong, it should be:

    0x06, 0x00, 0xff,              //   USAGE_PAGE (Vendor Defined Page 1)


The file "lsusb.txt" is the lsusb output of a SixAxis controller, it is
from <http://ps3.jim.sh/sixaxis/usb/>.