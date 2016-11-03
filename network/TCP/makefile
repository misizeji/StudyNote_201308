cc=gcc
#cc=gcc
obj=client_fun.o pthread_tcp.o 
target=pthread_tcp
cflags=-Wall -g

$(target):$(obj)
	$(cc) $^ -o  $(target) -lpthread
pthread_tcp.o:pthread_tcp.c
#client_fun.h
	$(cc) -c $< -o $@
client_fun.o:client_fun.c client_fun.h
	$(cc) -c $< -o $@
clean:
	rm $(obj) $(target) -rf
