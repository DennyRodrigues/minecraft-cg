# Makefile do projeto Minecraft
#
#   make              compila (gera ./mine)
#   make run          compila, se precisar, e roda
#   make clean        apaga os arquivos gerados
#   make ajuda        mostra este resumo
#
# Versões de demonstração (ver config.h): passe as chaves na linha de
# comando. Quando as chaves mudam, tudo é recompilado sozinho, e
# "make run" sem chaves volta à versão final.
#
#   make run USAR_DT=0 EXPERIMENTO_TIMER=16
#   make run USAR_DT=0 EXPERIMENTO_TIMER=50
#   make run VERSAO_COLISAO=1
#   make run LIMITAR_PITCH=0
#
# Como funciona: cada .c é compilado separadamente num .o, e no fim os
# .o são ligados com as bibliotecas no executável. O make compara as
# datas dos arquivos e só recompila o que mudou.
#
# Pastas:
#   src/      código (.c)
#   include/  cabeçalhos (.h); -Iinclude faz o #include "x.h" achá-los
#   build/    arquivos gerados na compilação (.o, .d, .opcoes)
#   docs/     documentação local (fora do Git, ver .gitignore)
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

# Cada chave passada na linha de comando vira um -DCHAVE=valor
CHAVES  = EXPERIMENTO_TIMER USAR_DT VERSAO_COLISAO LIMITAR_PITCH
DEFINES = $(foreach c,$(CHAVES),$(if $($(c)),-D$(c)=$($(c))))

all: $(PROGRAMA)

# ligação: junta os .o e as bibliotecas no executável
$(PROGRAMA): $(OBJETOS)
	$(CC) $(OBJETOS) -o $@ $(LIBS)

# compilação: cada .c vira um .o. -MMD gera um .d com os .h que o .c
# inclui, para que mudar um .h recompile quem depende dele
build/%.o: src/%.c build/.opcoes
	$(CC) $(CFLAGS) $(DEFINES) -c $< -o $@

# .opcoes guarda as opções da última compilação. Só é reescrito quando
# elas mudam, e aí todos os .o ficam desatualizados e são refeitos.
# Sem isso, trocar USAR_DT=0 por USAR_DT=1 não recompilaria nada.
build/.opcoes: FORCE
	@mkdir -p build
	@echo '$(CFLAGS) $(DEFINES)' | cmp -s - $@ || echo '$(CFLAGS) $(DEFINES)' > $@

run: $(PROGRAMA)
	./$(PROGRAMA)

clean:
	rm -rf build $(PROGRAMA)

ajuda:
	@echo "make                 compila (gera ./$(PROGRAMA))"
	@echo "make run             compila, se precisar, e roda"
	@echo "make clean           apaga os arquivos gerados"
	@echo ""
	@echo "Versões de demonstração (ver config.h):"
	@echo "make run USAR_DT=0 EXPERIMENTO_TIMER=16   passo fixo, máquina rápida"
	@echo "make run USAR_DT=0 EXPERIMENTO_TIMER=50   passo fixo, máquina lenta"
	@echo "make run EXPERIMENTO_TIMER=50             tempo de quadro, máquina lenta"
	@echo "make run VERSAO_COLISAO=1                 colisão só no ponto central"
	@echo "make run VERSAO_COLISAO=2                 colisão com a caixa, sem deslizar"
	@echo "make run VERSAO_COLISAO=3                 eixos juntos: trava na diagonal"
	@echo "make run LIMITAR_PITCH=0                  pitch sem limite"
	@echo "make run                                  versão final"

.PHONY: all run clean ajuda FORCE
FORCE:

-include $(OBJETOS:.o=.d)
