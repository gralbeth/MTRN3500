CC := g++
IDIR := -I/usr/local/include -I./include
CMN := Common/*.cpp
PM := PMModule/PMMain.cpp
LSR := LaserModule/*.cpp
DM := DisplayModule/*.cpp
GPS := GPSModule/*.cpp
XBOX := XboxModule/*.cpp
VCL := VehicleModule/*.cpp

all: pmmain lsrmain dmmain gpsmain xboxmain vehiclemain

pmmain: $(PM)
	${CC} ${IDIR} ${CMN} $(PM) -o bin/pm -w

lsrmain: $(LSR)
	${CC} ${IDIR} ${CMN} $(LSR) -o bin/lsr -w

dmmain: ${DM}
	${CC} ${DM} ${IDIR} ${CMN} -o bin/dm -framework OpenGL -framework GLUT -w

gpsmain: ${GPS}
	${CC} ${GPS} ${IDIR} ${CMN} -o bin/gps -w

xboxmain: ${XBOX}
	$(CC) $(XBOX) $(IDIR) $(CMN) -o bin/xbox -w -lSDL2main -lSDL2

vehiclemain: ${VCL}
	$(CC) $(VCL) ${IDIR} $(CMN) -o bin/vcl

clean:
	rm -rvf *.out

