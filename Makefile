# GPL 2015 Jake Sebastian-Jones

LD_LIBS=-ldl
CXXFLAGS+=-g

all: powerplug fooplugin.so barplugin.so

clean:
	rm -f *.o *.out *.a *.so powerplug

powerplug: Main.o PluginManager.o FooComponent.o BarComponent.o
	$(CXX) -o $@ $^ $(LD_LIBS)

fooplugin.so: FooPlugin.o PluginManager.o FooComponent.o
	$(CXX) -shared -o $@ $^

barplugin.so: BarPlugin.o PluginManager.o BarComponent.o
	$(CXX) -shared -o $@ $^

%.o: %.cpp %.hpp
	$(CXX) -c $(CXXFLAGS) -o $@ $< -fPIC

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $< -fPIC

