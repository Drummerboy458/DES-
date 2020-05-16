# 自动编译RSA算法的代码
NAME = rsa
.DEFAULT_GOAL = app
# Compilation flags
CC = g++
LD = g++
CFLAGS   += -O2 -Wall -Werror

OBJS = rsa.o main.o

main.o : 
	@$(CC) -c main.cpp
rsa.o : 
	@$(CC) -c rsa.cpp
app:$(OBJS)
	@echo + LD $@
	@$(LD) $(OBJS) -o $(NAME) 

clean:
	@rm -rf $(OBJS) $(NAME)
