CC := g++
IDIR := -I/usr/local/include -I./include
CMN := Common/SM.cpp

all: pmmain lsrmain

pmmain: PMModule/PMMain.cpp
	${CC} ${IDIR} ${CMN} PMModule/PMMain.cpp -o bin/pm

lsrmain: LaserPrac/LaserPrac.cpp
	${CC} ${IDIR} ${CMN} LaserPrac/LaserPrac.cpp -o bin/lsrprac

clean:
	rm -rvf *.out

