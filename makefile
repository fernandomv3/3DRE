OBJS = $(BUILDDIR)/main.o \
	   $(BUILDDIR)/Vec4.o \
	   $(BUILDDIR)/Mat4.o \
	   $(BUILDDIR)/Euler.o \
	   $(BUILDDIR)/Quaternion.o \
	   $(BUILDDIR)/GLProgram.o \
	   $(BUILDDIR)/Object3D.o \
	   $(BUILDDIR)/Light.o \
	   $(BUILDDIR)/Camera.o \
	   $(BUILDDIR)/MathUtils.o \
	   $(BUILDDIR)/Texture.o \
	   $(BUILDDIR)/Scene.o \
	   $(BUILDDIR)/Geometry.o \
	   $(BUILDDIR)/Material.o \
	   $(BUILDDIR)/Mesh.o \
	   $(BUILDDIR)/Renderer.o

INCDIR = include
SRCDIR = src
BUILDDIR = build
BINDIR = bin
CC = g++
DEBUG = -g -Wall
IFLAGS = -I $(INCDIR) 
SDLFLAGS = -lSDL2 
CFLAGS = -c $(DEBUG) $(IFLAGS) -std=c++11
GLEWFLAGS = -lGLEW
OPENGLFLAGS = -lGL
LFLAGS = $(DEBUG) $(GLEWFLAGS) $(SDLFLAGS) $(OPENGLFLAGS) 

vpath %.cpp $(SRCDIR)
vpath %.h $(INCDIR)

$(BINDIR)/3DRE: $(OBJS)
	@echo generating executable...
	$(CC) -o $(BINDIR)/3DRE $(OBJS) $(LFLAGS)

$(BUILDDIR)/%.o: %.cpp %.h
	@echo compiling $<
	$(CC) -o $(BUILDDIR)/$*.o $< $(CFLAGS) 

%.cpp: %.h

Object3D.h: Vec4.h Mat4.h Euler.h Quaternion.h

Camera.h: Object3D.h

Material.h: Vec4.h Texture.h

Mesh.h: Object3D.h Geometry.h Material.h

Light.h: Mesh.h

Renderer.h: Scene.h Camera.h GLProgram.h

Scene.h: Light.h

$(BUILDDIR)/main.o: $(SRCDIR)/main.cpp $(INCDIR)/Camera.h $(INCDIR)/Scene.h $(INCDIR)/Geometry.h $(INCDIR)/Material.h $(INCDIR)/Mesh.h $(INCDIR)/Renderer.h
	@echo compiling 3DRE binary
	$(CC) -o $(BUILDDIR)/main.o $(CFLAGS) $(SRCDIR)/main.cpp
	
clean:
	@echo cleaning project...
	rm $(BUILDDIR)/*.o $(BINDIR)/3DRE
	