cc=gcc
#gcc=arm-linux-gcc
obj=main.o printf.o
target=main
cflags=-Wall -g


$(target):$(obj)
	$(cc) $^ -o $@ $(cflags)
main.o:main.c
	$(cc) -c $< -o $@ $(cflags)
printf.o:printf.c
	$(cc) -c $< -o $@ $(cflags)
clean:
	rm $(obj) $(target)
