cc = $(CC)
bin = b2t
CFLAGS =  -O2  -ansi -Wpedantic -Wall -Werror

.PHONY: all clean

all:	$(bin)

$(bin):	main.o function.o opt.o
	$(cc) -o $(bin) main.o function.o opt.o
main.o:
	$(cc) -c -o main.o $(CFLAGS) main.c
function.o:
	$(cc) -c -o function.o $(CFLAGS) function.c
opt.o:
	$(cc) -c -o opt.o $(CFLAGS) opt.c
clean:
	rm -rf *.o $(bin) *~

