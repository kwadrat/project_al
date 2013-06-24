# Generated automatically from Makefile.in by configure.
OBJS= inoutx.o al.o catx.o plx.o bufx.o hexx.o procx.o masterx.o logx.o
CFLAGS= -g -O2 -DLINUX
CPPFLAGS=
CC= gcc
CXX= c++
DEFS =  -DUNIX=1 -DSTDC_HEADERS=1
LIBS =
PROGNAME=pl

$(PROGNAME): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $@

inoutx.o: inoutx.cpp inoutx.h masterx.h procx.h typy.h procx.h typy.h typy.h
	$(CXX) $(CFLAGS) -c $<

pl.o: al.cpp typy.h bufx.h procx.h logx.h masterx.h typy.h procx.h typy.h \
      typy.h bufx.h procx.h masterx.h hexx.h typy.h procx.h typy.h typy.h \
      procx.h typy.h typy.h procx.h procx.h typy.h typy.h typy.h typy.h
	$(CXX) $(CFLAGS) -c $<

catx.o: catx.cpp catx.h procx.h typy.h
	$(CXX) $(CFLAGS) -c $<

plx.o: plx.cpp plx.h procx.h typy.h
	$(CXX) $(CFLAGS) -c $<

bufx.o: bufx.cpp bufx.h typy.h typy.h procx.h typy.h
	$(CXX) $(CFLAGS) -c $<

hexx.o: hexx.cpp hexx.h procx.h typy.h
	$(CXX) $(CFLAGS) -c $<

procx.o: procx.cpp procx.h typy.h
	$(CXX) $(CFLAGS) -c $<

masterx.o: masterx.cpp masterx.h typy.h procx.h typy.h
	$(CXX) $(CFLAGS) -c $<

logx.o: logx.cpp logx.h inoutx.h procx.h masterx.h catx.h plx.h hexx.h typy.h \
        bufx.h procx.h masterx.h hexx.h procx.h typy.h typy.h procx.h procx.h \
        procx.h procx.h typy.h procx.h typy.h typy.h procx.h procx.h typy.h \
        typy.h typy.h typy.h typy.h typy.h typy.h typy.h
	$(CXX) $(CFLAGS) -c $<

install:

clean: clean-unix
clean-amiga:
	@delete \#?.o \#?.bak \#?.lnk \#?.info $(PROGNAME)
clean-unix:
	rm -f *.o $(PROGNAME) config.cache config.log config.status

full-clean: clean prog-clean

prog-clean:
	( rm configure Makefile; autoconf)
	
