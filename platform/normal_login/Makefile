OBJ += normal_login.o
#OBJ += gprs.o
OBJ += sqlite3.o
#OBJ += test.o

CC = gcc
CFLAGS = -g -Wall
OPTION =  -ldl -lpthread
EXEC_NAME = normal_login
EXEC_PATH = ./$(EXEC_NAME)

$(EXEC_NAME):$(OBJ)
	@echo make ...
	@$(CC) $^ -o $(EXEC_PATH) $(CFLAGS) $(OPTION)
	@echo make over
	@echo Execute target is $(EXEC_PATH)
%.o:%.c
	@$(CC) -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	@echo clean ...
	rm $(EXEC_PATH) *.o -rf
	@echo clean over

	