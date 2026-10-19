# minecraft-cg

Um cenário no estilo Minecraft feito em C com OpenGL no pipeline fixo, como material da disciplina de Computação Gráfica.

O código inteiro está em um único arquivo, [`mine.c`](mine.c), comentado para ser lido do começo ao fim.

## O que tem no cenário

- Um terreno de 16 × 16 colunas de blocos.
- Uma montanha irregular com neve no topo.
- Um gramado com um laguinho.
- Uma praia e o mar.
- Uma árvore no formato do Minecraft.
- Duas nuvens.

Tudo é feito com **um único cubo de lado 1**, desenhado várias vezes em posições diferentes. Não há texturas nem iluminação: cada face recebe um brilho fixo (topo claro, laterais médias, base escura), o que dá a impressão de volume.

## Conteúdos das semanas 1 a 4

| Semana | Conteúdo | Onde aparece no código |
|---|---|---|
| 1 | Janela, laço principal, tempo de quadro, FPS no título, ESC | `main`, `desenha`, `ocioso` |
| 2 | Primitivas (`GL_QUADS`) e cor por face | `desenhaBloco`, `corDaFace` |
| 3 | Cubo 3D, buffer de profundidade, descarte de faces, instâncias com `glPushMatrix`/`glPopMatrix` | `desenhaBloco`, `blocoEm` |
| 4 | Perspectiva, correção de proporção, câmera com `gluLookAt`, navegação pelo teclado | `redimensiona`, `desenha`, `teclado` |

## Dependências

- Compilador C (`gcc`)
- OpenGL, GLU, GLUT (freeglut) e GLEW

**Ubuntu / Debian**
```bash
sudo apt install build-essential freeglut3-dev libglew-dev
```

**Arch / Manjaro**
```bash
sudo pacman -S gcc freeglut glew glu
```

**Windows**: use o MinGW com as bibliotecas freeglut e GLEW instaladas.

## Como compilar e rodar

**Linux**
```bash
gcc mine.c -o mine -lGLEW -lGL -lGLU -lglut -lm
./mine
```

**Windows (MinGW)**
```bash
gcc mine.c -o mine.exe -lglew32 -lfreeglut -lopengl32 -lglu32
mine.exe
```

## Controles

| Tecla | Ação |
|---|---|
| W / S | Andar para frente / para trás |
| A / D | Andar para a esquerda / para a direita |
| ← / → | Girar a câmera |
| ↑ / ↓ | Subir / descer |
| ESC | Sair |

A câmera começa sobre o mar, olhando para a terra.

## Como o cenário é montado

Os eixos do mundo são:

- **x**: da esquerda (0) para a direita (15)
- **y**: altura, a partir de 0, crescendo para cima
- **z**: do fundo do mapa (0) para a frente (15), onde fica o mar

### O mapa

O terreno vem da matriz `mapa`, que mostra o mundo **visto de cima**. Cada linha é um valor de `z`, cada número na linha é um valor de `x`, e o número em si é a **altura** daquela coluna de blocos:

| Valor | Significado |
|---|---|
| 0 | Mar (areia no fundo, água por cima) |
| 1 | Praia |
| 2 ou mais | Terra firme com grama no topo |
| 6 ou mais | Topo coberto de neve (`NIVEL_NEVE`) |

O acesso é `mapa[z][x]`, com a linha antes da coluna. Assim a tabela aparece no código do mesmo jeito que o mundo visto de cima.

### Do mapa aos blocos

1. `desenhaCena` percorre os 16 × 16 quadrados do mapa e chama `colunaEm(x, z)` para cada um.
2. `colunaEm` lê a altura `h` no mapa e empilha blocos de `y = 0` até o topo. Para cada andar, uma sequência de `if` escolhe o tipo comparando `y` com `h`:
   - acima do chão e até o nível da água: **água**
   - coluna baixa: **areia**
   - topo de coluna alta: **neve**
   - topo: **grama**
   - dois andares abaixo do topo: **terra**
   - o resto: **pedra**
3. `blocoEm(tipo, x, y, z)` desloca a origem com `glTranslatef` e desenha o cubo de sempre ali.
4. `arvoreEm` e `nuvemEm` usam o mesmo `blocoEm` para montar a árvore e as nuvens.

Os blocos não ficam guardados em lugar nenhum: a cada quadro a cena é recalculada e redesenhada a partir do mapa.

## Experimente

- **Mudar o terreno**: edite os números da matriz `mapa`. Um `0` no meio do gramado vira um lago; um `9` vira uma torre.
- **Mais ou menos neve**: altere `NIVEL_NEVE`.
- **Outra árvore**: acrescente `arvoreEm(x, z);` em `desenhaCena`, num quadrado de grama.
- **Outra nuvem**: acrescente `nuvemEm(x, z, largura, fundo);` em `desenhaCena`.
- **Um tipo de bloco novo**: crie um `#define` (por exemplo, `#define MADEIRA 10`) e acrescente um `case` com a cor em `corDaFace`.
