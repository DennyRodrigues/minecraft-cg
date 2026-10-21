# minecraft-cg

Um cenário no estilo Minecraft feito em C com OpenGL no pipeline fixo, como material da disciplina de Computação Gráfica.

O código está dividido por assunto, e cada arquivo é comentado para ser lido do começo ao fim:

```
minecraft-cg/
├── Makefile          compilação (make, make run, make clean)
├── README.md
├── src/              código (.c)
├── include/          cabeçalhos (.h)
└── build/            gerado pelo make (.o, .d); não vai para o Git
```

| Arquivo | Conteúdo |
|---|---|
| [`src/main.c`](src/main.c) | Janela, registro dos callbacks, desenho do quadro e FPS |
| [`include/config.h`](include/config.h) | Constantes do jogo |
| [`include/graficos.h`](include/graficos.h) | Cabeçalhos do OpenGL, GLU e GLUT |
| [`src/mundo.c`](src/mundo.c) / [`include/mundo.h`](include/mundo.h) | Mapa, matriz do mundo, construção e desenho dos blocos |
| [`src/colisao.c`](src/colisao.c) / [`include/colisao.h`](include/colisao.h) | Posição do personagem e colisão com o mundo |
| [`src/camera.c`](src/camera.c) / [`include/camera.h`](include/camera.h) | Projeção, direção do olhar, primeira e terceira pessoa, desenho do personagem |
| [`src/entrada.c`](src/entrada.c) / [`include/entrada.h`](include/entrada.h) | Teclado, mouse, roda e atualização do movimento |

O comentário MÓDULOS, no início de `src/main.c`, explica por que o código é dividido assim (compilação separada, `.h` com declarações e `.c` com definições, `extern`). As funções centrais têm um comentário antes delas explicando o que fazem e por quê.

## O que tem no cenário

- Um terreno de 16 × 16 colunas de blocos.
- Uma montanha irregular com neve no topo.
- Um gramado com um laguinho.
- Uma praia e o mar.
- Uma árvore no formato do Minecraft.
- Duas nuvens.

Tudo é feito com **um único cubo de lado 1**, desenhado várias vezes em posições diferentes. Não há texturas nem iluminação: cada face recebe um brilho fixo (topo claro, laterais médias, base escura), o que dá a impressão de volume.

## Conteúdos das semanas 1 a 5

| Semana | Conteúdo | Onde aparece no código |
|---|---|---|
| 1 | Janela, laço principal, tempo de quadro, FPS no título, ESC | `main`, `desenha`, `atualiza` |
| 2 | Primitivas (`GL_QUADS`) e cor por face | `desenhaBloco`, `corDaFace` |
| 3 | Cubo 3D, buffer de profundidade, descarte de faces, instâncias com `glPushMatrix`/`glPopMatrix` | `desenhaBloco`, `blocoEm` |
| 4 | Perspectiva, correção de proporção, câmera com `gluLookAt`, navegação pelo teclado | `aplicaProjecao`, `redimensiona`, `posicionaCamera` |
| 5 | Teclado por estado e tempo de quadro; mouse com yaw e pitch e captura do cursor; matriz do mundo e colisão por caixa alinhada aos eixos; primeira e terceira pessoa; zoom pela roda | `teclado`, `tecladoSolto`, `atualiza`, `mundo`, `posicaoLivre`, `moveJogador`, `direcaoDoOlhar`, `movimentoMouse`, `posicionaCamera`, `desenhaJogador`, `rodaMouse` |

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

```bash
make run
```

| Comando | O que faz |
|---|---|
| `make` | Compila e gera `./mine` |
| `make run` | Compila, se precisar, e roda |
| `make clean` | Apaga os arquivos gerados (`.o`, `.d`, executável) |

O `make` só recompila os arquivos que mudaram. No Windows, use o MSYS2 com MinGW: o `Makefile` detecta o sistema e troca o nome das bibliotecas e do executável (`mine.exe`).

Sem o `make`, o comando equivalente no Linux é:
```bash
gcc -Iinclude src/*.c -o mine -lGLEW -lGL -lGLU -lglut -lm
```

## Controles

| Entrada | Ação | Leitura |
|---|---|---|
| W / S | Andar para frente / para trás | estado |
| A / D | Andar para a esquerda / para a direita | estado |
| R (segurando) | Correr (dobra a velocidade) | estado |
| ← / → | Girar o olhar | estado |
| Espaço ou ↑ / Shift ou ↓ | Subir / descer | estado |
| Mouse | Direção do olhar (já começa capturado) | evento de movimento |
| Roda ou + / - | Aproximar / afastar (campo de visão) | evento |
| Tab | Liberar / capturar o cursor (clicar na janela também captura) | evento |
| F5 ou V | Alternar primeira / terceira pessoa | evento |
| P | Alternar perspectiva / ortográfica | evento |
| ESC | Abrir / fechar o menu de pausa, com as instruções de movimentação | evento |
| Q (com o menu aberto) | Sair | evento |

A coluna "Leitura" diz como cada entrada é tratada: ações contínuas, que duram enquanto a tecla está pressionada, são lidas **por estado** a cada quadro; ações discretas, que acontecem uma vez por toque, são tratadas **por evento**.

O personagem começa no gramado, olhando para a montanha, com a câmera em terceira pessoa. Não há gravidade: para subir um degrau, use Espaço ou ↑. O mouse já começa controlando o olhar; Tab libera o cursor, e Tab ou um clique na janela o capturam de novo.

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

1. `construirMundo`, chamada uma única vez no início, percorre os 16 × 16 quadrados do mapa e chama `colunaEm(x, z)` para cada um.
2. `colunaEm` lê a altura `h` no mapa e empilha blocos de `y = 0` até o topo. Para cada andar, uma sequência de `if` escolhe o tipo comparando `y` com `h`:
   - acima do chão e até o nível da água: **água**
   - coluna baixa: **areia**
   - topo de coluna alta: **neve**
   - topo: **grama**
   - dois andares abaixo do topo: **terra**
   - o resto: **pedra**
3. Cada bloco é gravado com `colocaBloco(tipo, x, y, z)` na matriz `mundo`.
4. `arvoreEm` e `nuvemEm` usam o mesmo `colocaBloco` para montar a árvore e as nuvens.
5. A cada quadro, `desenhaCena` percorre a matriz e chama `blocoEm(tipo, x, y, z)` para cada posição ocupada, que desloca a origem com `glTranslatef` e desenha o cubo de sempre ali.

### A matriz do mundo

`mundo[x][y][z]` guarda o tipo de cada bloco (ou `VAZIO`) numa grade de 16 × 16 × 16. **Atenção à ordem**: o `mapa` é `[z][x]`, para parecer o mundo visto de cima, mas o `mundo` segue a ordem dos eixos, `[x][y][z]`. Todo acesso passa por `colocaBloco` e `blocoNoMundo`, que também testam os limites.

A matriz é o que permite a colisão: a célula que contém um ponto `(px, py, pz)` é `(floor(px), floor(py), floor(pz))`, então saber se o personagem bate num bloco é consultar as poucas células que a caixa dele ocupa (`posicaoLivre`). Fora do mundo conta como sólido, e a água não é sólida.

## Experimente

- **Mudar o terreno**: edite os números da matriz `mapa`. Um `0` no meio do gramado vira um lago; um `9` vira uma torre.
- **Mais ou menos neve**: altere `NIVEL_NEVE`.
- **Outra árvore**: acrescente `arvoreEm(x, z);` em `construirMundo`, num quadrado de grama.
- **Outra nuvem**: acrescente `nuvemEm(x, z, largura, fundo);` em `construirMundo`.
- **Um tipo de bloco novo**: crie um `#define` (por exemplo, `#define MADEIRA 20`; de 10 a 16 já são as cores do personagem) e acrescente um `case` com a cor em `corDaFace`.
- **Quebrar um bloco**: `colocaBloco(VAZIO, x, y, z)` depois de `construirMundo`; o desenho e a colisão passam a ver o buraco.
