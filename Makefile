GCC = gcc
SRC = main.c node.c tools.c
OBJS = main.o node.o tools.o
LIBS = -lm

main: $(OBJS)
	$(GCC) $(OBJS) $(LIBS) -o $@

%.o: %.c
	$(GCC) -c $< -o $@

clean:
	rm -f $(OBJS) main


