TARGET_DIR = ./build
PROJECTS = ex1

$(TARGET_DIR)/%: %/main.c
	mkdir -p $(TARGET_DIR)
	gcc $< -o $@

ado1: build/ado1

all: $(PROJECTS)

clean:
	rm -rf $(TARGET_DIR)

.PHONY: all clean $(PROJECTS)
