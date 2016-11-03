cd ../driver
insmod am2311.ko
insmod key4+5.ko
lsmod
cd ../input_t9
./input_t9 -qws
