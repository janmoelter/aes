#CXX=clang++
CXX=g++
CXXFLAGS=-g -std=c++14 -O3
RM=rm -rf
MKDIR=mkdir -p

SRCS=$(wildcard src/*.cpp)
OBJS=$(subst src/,obj/,$(subst .cpp,.o,$(SRCS)))

OUT=aes

.PHONY : all
all : bin/$(OUT)

.PHONY: clean
clean:
	@$(RM) obj/

.PHONY: distclean
distclean:
	@$(RM) obj/ bin/


bin/$(OUT): $(OBJS)
	@$(MKDIR) bin/
	$(CXX) -o bin/$(OUT) $^

obj/%.o: src/%.cpp
	@$(MKDIR) obj/
	$(CXX) $(CXXFLAGS) -o $@ -c $<
