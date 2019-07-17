
#GPS = GPSModule/GPSMain.cpp
PM = PMModule/PMMain.cpp
DM = DisplayModule/*.cpp
LSR = LaserModule/LaserMain.cpp
IDIR := -I/usr/local/include -I./Include

# Make all the executables and put them in bin
all: pm lsr dm

# Make the process management module
pm: $(PM)
	g++ $(PM) $(IDIR) -o bin/pm

# Make the gps module
gps: $(LSR)
	g++ $(LSR) $(IDIR) -o bin/lsr

# Make the display module
dm: $(DM)
	g++ $(DM) $(IDIR) -o bin/dm -framework OpenGL -framework GLUT

# TODO - add other modules here

# Clean up
clean:
	rm -rf bin/*