# 自动编译DES算法的代码
NAME = des
.DEFAULT_GOAL = app
# Compilation flags
CC = g++
LD = g++
CFLAGS   += -O2 -Wall -Werror

OBJS = des.o main.o

main.o : 
	@$(CC) -c main.cpp
des.o : 
	@$(CC) -c des.cpp
app:$(OBJS)
	@echo + LD $@
	@$(LD) $(OBJS) -o $(NAME) 

clean:
	@rm -rf $(OBJS) $(NAME)
