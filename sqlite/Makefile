cc=gcc
obj=sqlite3.o 1_exec.o
target=demo
flags=-Wall -g
$(target):$(obj)
	$(cc)  $^ -o $@ $(flags) -ldl -lpthread
	@echo make success
sqlite3.o:sqlite3.c sqlite3.h
	$(cc) -c $< -o $@ $(flags)
1_exec.o:1_exec.c 
	$(cc) -c $< -o $@ $(flags) 
clean:
	rm $(obj) $(target) 
