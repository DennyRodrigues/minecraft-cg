/*
 * Projeto Minecraft
 *
 * Reúne o que foi visto nas semanas 1 a 4:
 *   Semana 1: janela, laço principal, tempo de quadro, FPS no título, ESC
 *   Semana 2: primitivas (GL_QUADS) e cor por face
 *   Semana 3: cubo 3D, buffer de profundidade, descarte de faces,
 *             transformações e instâncias com glPushMatrix/glPopMatrix
 *   Semana 4: perspectiva, correção de proporção, câmera com gluLookAt
 *             e navegação pelo teclado
 *
 * Cenário: terreno 16 x 16 com uma montanha, uma praia e o mar,
 * uma árvore e duas nuvens. Tudo é feito com um único cubo de lado 1,
 * desenhado várias vezes em posições diferentes.
 *
 * Eixos do mundo:
 *   x  esquerda (0) para a direita (15)
 *   y  altura: 0 é o fundo, cresce para cima
 *   z  fundo do mapa (0) para a frente (15), onde fica o mar
 *
 * Controles:
 *   W / S        andar para frente / para trás
 *   A / D        andar para a esquerda / para a direita
 *   Setas < >    girar a câmera
 *   Setas ^ v    subir / descer
 *   ESC          sair
 *
 * Compilar e rodar (Linux):
 *   gcc mine.c -o mine -lGLEW -lGL -lGLU -lglut -lm && ./mine
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>            /* carrega as funções do OpenGL */
#include <GL/glut.h>            /* janela, teclado e laço principal */
#include <GL/glu.h>             /* gluPerspective e gluLookAt */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265f

/* ------------------------------------------------------------------ */
/* Tipos de bloco                                                     */
/* Cada tipo é só um número com nome. O número não muda a forma do    */
/* bloco (todos são o mesmo cubo); ele só escolhe a cor, lá em        */
/* corDaFace(). Para criar um tipo novo: um #define aqui e um case    */
/* com a cor em corDaFace().                                          */
/* ------------------------------------------------------------------ */

#define GRAMA  1
#define TERRA  2
#define PEDRA  3
#define TRONCO 4
#define FOLHA  5
#define AREIA  6
#define AGUA   7
#define NUVEM  8
#define NEVE   9

/* ------------------------------------------------------------------ */
/* Tamanho do mundo                                                   */
/* ------------------------------------------------------------------ */

#define LADO        16  /* o chão tem LADO x LADO quadrados (colunas) */
#define NIVEL_AGUA  1   /* colunas com altura até este valor são praia ou
                           mar; acima do chão delas, até esta altura, vai
                           água */
#define NIVEL_NEVE  6   /* colunas com altura a partir deste valor têm
                           neve no topo em vez de grama */
#define ALTURA_NUVEM 14 /* altura (y) das nuvens; bem acima da neve, para
                           não se confundir com ela */

/* ------------------------------------------------------------------ */
/* Câmera                                                             */
/* A câmera é só uma posição (camX, camY, camZ) e um ângulo de giro.  */
/* O teclado altera essas variáveis; desenha() as usa para montar a   */
/* vista a cada quadro.                                               */
/* ------------------------------------------------------------------ */

float camX = 8.0f;      /* começa sobre o mar, no meio em x, */
float camY = 5.0f;      /* um pouco acima do chão,           */
float camZ = 24.0f;     /* e fora do mapa, olhando para ele  */
float camAngulo = 0.0f; /* giro em graus; 0 olha para -z (para dentro do mapa) */

#define PASSO 0.5f      /* quanto anda a cada evento de tecla */
#define GIRO  4.0f      /* quantos graus gira a cada evento de tecla */

/* ------------------------------------------------------------------ */
/* Projeção (semana 4)                                                */
/* Define a "lente" da câmera: campo de visão de 60 graus, proporção  */
/* igual à da janela (para o cubo não sair esticado) e o intervalo    */
/* de distâncias visíveis, de 0.1 a 200.                              */
/* A GLUT chama esta função ao abrir a janela e sempre que ela muda   */
/* de tamanho.                                                        */
/* ------------------------------------------------------------------ */

void redimensiona(int largura, int altura)
{
    if (altura == 0) altura = 1;            /* evita divisão por zero */

    glViewport(0, 0, largura, altura);      /* desenha na janela inteira */

    glMatrixMode(GL_PROJECTION);            /* as próximas operações mexem na lente */
    glLoadIdentity();
    gluPerspective(60.0, (double)largura / altura, 0.1, 200.0);

    glMatrixMode(GL_MODELVIEW);             /* volta para a matriz dos objetos */
}

/* ------------------------------------------------------------------ */
/* Teclado (semana 4)                                                 */
/* Movimentação rudimentar: um passo fixo a cada evento de tecla.     */
/* Segurar a tecla depende da repetição do sistema operacional, por   */
/* isso o movimento hesita no início. Na semana 5 isto será trocado   */
/* por um vetor de teclas pressionadas, multiplicado pelo tempo de    */
/* quadro.                                                            */
/* Na GLUT, letras chegam em teclado() e setas em especial().         */
/* ------------------------------------------------------------------ */

void teclado(unsigned char tecla, int x, int y)
{
    (void)x; (void)y;                       /* posição do mouse: não usada */

    /* Para andar "para frente" é preciso saber para onde a câmera olha.
       Com o ângulo em radianos, seno e cosseno dão essa direção no chão
       (plano x-z). A direção "para a direita" é a de frente girada 90°. */
    float rad = camAngulo * PI / 180.0f;
    float frenteX = sinf(rad);
    float frenteZ = -cosf(rad);
    float direitaX = cosf(rad);
    float direitaZ = sinf(rad);

    switch (tecla) {
        case 27: exit(0);                   /* 27 é o código da tecla ESC */

        case 'w': case 'W': camX += frenteX * PASSO;  camZ += frenteZ * PASSO;  break;
        case 's': case 'S': camX -= frenteX * PASSO;  camZ -= frenteZ * PASSO;  break;
        case 'd': case 'D': camX += direitaX * PASSO; camZ += direitaZ * PASSO; break;
        case 'a': case 'A': camX -= direitaX * PASSO; camZ -= direitaZ * PASSO; break;
    }
}

void especial(int tecla, int x, int y)
{
    (void)x; (void)y;

    switch (tecla) {
        case GLUT_KEY_RIGHT: camAngulo += GIRO; break;   /* gira para a direita */
        case GLUT_KEY_LEFT:  camAngulo -= GIRO; break;   /* gira para a esquerda */
        case GLUT_KEY_UP:    camY += PASSO;     break;   /* sobe */
        case GLUT_KEY_DOWN:  camY -= PASSO;     break;   /* desce */
    }
}

/* ------------------------------------------------------------------ */
/* Cores dos blocos                                                   */
/* Escolhe a cor de UMA face do cubo, a partir de:                    */
/*   tipo    qual bloco é (GRAMA, PEDRA...), define a cor base        */
/*   ehTopo  1 se for a face de cima; a grama usa isso para ser verde */
/*           só no topo e marrom nas laterais                         */
/*   brilho  quanto clarear ou escurecer a face. Ainda não há         */
/*           iluminação (semana 6), então cada face recebe um brilho  */
/*           fixo: topo claro, laterais médias, base escura. Isso dá  */
/*           a impressão de volume.                                   */
/*   tom     pequena variação entre blocos vizinhos (xadrez), para    */
/*           dar para enxergar onde um bloco termina e o outro começa */
/* A cor final é a cor base multiplicada por brilho e tom.            */
/* ------------------------------------------------------------------ */

void corDaFace(int tipo, int ehTopo, float brilho, float tom)
{
    float r, g, b;                          /* vermelho, verde, azul, de 0 a 1 */

    switch (tipo) {
        case GRAMA:
            if (ehTopo) { r = 0.37f; g = 0.62f; b = 0.21f; }   /* verde no topo */
            else        { r = 0.53f; g = 0.38f; b = 0.26f; }   /* terra nas laterais */
            break;
        case TERRA:  r = 0.53f; g = 0.38f; b = 0.26f; break;
        case PEDRA:  r = 0.49f; g = 0.49f; b = 0.49f; break;
        case TRONCO: r = 0.40f; g = 0.31f; b = 0.19f; break;
        case FOLHA:  r = 0.23f; g = 0.48f; b = 0.13f; break;
        case AREIA:  r = 0.86f; g = 0.81f; b = 0.58f; break;
        case AGUA:   r = 0.20f; g = 0.40f; b = 0.85f; break;
        case NUVEM:  r = 1.00f; g = 1.00f; b = 1.00f; break;
        case NEVE:   r = 0.95f; g = 0.97f; b = 1.00f; break;   /* branco levemente azulado */
        default:    r = 1.00f; g = 0.00f; b = 1.00f; break;   /* rosa = tipo desconhecido */
    }

    glColor3f(r * brilho * tom, g * brilho * tom, b * brilho * tom);
}

/* ------------------------------------------------------------------ */
/* Um bloco: cubo de lado 1, do canto (0,0,0) ao canto (1,1,1).       */
/* São 6 faces, cada uma um GL_QUADS de 4 vértices.                   */
/* Os vértices de cada face estão em sentido anti-horário quando      */
/* vistos de fora do cubo. É assim que o OpenGL sabe qual lado da     */
/* face é a frente, e o GL_CULL_FACE pode pular as faces viradas de   */
/* costas para a câmera (semanas 2 e 3).                              */
/* Este cubo sempre fica na origem; quem o leva para outro lugar é    */
/* blocoEm(), logo abaixo.                                            */
/* ------------------------------------------------------------------ */

void desenhaBloco(int tipo, float tom)
{
    glBegin(GL_QUADS);

    /* frente (z = 1) */
    corDaFace(tipo, 0, 0.80f, tom);
    glVertex3f(0, 0, 1); glVertex3f(1, 0, 1); glVertex3f(1, 1, 1); glVertex3f(0, 1, 1);

    /* trás (z = 0) */
    corDaFace(tipo, 0, 0.80f, tom);
    glVertex3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0); glVertex3f(1, 1, 0);

    /* direita (x = 1) */
    corDaFace(tipo, 0, 0.65f, tom);
    glVertex3f(1, 0, 1); glVertex3f(1, 0, 0); glVertex3f(1, 1, 0); glVertex3f(1, 1, 1);

    /* esquerda (x = 0) */
    corDaFace(tipo, 0, 0.65f, tom);
    glVertex3f(0, 0, 0); glVertex3f(0, 0, 1); glVertex3f(0, 1, 1); glVertex3f(0, 1, 0);

    /* topo (y = 1): a única face com ehTopo = 1 */
    corDaFace(tipo, 1, 1.00f, tom);
    glVertex3f(0, 1, 1); glVertex3f(1, 1, 1); glVertex3f(1, 1, 0); glVertex3f(0, 1, 0);

    /* base (y = 0) */
    corDaFace(tipo, 0, 0.50f, tom);
    glVertex3f(0, 0, 0); glVertex3f(1, 0, 0); glVertex3f(1, 0, 1); glVertex3f(0, 0, 1);

    glEnd();
}

/* ------------------------------------------------------------------ */
/* Instância (semana 3): desenha um bloco do tipo pedido na posição   */
/* (x, y, z) do mundo. Todas as outras funções da cena acabam aqui.   */
/*                                                                    */
/*   glPushMatrix   guarda a transformação atual                      */
/*   glTranslatef   desloca a origem para (x, y, z)                   */
/*   desenhaBloco   desenha o cubo de sempre, agora na nova origem    */
/*   glPopMatrix    desfaz o deslocamento, para o próximo bloco       */
/*                  partir do mesmo ponto                             */
/*                                                                    */
/* Como o cubo tem lado 1 e as posições são inteiras, cada bloco      */
/* termina exatamente onde o vizinho começa: eles se encaixam numa    */
/* grade, sem buracos.                                                */
/* ------------------------------------------------------------------ */

void blocoEm(int tipo, int x, int y, int z)
{
    /* soma par: tom normal; soma ímpar: um pouco mais escuro (xadrez) */
    float tom = ((x + y + z) % 2 == 0) ? 1.0f : 0.92f;

    glPushMatrix();
    glTranslatef((float)x, (float)y, (float)z);
    desenhaBloco(tipo, tom);
    glPopMatrix();
}

/* ------------------------------------------------------------------ */
/* Mapa: o mundo visto de cima. Cada número é a altura de uma coluna  */
/* de blocos. Cada linha é um valor de z (de trás para a frente) e    */
/* cada número na linha é um valor de x (da esquerda para a direita). */
/*                                                                    */
/*   0     mar (areia no fundo, água por cima)                        */
/*   1     praia                                                      */
/*   2+    terra firme com grama no topo; quanto maior, mais alto     */
/*   6+    (NIVEL_NEVE) topo coberto de neve                          */
/*                                                                    */
/* O mapa guarda só a ALTURA de cada coluna. O tipo de cada bloco da  */
/* coluna (grama, terra, pedra...) é decidido depois, em colunaEm().  */
/*                                                                    */
/* Para mudar o cenário, basta editar os números. Experimente pôr um  */
/* 0 no meio do gramado (lago) ou um 9 (torre).                       */
/* ------------------------------------------------------------------ */

int mapa[LADO][LADO] = {
/*  x: 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 */
    {  3, 3, 4, 4, 5, 5, 5, 4, 4, 3, 3, 2, 2, 2, 3, 3 },   /* z =  0 */
    {  3, 4, 5, 5, 6, 6, 5, 5, 4, 3, 2, 2, 2, 3, 3, 3 },   /* z =  1 */
    {  4, 5, 6, 6, 7, 7, 6, 5, 4, 3, 2, 2, 2, 2, 3, 4 },   /* z =  2 */
    {  4, 5, 6, 7, 8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 2, 3 },   /* z =  3  pico (8) */
    {  4, 5, 6, 7, 7, 7, 6, 5, 3, 3, 2, 2, 2, 2, 2, 2 },   /* z =  4 */
    {  3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 2, 2, 2, 2, 2, 2 },   /* z =  5 */
    {  3, 4, 4, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 2, 2, 2 },   /* z =  6 */
    {  3, 3, 4, 4, 4, 4, 3, 3, 2, 2, 1, 0, 0, 1, 2, 2 },   /* z =  7  laguinho */
    {  2, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 0, 0, 1, 2, 2 },   /* z =  8 */
    {  2, 2, 3, 3, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2 },   /* z =  9 */
    {  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },   /* z = 10 */
    {  1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2 },   /* z = 11 */
    {  1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1 },   /* z = 12  praia */
    {  0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0 },   /* z = 13 */
    {  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },   /* z = 14  mar */
    {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },   /* z = 15 */
};

/* Consulta a altura de um quadrado do mapa. Atenção à ordem dos
   índices: primeiro a linha (z), depois a coluna (x). É por isso que a
   tabela acima aparece no código do mesmo jeito que o mundo visto de
   cima. */
int alturaDoTerreno(int x, int z)
{
    return mapa[z][x];
}

/* ------------------------------------------------------------------ */
/* Uma coluna do terreno na posição (x, z) do chão.                   */
/* Lê a altura h no mapa e empilha blocos de y = 0 até o topo. Para   */
/* cada andar y, a sequência de if escolhe o tipo comparando y com h: */
/*                                                                    */
/*   y acima do chão (y > h)      AGUA   só acontece em colunas mais  */
/*                                       baixas que NIVEL_AGUA (mar)  */
/*   coluna baixa (h <= NIVEL)    AREIA  praia e fundo do mar         */
/*   topo de coluna alta          NEVE   y == h e h >= NIVEL_NEVE     */
/*   último andar (y == h)        GRAMA  o topo das demais colunas    */
/*   2 andares abaixo do topo     TERRA                               */
/*   o resto, mais embaixo        PEDRA                               */
/*                                                                    */
/* A ordem dos if importa: o primeiro que for verdadeiro decide. Por  */
/* isso a NEVE vem antes da GRAMA; se viesse depois, nunca seria      */
/* escolhida, porque o topo já teria virado grama.                    */
/*                                                                    */
/* Exemplo, coluna de altura 6: PEDRA de y = 0 a 3, TERRA em 4 e 5,   */
/* NEVE em 6. Coluna de altura 4: PEDRA em 0 e 1, TERRA em 2 e 3,     */
/* GRAMA em 4. Coluna do mar (altura 0): AREIA em 0, AGUA em 1.       */
/* ------------------------------------------------------------------ */

void colunaEm(int x, int z)
{
    int h = alturaDoTerreno(x, z);
    int y;

    /* sobe até o topo da coluna, ou até o nível da água se ela for mais
       baixa, para preencher o mar */
    for (y = 0; y <= h || y <= NIVEL_AGUA; y++) {
        if (y > h)                blocoEm(AGUA,  x, y, z);
        else if (h <= NIVEL_AGUA) blocoEm(AREIA, x, y, z);
        else if (y == h && h >= NIVEL_NEVE) blocoEm(NEVE, x, y, z);
        else if (y == h)          blocoEm(GRAMA, x, y, z);
        else if (y >= h - 2)      blocoEm(TERRA, x, y, z);
        else                      blocoEm(PEDRA, x, y, z);
    }
}

/* ------------------------------------------------------------------ */
/* Árvore no formato do Minecraft, plantada no quadrado (x, z).       */
/* Primeiro descobre a altura do chão ali, para a árvore nascer em    */
/* cima da grama. Depois empilha, a partir do chão:                   */
/*                                                                    */
/*   chao+7          F            1 folha no topo                     */
/*   chao+6       F  F  F         camada 3 x 3                        */
/*   chao+5    F  F  T  F  F      camada 5 x 5, tronco no meio        */
/*   chao+4    F  F  T  F  F      camada 5 x 5, tronco no meio        */
/*   chao+3          T                                                */
/*   chao+2          T                                                */
/*   chao+1          T            tronco: 5 blocos                    */
/*   chao      (grama do mapa)                                        */
/*                                                                    */
/* dx e dz são deslocamentos em relação ao tronco: dx = -2 é dois     */
/* blocos à esquerda, dx = 2 é dois à direita, e assim por diante.    */
/* ------------------------------------------------------------------ */

void arvoreEm(int x, int z)
{
    int chao = alturaDoTerreno(x, z);
    int y, dx, dz;

    /* tronco: mesma posição (x, z), subindo 5 andares */
    for (y = chao + 1; y <= chao + 5; y++)
        blocoEm(TRONCO, x, y, z);

    /* duas camadas largas de folhas, de 2 para cada lado do tronco */
    for (y = chao + 4; y <= chao + 5; y++)
        for (dx = -2; dx <= 2; dx++)
            for (dz = -2; dz <= 2; dz++)
                if (dx != 0 || dz != 0)     /* pula o centro: ali está o tronco */
                    blocoEm(FOLHA, x + dx, y, z + dz);

    /* camada menor, de 1 para cada lado; o tronco já acabou, então o
       centro também recebe folha */
    for (dx = -1; dx <= 1; dx++)
        for (dz = -1; dz <= 1; dz++)
            blocoEm(FOLHA, x + dx, chao + 6, z + dz);

    /* folha do topo, bem em cima do tronco */
    blocoEm(FOLHA, x, chao + 7, z);
}

/* ------------------------------------------------------------------ */
/* Nuvem: uma placa retangular de blocos brancos, sempre na altura    */
/* y = ALTURA_NUVEM, como as nuvens achatadas do Minecraft.           */
/*   (x, z)    canto da nuvem no mapa                                 */
/*   largura   quantos blocos ela ocupa em x                          */
/*   fundo     quantos blocos ela ocupa em z                          */
/* Exemplo: nuvemEm(1, 2, 5, 3) ocupa x de 1 a 5 e z de 2 a 4.        */
/* ------------------------------------------------------------------ */

void nuvemEm(int x, int z, int largura, int fundo)
{
    int dx, dz;

    for (dx = 0; dx < largura; dx++)
        for (dz = 0; dz < fundo; dz++)
            blocoEm(NUVEM, x + dx, ALTURA_NUVEM, z + dz);
}

/* ------------------------------------------------------------------ */
/* A cena inteira, montada do zero a cada quadro:                     */
/*   1. percorre os 16 x 16 quadrados do mapa e desenha a coluna de   */
/*      cada um (terreno, praia e mar)                                */
/*   2. planta a árvore                                               */
/*   3. coloca as nuvens                                              */
/* Os blocos não ficam guardados em lugar nenhum: a cada quadro tudo  */
/* é recalculado e redesenhado a partir do mapa.                      */
/* ------------------------------------------------------------------ */

void desenhaCena(void)
{
    int x, z;

    for (x = 0; x < LADO; x++)
        for (z = 0; z < LADO; z++)
            colunaEm(x, z);

    arvoreEm(13, 6);                        /* no gramado, à direita da montanha */

    nuvemEm(1, 2, 5, 3);                    /* sobre a montanha */
    nuvemEm(10, 0, 4, 2);                   /* sobre a árvore, mais ao fundo */
}

/* ------------------------------------------------------------------ */
/* Quadro: a GLUT chama esta função sempre que a janela precisa ser   */
/* redesenhada. A cada chamada:                                       */
/*   1. apaga a imagem anterior (cor e profundidade)                  */
/*   2. posiciona a câmera                                            */
/*   3. desenha a cena                                                */
/*   4. mostra o resultado na tela                                    */
/*   5. atualiza a contagem de FPS                                    */
/* ------------------------------------------------------------------ */

int inicioContagem = 0;   /* início da contagem de FPS (semana 1), em ms */
int quadros = 0;          /* quadros desenhados desde inicioContagem */

void desenha(void)
{
    /* apaga a cor (fica o azul do glClearColor) e o buffer de
       profundidade, que guarda a distância de cada pixel */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* câmera (semana 4): gluLookAt recebe de onde olha (posição da
       câmera), para onde olha (um ponto logo à frente, na direção do
       ângulo) e qual direção é "cima" (eixo y) */
    glLoadIdentity();
    float rad = camAngulo * PI / 180.0f;
    gluLookAt(camX, camY, camZ,
              camX + sinf(rad), camY, camZ - cosf(rad),
              0.0, 1.0, 0.0);

    desenhaCena();

    /* desenhamos num buffer escondido; aqui ele troca de lugar com o que
       está na tela, para não aparecer a imagem pela metade */
    glutSwapBuffers();

    /* conta os quadros e, a cada 1000 ms, mostra o total no título */
    quadros++;
    int agora = glutGet(GLUT_ELAPSED_TIME);
    if (agora - inicioContagem >= 1000) {
        char titulo[64];
        sprintf(titulo, "Minecraft | %d FPS", quadros);
        glutSetWindowTitle(titulo);
        quadros = 0;
        inicioContagem = agora;
    }
}

/* Chamada pela GLUT quando não há nenhum evento (tecla, janela) para
   tratar. Pede um novo quadro, o que mantém a animação rodando. */
void ocioso(void)
{
    glutPostRedisplay();
}

/* ------------------------------------------------------------------ */
/* Programa principal                                                 */
/* Cria a janela, registra as funções que a GLUT vai chamar em cada   */
/* situação (callbacks), liga os testes do OpenGL e entra no laço     */
/* principal, de onde não sai mais.                                   */
/* ------------------------------------------------------------------ */

int main(int argc, char **argv)
{
    /* janela com buffer duplo (GLUT_DOUBLE), cores RGB e buffer de
       profundidade (GLUT_DEPTH) */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Minecraft");

    /* a GLEW só pode ser iniciada depois que a janela existe */
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("Erro GLEW: %s\n", glewGetErrorString(err));
        return 1;
    }

    printf("OpenGL %s | %s\n", glGetString(GL_VERSION), glGetString(GL_RENDERER));

    /* callbacks: a GLUT chama cada função quando o evento acontece */
    glutReshapeFunc(redimensiona);          /* janela abriu ou mudou de tamanho */
    glutDisplayFunc(desenha);               /* hora de desenhar */
    glutIdleFunc(ocioso);                   /* nada acontecendo */
    glutKeyboardFunc(teclado);              /* tecla comum (letras, ESC) */
    glutSpecialFunc(especial);              /* tecla especial (setas) */

    glEnable(GL_DEPTH_TEST);                /* o mais próximo cobre o mais distante,
                                               não importa a ordem de desenho */
    glEnable(GL_CULL_FACE);                 /* não desenha faces de costas */
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);   /* cor de fundo: azul do céu */

    inicioContagem = glutGet(GLUT_ELAPSED_TIME);

    glutMainLoop();                         /* laço principal: só sai no ESC */
    return 0;
}
