SRC_DIR := src
SRC_FILES := $(wildcard $(SRC_DIR)/*)
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)

all: cache.exe

cache.exe: $(SRC_FILES)
	g++ $(CPP_FILES) -o $@

clean:
	rm cache.exe