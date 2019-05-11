FIRE_CXX ?= clang++
FIRE_CXXFLAGS ?= -O3 -std=gnu++2a -Wall -Werror -Wextra -fPIE -fPIC -fstack-protector-strong -fsanitize=safe-stack -fsanitize=safe-stack
FIRE_LDFLAGS ?= -fuse-ld=gold -flto -Wl,-z,relro -Wl,-z,now
FIRE_LDLIBS ?= -lglog

all: fireusage.a fireusage.o fireusage.so

objects = usage.o

fireusage.a: $(objects)
	ar rcs $@ $^

fireusage.o: $(objects)
	gold -z relro -z now -r --output=$@ $+

fireusage.so: $(objects)
	$(FIRE_CXX) $(FIRE_CXXFLAGS) $(FIRE_LDFLAGS) -shared -o $@ $+ $(FIRE_LDFLIBS)

%.o: %.cc *.h Makefile
	$(FIRE_CXX) $(FIRE_CXXFLAGS) -c -o $@ $<

clean:
	rm --force *.so *.o *.a

asan:
	$(MAKE) clean
	FIRE_CXXFLAGS="-O1 -g -fsanitize=address -fno-omit-frame-pointer -std=gnu++2a -fPIE -fPIC" $(MAKE) all

tsan:
	$(MAKE) clean
	FIRE_CXXFLAGS="-O1 -g -fsanitize=thread -std=gnu++2a -fPIE -fPIC" $(MAKE) all

ubsan:
	$(MAKE) clean
	FIRE_CXXFLAGS="-O1 -g -fsanitize=undefined -std=gnu++2a -fPIE -fPIC" $(MAKE) all
