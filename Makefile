SRC_DIR := src
POLICY_DIR := policies
SRC_FILES := $(wildcard $(SRC_DIR)/*pp)
POLICY_FILES := $(wildcard $(POLICY_DIR)/*pp)

all: cache.exe

cache.exe: $(SRC_FILES) $(POLICY_FILES) # TODO: need to recompile when G++FLAGS is changed
	g++ -std=c++11 $(SRC_FILES) $(POLICY_FILES) $(G++FLAGS) -o $@

clean:
	rm cache.exe
