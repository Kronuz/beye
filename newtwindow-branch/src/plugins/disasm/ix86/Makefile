include ../../../../config.mak

CXXSRCS = ix86_fpu.cpp ix86_fun.cpp ix86.cpp
SRCS    =
OBJS	= $(SRCS:.c=.o)
CXXOBJS	= $(CXXSRCS:.cpp=.o)

CXXFLAGS = -I.. -I../.. -I../../.. $(HOST_CFLAGS)
LDXXFLAGS = $(HOST_LDFLAGS)

.SUFFIXES: .c .o

# .PHONY: all clean

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
.cpp.o:
	$(CC) -c $(CXXFLAGS) -o $@ $<

all:	dep $(OBJS) $(CXXOBJS)

clean:
	rm -f *.o *.a *~

distclean:
	rm -f test *.o $(LIBNAME) *~ .depend

dep:    .depend

.depend: $(SRCS) $(CXXSRCS)
	$(CC) -MM $(CXXFLAGS) $(SRCS) $(CXXSRCS) 1>.depend

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
