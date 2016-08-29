CC=g++
FLAGS=--std=c++0x -lpthread
TARGETS=hellothread sharing bare waiting futures

all: $(TARGETS)

%: %.cpp
	$(CC) $? -o $@ $(FLAGS)

clean:
rm -f $(TARGETS) *.o
