srcdir := src
objdir := obj
bindir := bin
logdir := logs
srcsub := test
relsrc := $(addprefix ../$(srcdir)/,$(srcsub))
relinc := ../$(srcdir)
relobj := ../$(objdir)
relbin := ../$(bindir)
cflags := -Wall -O3 -I$(relinc)
lflags := -g
objfls := $(relobj)/*.o
srcfls := $(addsuffix /*.cpp,$(relsrc))

all:$(objdir) $(bindir) $(logdir)
	make --directory=./obj --makefile=../Makefile binary

$(objdir):
	mkdir $(objdir)
	
$(bindir):
	mkdir $(bindir)

$(logdir):
	mkdir $(logdir)

binary:objects
	g++ $(lflags) -o $(relbin)/binary $(objfls)

objects:
	g++ $(cflags) -c $(srcfls)

clean:
	rm -r $(objdir) $(bindir) $(logdir)
