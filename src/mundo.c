/*
 * mundo.c
 *
 * O cenário: cores e desenho dos blocos, o mapa de alturas, a matriz
 * 3D do mundo, a construção do terreno, da árvore e das nuvens, e o
 * desenho da cena inteira a partir da matriz.
 */

#include "graficos.h"
#include "config.h"
#include "mundo.h"

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
        case TERRA:   r = 0.53f; g = 0.38f; b = 0.26f; break;
        case PEDRA:   r = 0.49f; g = 0.49f; b = 0.49f; break;
        case TRONCO:  r = 0.40f; g = 0.31f; b = 0.19f; break;
        case FOLHA:   r = 0.23f; g = 0.48f; b = 0.13f; break;
        case AREIA:   r = 0.86f; g = 0.81f; b = 0.58f; break;
        case AGUA:    r = 0.20f; g = 0.40f; b = 0.85f; break;
        case NUVEM:   r = 1.00f; g = 1.00f; b = 1.00f; break;
        case NEVE:    r = 0.95f; g = 0.97f; b = 1.00f; break;   /* branco levemente azulado */
        case PELE:    r = 0.80f; g = 0.60f; b = 0.47f; break;   /* personagem: */
        case CABELO:  r = 0.30f; g = 0.20f; b = 0.12f; break;
        case CAMISA:  r = 0.00f; g = 0.62f; b = 0.66f; break;
        case CALCA:   r = 0.22f; g = 0.22f; b = 0.55f; break;
        case SAPATO:  r = 0.35f; g = 0.35f; b = 0.35f; break;
        case OLHO_BRANCO: r = 0.95f; g = 0.95f; b = 0.95f; break;
        case OLHO:    r = 0.30f; g = 0.25f; b = 0.65f; break;
        default:     r = 1.00f; g = 0.00f; b = 1.00f; break;   /* rosa = tipo desconhecido */
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
/* blocoEm(), logo abaixo, ou desenhaJogador().                       */
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
/* (x, y, z) do mundo. Todos os blocos da cena acabam aqui.           */
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
/*  x: 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15                 */
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

/* ==================================================================
 * MUNDO: a matriz 3D que guarda o tipo de cada bloco
 *
 * O problema
 *   Até a semana 4 o mundo não ficava guardado em lugar nenhum: a cada
 *   quadro, desenhaCena() recalculava as colunas, a árvore e as nuvens e
 *   desenhava na hora. Isso basta para DESENHAR, mas não para responder
 *   "existe um bloco na posição (5, 3, 7)?", que é a pergunta de que a
 *   colisão precisa. O mapa de alturas também não basta: ele não sabe
 *   da árvore, das nuvens, nem de um bloco colocado no meio do ar.
 *
 * A teoria
 *   O espaço é discretizado numa grade regular: o mundo contínuo é
 *   dividido em células cúbicas de lado 1 (em inglês, voxels, "pixels de
 *   volume"), e cada célula guarda um número, o tipo do bloco que a
 *   ocupa, ou VAZIO. O bloco de índice (x, y, z) ocupa o cubo que vai de
 *   (x, y, z) até (x+1, y+1, z+1).
 *
 *   Daí sai a relação que a colisão inteira usa: a célula que contém um
 *   ponto qualquer (px, py, pz) é
 *
 *       (floor(px), floor(py), floor(pz))
 *
 *   ou seja, a coordenada do mundo arredondada para baixo JÁ É o índice
 *   da matriz. Não é preciso procurar o bloco: basta calcular onde ele
 *   estaria e olhar.
 *
 *   A matriz também separa DADO (o que existe no mundo) de DESENHO (como
 *   mostrar). O mesmo dado serve para desenhar, para a física e, no
 *   futuro, para quebrar e colocar blocos.
 *
 * Como o código produz o efeito
 *   mundo[x][y][z] tem LADO x ALTURA_MUNDO x LADO = 16 x 16 x 16 = 4096
 *   posições de um byte. ATENÇÃO à ordem: o mapa é [z][x] (para parecer
 *   o mundo visto de cima), mas mundo é [x][y][z], na ordem dos eixos.
 *   Para não confundir, todo acesso passa por colocaBloco() e
 *   blocoNoMundo(), que também testam os limites: em C, escrever fora de
 *   um vetor não dá erro, simplesmente estraga outra variável.
 *
 * Pergunta provável
 *   "Por que não guardar só o mapa de alturas e calcular o resto?"
 *   Porque a altura de uma coluna descreve apenas terreno maciço do chão
 *   para cima. A copa da árvore, as nuvens (blocos no ar, com vazio
 *   embaixo) e qualquer bloco colocado depois não cabem nessa descrição.
 * ================================================================== */

unsigned char mundo[LADO][ALTURA_MUNDO][LADO];

/* 1 se (x, y, z) é uma posição válida da matriz                      */
int dentroDoMundo(int x, int y, int z)
{
    return x >= 0 && x < LADO &&
           y >= 0 && y < ALTURA_MUNDO &&
           z >= 0 && z < LADO;
}

/* Grava um bloco na matriz; posições fora do mundo são ignoradas     */
void colocaBloco(int tipo, int x, int y, int z)
{
    if (dentroDoMundo(x, y, z))
        mundo[x][y][z] = (unsigned char)tipo;
}

/* Lê um bloco da matriz; fora do mundo não há bloco                  */
int blocoNoMundo(int x, int y, int z)
{
    if (!dentroDoMundo(x, y, z)) return VAZIO;
    return mundo[x][y][z];
}

/* ------------------------------------------------------------------ */
/* Uma coluna do terreno na posição (x, z) do chão.                   */
/* Lê a altura h no mapa e coloca blocos de y = 0 até o topo. Para    */
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
        if (y > h)                colocaBloco(AGUA,  x, y, z);
        else if (h <= NIVEL_AGUA) colocaBloco(AREIA, x, y, z);
        else if (y == h && h >= NIVEL_NEVE) colocaBloco(NEVE, x, y, z);
        else if (y == h)          colocaBloco(GRAMA, x, y, z);
        else if (y >= h - 2)      colocaBloco(TERRA, x, y, z);
        else                      colocaBloco(PEDRA, x, y, z);
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
        colocaBloco(TRONCO, x, y, z);

    /* duas camadas largas de folhas, de 2 para cada lado do tronco */
    for (y = chao + 4; y <= chao + 5; y++)
        for (dx = -2; dx <= 2; dx++)
            for (dz = -2; dz <= 2; dz++)
                if (dx != 0 || dz != 0)     /* pula o centro: ali está o tronco */
                    colocaBloco(FOLHA, x + dx, y, z + dz);

    /* camada menor, de 1 para cada lado; o tronco já acabou, então o
       centro também recebe folha */
    for (dx = -1; dx <= 1; dx++)
        for (dz = -1; dz <= 1; dz++)
            colocaBloco(FOLHA, x + dx, chao + 6, z + dz);

    /* folha do topo, bem em cima do tronco */
    colocaBloco(FOLHA, x, chao + 7, z);
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
            colocaBloco(NUVEM, x + dx, ALTURA_NUVEM, z + dz);
}

/* ==================================================================
 * CONSTRUIRMUNDO: preenche a matriz uma única vez, no início
 *
 * O problema
 *   O mundo não muda sozinho de um quadro para o outro. Recalcular as
 *   colunas, a árvore e as nuvens 60 vezes por segundo, como antes, era
 *   trabalho repetido sem resultado novo.
 *
 * A teoria
 *   É a separação clássica entre CONSTRUIR o estado (uma vez, ou quando
 *   algo muda) e CONSULTAR o estado (a cada quadro, para desenhar e para
 *   colidir). A função que constrói pode ser lenta e clara; as que
 *   consultam precisam ser rápidas.
 *
 * Como o código produz o efeito
 *   Zera a matriz (tudo VAZIO) e chama as mesmas funções de sempre:
 *   colunaEm() para as 16 x 16 colunas, arvoreEm() e nuvemEm(). Elas
 *   continuam com a mesma lógica; só trocaram blocoEm() (desenhar) por
 *   colocaBloco() (gravar). A ordem importa: a árvore vem depois do
 *   terreno e, se ocupasse a mesma célula, escreveria por cima.
 *
 * Pergunta provável
 *   "E se eu quiser quebrar um bloco durante o jogo?"
 *   Basta colocaBloco(VAZIO, x, y, z). O desenho e a colisão leem a
 *   matriz, então os dois passam a ver o buraco no quadro seguinte.
 * ================================================================== */

void construirMundo(void)
{
    int x, y, z;

    for (x = 0; x < LADO; x++)
        for (y = 0; y < ALTURA_MUNDO; y++)
            for (z = 0; z < LADO; z++)
                mundo[x][y][z] = VAZIO;

    for (x = 0; x < LADO; x++)
        for (z = 0; z < LADO; z++)
            colunaEm(x, z);

    arvoreEm(13, 6);                        /* no gramado, à direita da montanha */

    nuvemEm(1, 2, 5, 3);                    /* sobre a montanha */
    nuvemEm(10, 0, 4, 2);                   /* sobre a árvore, mais ao fundo */
}

/* ------------------------------------------------------------------ */
/* A cena inteira: percorre a matriz do mundo e desenha cada posição  */
/* que tem bloco. Os blocos já foram colocados por construirMundo();  */
/* aqui só se lê a matriz. A cena sai igual à da semana 4, porque a   */
/* matriz foi preenchida pelas mesmas funções que antes desenhavam.   */
/* ------------------------------------------------------------------ */

void desenhaCena(void)
{
    int x, y, z;

    for (x = 0; x < LADO; x++)
        for (y = 0; y < ALTURA_MUNDO; y++)
            for (z = 0; z < LADO; z++)
                if (mundo[x][y][z] != VAZIO)
                    blocoEm(mundo[x][y][z], x, y, z);
}
