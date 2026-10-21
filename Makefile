# Makefile do projeto Minecraft
#
#   make              compila (gera ./mine)
#   make run          compila, se precisar, e roda
#   make clean        apaga os arquivos gerados
#
# Cada .c é compilado separadamente num .o, e no fim os .o são ligados
# com as bibliotecas no executável. O make compara as datas dos arquivos
# e só recompila o que mudou.
#
# Pastas:
#   src/      código (.c)
#   include/  cabeçalhos (.h); -Iinclude faz o #include "x.h" achá-los
#   build/    arquivos gerados na compilação (.o, .d)
# O executável fica na raiz, para rodar com ./mine.

CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -MMD -MP -Iinclude
FONTES  = $(wildcard src/*.c)
OBJETOS = $(patsubst src/%.c,build/%.o,$(FONTES))

# No Windows (MSYS2/MinGW) as bibliotecas têm outros nomes
ifeq ($(OS),Windows_NT)
    PROGRAMA = mine.exe
    LIBS     = -lglew32 -lfreeglut -lopengl32 -lglu32
else
    PROGRAMA = mine
    LIBS     = -lGLEW -lGL -lGLU -lglut -lm
endif

all: $(PROGRAMA)

# ligação: junta os .o e as bibliotecas no executável
$(PROGRAMA): $(OBJETOS)
	$(CC) $(OBJETOS) -o $@ $(LIBS)

# compilação: cada .c vira um .o. -MMD gera um .d com os .h que o .c
# inclui, para que mudar um .h recompile quem depende dele
build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

run: $(PROGRAMA)
	./$(PROGRAMA)

clean:
	rm -rf build $(PROGRAMA)

.PHONY: all run clean

-include $(OBJETOS:.o=.d)
