
CXX      ?= g++
CXXFLAGS += -std=c++0x -Iasync
LDFLAGS  += -luv
ifeq ($(OS),Windows_NT)
CXXFLAGS += -DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0600
LDFLAGS  += -lws2_32 -lpsapi -liphlpapi -lshell32
endif
prefix   ?= /usr/local

async_HEADERS 	   = async/async.h \
					 async/EventLoop.h \
					 async/Timer.h \
					 async/Idle.h \
					 async/IpAddress.h

async_SOURCES      = async/EventLoop.cpp \
					 async/Timer.cpp \
					 async/Idle.cpp \
					 async/IpAddress.cpp

async_test_SOURCES = test/main.cpp \
				     test/basic_tests.cpp \
					 test/udp_tests.cpp

all: libasync.a

libasync.a: $(patsubst %.cpp, .obj/%.o, $(async_SOURCES))
	ar rvs $@ $^

check: async-test
	./async-test
	
async-test: $(patsubst %.cpp, .obj/%.o, $(async_test_SOURCES)) libasync.a
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -lrtest -o $@

clean:
	rm -rf glm-test* .obj
	
install: libasync.a
	mkdir -p $(prefix)/include/async
	cp $(async_HEADERS) $(prefix)/include/async
	mkdir -p $(prefix)/libs
	cp libasync.a $(prefix)/libs

uninstall:
	rm -r $(prefix)/include/async
	rm $(prefix)/libs/libasync.a
	
.obj/%.o : %.cpp	
	mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) -MD -c $^ -o $@

ifneq "$(MAKECMDGOALS)" "clean"
-include $(patsubst %.cpp, obj/%.d, $(async_SOURCES))
-include $(patsubst %.cpp, obj/%.d, $(async_test_SOURCES))
endif
