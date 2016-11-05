cc=gcc
obj=main.o UDP_msg.o keyboard_input.o TCP_file.o sqlite3.o
target=main
cflags=-Wall -g

$(target):$(obj)
	$(cc) $^ -o $(target) $(cflags) -lpthread -ldl
	@echo  make success
main.o:main.c 
#UDP_msg.h keyboard_input.h TCP_file.h IPMSG.H
	$(cc) -c $< -o $@ $(cflags) -lpthread
UDP_msg.o:UDP_msg.c UDP_msg.h sqlite3.h
	$(cc) -c $< -o $@ $(cflags) 
keyboard_input.o:keyboard_input.c keyboard_input.h sqlite3.h
	$(cc) -c $< -o $@ $(cflags)
TCP_file.o:TCP_file.c TCP_file.h
	$(cc) -c $< -o $@ $(cflags)
sqlite3.o:sqlite3.c sqlite3.h
	$(cc) -c $< -o $@ $(cflags)
clean:
	@echo clean ...
	rm $(obj) $(target)
	@echo clean over
