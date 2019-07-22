CC := g++
IDIR := -I/usr/local/include -I./include
CMN := Common/SM.cpp

all: pmmain lsrmain

pmmain: PMModule/PMMain.cpp
	${CC} ${IDIR} ${CMN} PMModule/PMMain.cpp -o bin/pm

lsrmain: LaserModule/LaserMain.cpp
	${CC} ${IDIR} ${CMN} LaserModule/LaserMain.cpp -o bin/lsr

clean:
	rm -rvf *.out

