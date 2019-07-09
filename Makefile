FLAGS = 
GPS = GPSModule/GPSMain.cpp
PM = PMModule/PMMain.cpp
DM = DisplayModule/*.cpp
CMN = Common/*.cpp
INC = Include/*.h

# Make all the executables and put them in bin
all: pm gps dm

# Make the process management module
pm: $(PM)
	g++ $(PM) $(INC) $(CMN) -o bin/pm

# Make the gps module
gps: $(GPS)
	g++ $(GPS) $(INC) $(CMN)-o bin/gps

# Make the display module
dm: $(DM)
	g++ $(DM) $(INC) $(CMN) -o bin/dm -framework OpenGL -framework GLUT

# TODO - add other modules here

# Clean up
clean:
	rm -rf bin/*