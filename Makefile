TARGET_DIR = ./build
PROJECTS = ex1

$(TARGET_DIR)/%: %/main.c
	mkdir -p $(TARGET_DIR)
	gcc $< -o $@

ex1: build/ex1
ex2: build/ex2

build: $(PROJECTS)

clean:
	rm -rf $(TARGET_DIR)

.PHONY: all clean $(PROJECTS)
