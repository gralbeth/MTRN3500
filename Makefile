LIBS = -framework OpenGL -framework GLUT
SRC = *.cpp
LIBDIR = -L∼/lib
INCDIR = -I∼/include
all:
	g++ -o display $(SRC) $(LIBS) $(LIBDIR) $(INCDIR) -g

PM: 
	g++ -o pm $(SRC) $(LIBS) $(LIBDIR) $(INCDIR) -g