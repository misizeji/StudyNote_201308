cc=gcc
#cc=arm-linux-gcc
obj=add.o div.o sub.o mux.o main.o
target=main
flags=-Wall -g	

$(target):$(obj)
	$(cc) $^ -o $(target)
main.o:main.c
#add.h sub.h mux.h div.h
	$(cc) -c $< -o $@
add.o:add.c add.h
	$(cc) -c $< -o $@
div.o:div.c div.h
	$(cc) -c $< -o $@
sub.o:sub.c sub.h
	$(cc) -c $< -o $@
mux.o:mux.c mux.h
	$(cc) -c $< -o $@
clean:
	rm $(obj) $(target)
