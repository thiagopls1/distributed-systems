TARGET_DIR = build

# Caminhos completos dos projetos
PROJECTS = ex1 ex2 lista1/ex1

# Regra genérica
$(TARGET_DIR)/%: %/main.c
	mkdir -p $(dir $@)
	gcc $< -o $@

# Alvo principal
all: $(addprefix $(TARGET_DIR)/,$(PROJECTS))

clean:
	rm -rf $(TARGET_DIR)

.PHONY: all clean