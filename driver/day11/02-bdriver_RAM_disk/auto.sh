rm -rf /lib/modules/2.6.34/tst-driver.ko
cp -a tst-driver.ko /lib/modules/2.6.34/ -rf
insmod tst-driver.ko
rm -rf /lib/modules/2.6.34/modules.dep.bb
