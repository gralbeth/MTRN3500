CC := g++
IDIR := -I/usr/local/include -I./include

pmmain: PMModule/PMMain.cpp
	${CC} ${IDIR} PMModule/PMMain.cpp -o pmmain.out

lasermain: LaserModule/LaserMain.cpp
    $(CC) $(IDIR) LaserModule/LaserMain.cpp -o lasermain.out

clean:
	rm -rvf *.out