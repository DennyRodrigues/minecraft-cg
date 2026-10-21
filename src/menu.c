/*
 * menu.c
 *
 * Menu de pausa, como o do Minecraft: ESC abre e fecha, o jogo para de
 * se mover, o cursor fica livre e a tela mostra as instruções de
 * movimentação por cima da cena escurecida. Com o menu aberto, Q sai do
 * jogo.
 */

#include "graficos.h"
#include "camera.h"             /* larguraJanela, alturaJanela */
#include "entrada.h"            /* capturaCursor, liberaCursor */
#include "menu.h"

int menuAberto = 0;

/* Abrir pausa o jogo (atualiza() deixa de mover o personagem) e libera
   o cursor, para o usuário poder usar o mouse fora do jogo. Fechar
   captura o cursor de novo. */
void abreMenu(void)
{
    menuAberto = 1;
    liberaCursor();
}

void fechaMenu(void)
{
    menuAberto = 0;
    capturaCursor();
}

void alternaMenu(void)
{
    if (menuAberto) fechaMenu();
    else            abreMenu();
}

/* ------------------------------------------------------------------ */
/* Texto                                                              */
/* As fontes da GLUT (glutBitmapCharacter) usam a codificação         */
/* Latin-1: um byte por letra, e as letras acentuadas do português    */
/* têm códigos de 192 a 255. Este arquivo, como o resto do código,    */
/* está em UTF-8, em que essas mesmas letras ocupam DOIS bytes (por   */
/* exemplo, "ç" é 0xC3 0xA7). Enviados direto para a GLUT, os dois    */
/* bytes virariam dois símbolos estranhos.                            */
/* proximoCaractere() lê um caractere UTF-8 e devolve o código        */
/* Latin-1 dele. Nos dois bytes 110xxxxx 10yyyyyy, os bits xxxxx e    */
/* yyyyyy juntos formam o código: (b1 & 0x1F) << 6 | (b2 & 0x3F).     */
/* Caracteres fora do Latin-1 (como as setas) viram '?', por isso o   */
/* menu escreve "seta para cima" em vez do símbolo.                   */
/* ------------------------------------------------------------------ */

#define FONTE       GLUT_BITMAP_HELVETICA_18
#define FONTE_TITULO GLUT_BITMAP_TIMES_ROMAN_24

static int proximoCaractere(const char **texto)
{
    unsigned char b1 = (unsigned char)**texto;
    (*texto)++;

    if (b1 < 0x80) return b1;                       /* ASCII: 1 byte */

    if ((b1 & 0xE0) == 0xC0 && **texto) {           /* 2 bytes */
        unsigned char b2 = (unsigned char)**texto;
        int codigo = ((b1 & 0x1F) << 6) | (b2 & 0x3F);
        (*texto)++;
        return codigo <= 0xFF ? codigo : '?';
    }

    while ((**texto & 0xC0) == 0x80) (*texto)++;    /* pula o resto */
    return '?';
}

/* largura do texto na tela, em pixels */
static int larguraTexto(void *fonte, const char *texto)
{
    int largura = 0;
    while (*texto)
        largura += glutBitmapWidth(fonte, proximoCaractere(&texto));
    return largura;
}

/* escreve com o início da linha de base em (x, y), em pixels */
static void escreve(void *fonte, float x, float y, const char *texto)
{
    glRasterPos2f(x, y);
    while (*texto)
        glutBitmapCharacter(fonte, proximoCaractere(&texto));
}

/* um retângulo preenchido, em pixels */
static void retangulo(float x0, float y0, float x1, float y1)
{
    glBegin(GL_QUADS);
    glVertex2f(x0, y0); glVertex2f(x1, y0);
    glVertex2f(x1, y1); glVertex2f(x0, y1);
    glEnd();
}

/* ------------------------------------------------------------------ */
/* Menu: desenhado por cima da cena numa segunda passada, em 2D.      */
/* Projeção ortográfica em pixels (gluOrtho2D), modelo-visualização   */
/* identidade e teste de profundidade desligado, para o painel não    */
/* ficar atrás dos blocos. A cena é escurecida com transparência      */
/* (alfa 0.5). As matrizes 3D são guardadas e devolvidas no fim. A    */
/* cor do texto vale a partir do glRasterPos, por isso glColor vem    */
/* antes.                                                             */
/* ------------------------------------------------------------------ */

static const char *instrucoes[][2] = {
    { "W / S",                    "andar para frente / para trás" },
    { "A / D",                    "andar para a esquerda / para a direita" },
    { "R (segurando)",            "correr" },
    { "Espaço ou seta para cima", "subir" },
    { "Shift ou seta para baixo", "descer" },
    { "Setas esquerda / direita", "girar o olhar" },
    { "Mouse",                    "olhar ao redor" },
};

#define NUM_INSTRUCOES (int)(sizeof(instrucoes) / sizeof(instrucoes[0]))
#define ALTURA_LINHA 30
#define MARGEM       40
#define ESPACO_COLUNAS 40

void desenhaMenu(void)
{
    const char *titulo = "Movimentação";
    const char *rodape = "ESC volta ao jogo     Q sai do jogo";
    int larguraChaves = 0, larguraAcoes = 0, i;

    /* 1. configuração 2D, guardando a 3D */
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, larguraJanela, 0, alturaJanela);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* 2. escurece a cena */
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    retangulo(0, 0, larguraJanela, alturaJanela);

    /* 3. tamanho e posição do painel, a partir do texto */
    for (i = 0; i < NUM_INSTRUCOES; i++) {
        int lc = larguraTexto(FONTE, instrucoes[i][0]);
        int la = larguraTexto(FONTE, instrucoes[i][1]);
        if (lc > larguraChaves) larguraChaves = lc;
        if (la > larguraAcoes)  larguraAcoes  = la;
    }

    float larguraPainel = larguraChaves + ESPACO_COLUNAS + larguraAcoes + 2 * MARGEM;
    float alturaPainel  = (NUM_INSTRUCOES + 4) * ALTURA_LINHA + 2 * MARGEM;
    float esquerda = (larguraJanela - larguraPainel) / 2.0f;
    float topo     = (alturaJanela + alturaPainel) / 2.0f;

    glColor4f(0.08f, 0.08f, 0.12f, 0.85f);
    retangulo(esquerda, topo - alturaPainel, esquerda + larguraPainel, topo);

    glDisable(GL_BLEND);

    /* 4. texto: a cor vem antes do glRasterPos (feito em escreve) */
    float y = topo - MARGEM - 20;
    glColor3f(1.0f, 1.0f, 1.0f);
    escreve(FONTE_TITULO,
            (larguraJanela - larguraTexto(FONTE_TITULO, titulo)) / 2.0f, y, titulo);

    y -= 2 * ALTURA_LINHA;
    for (i = 0; i < NUM_INSTRUCOES; i++) {
        glColor3f(1.0f, 0.85f, 0.35f);                  /* tecla: amarelo */
        escreve(FONTE, esquerda + MARGEM, y, instrucoes[i][0]);
        glColor3f(0.92f, 0.92f, 0.92f);                 /* ação: branco */
        escreve(FONTE, esquerda + MARGEM + larguraChaves + ESPACO_COLUNAS, y,
                instrucoes[i][1]);
        y -= ALTURA_LINHA;
    }

    y -= ALTURA_LINHA;
    glColor3f(0.65f, 0.65f, 0.70f);
    escreve(FONTE, (larguraJanela - larguraTexto(FONTE, rodape)) / 2.0f, y, rodape);

    /* 5. devolve a configuração 3D */
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();                                      /* modelo-visualização */
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
