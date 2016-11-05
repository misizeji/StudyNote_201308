src += main.c
src += player_process.c
src += player_display.c
src += player_key.c
src += player_touch.c
src += lyrics_process.c

obj = $(src:%.c=%.o)

CC = arm-linux-gcc
FLAG = -Wall
OPTION = -lpthread -L../lib -llcd
EXEC_NAME = play
EXEC_PATH = ../execute/$(EXEC_NAME)

play:$(obj)
	@echo make ...
	$(CC) $^ -o $(EXEC_PATH) $(FLAG) $(OPTION)
	@echo make over
	@echo Execute target is $(EXEC_PATH)
%.o:%.c
	$(CC) -c -o $@ $< $(FLAG)

.PHONY:clean
clean:
	@echo clean ...
	rm $(EXEC_PATH) *.o -rf
	@echo clean over
