SRC_DIR := src
POLICY_DIR := policies
SRC_FILES := $(wildcard $(SRC_DIR)/*)
POLICY_FILES := $(wildcard $(POLICY_DIR)/*)

all: cache.exe

cache.exe: $(SRC_FILES) $(POLICY_FILES)
	g++ $(SRC_FILES) $(POLICY_FILES)-o $@

clean:
	rm cache.exe
