OBJ += gprs_msg.o
OBJ += gprs.o
#OBJ += test.o

CC = arm-linux-gcc
CFLAGS = -g -Wall
#OPTION =  -lpthread -ldl
EXEC_NAME = gprs_msg
EXEC_PATH = ./$(EXEC_NAME)

$(EXEC_NAME):$(OBJ)
	@echo make ...
	@$(CC) $^ -o $(EXEC_PATH) $(CFLAGS) 
	@echo make over
	@echo Execute target is $(EXEC_PATH)
%.o:%.c
	@$(CC) -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	@echo clean ...
	rm $(EXEC_PATH) *.o -rf
	@echo clean over

	