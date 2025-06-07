CFLAGS = `pkg-config --cflags sdl2`
LIBS = `pkg-config --libs sdl2 epoxy` 
LINK = Material.cpp GraphicsEngine.cpp Input.cpp Game.cpp ../Camera.cpp

all:
	g++ $(CFLAGS) main.cpp $(LINK) $(LIBS) -o app

# Alternative version if you need specific includes
# INCLUDE = -I/path/to/your/headers
# all:
#	g++ $(CFLAGS) $(INCLUDE) main.cpp $(LINK) $(LIBS) -o app

clean:
	rm -f app

.PHONY: all clean
