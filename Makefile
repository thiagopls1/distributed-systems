TARGET_DIR = build

PROJECTS = ex1 ex2 lista1/ex1

$(TARGET_DIR)/%: %/main.c
	mkdir -p $(dir $@)
	gcc $< -o $@

all: $(addprefix $(TARGET_DIR)/,$(PROJECTS))

clean:
	rm -rf $(TARGET_DIR)

.PHONY: all clean
