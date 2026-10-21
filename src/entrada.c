/*
 * entrada.c
 *
 * Teclado (por evento e por estado), mouse (giro do olhar e captura do
 * cursor), roda e teclas + e - (zoom), e a atualização do movimento a
 * cada quadro, escalada pelo tempo de quadro.
 */

#include <stdlib.h>             /* exit */
#include <math.h>               /* sinf, cosf */
#include <ctype.h>              /* tolower */

#include "graficos.h"
#include "config.h"
#include "colisao.h"
#include "camera.h"
#include "entrada.h"
#include "menu.h"

/* ------------------------------------------------------------------ */
/* Teclado. A GLUT só avisa eventos (pressionar e soltar); o estado é */
/* construído aqui: teclas[] fica em 1 do pressionar ao soltar e é    */
/* lido a cada quadro em atualiza(). Por isso a repetição automática  */
/* do sistema está desligada (glutIgnoreKeyRepeat em main).           */
/*                                                                    */
/* As ações discretas (menu, cursor, projeção, câmera, zoom)          */
/* continuam por evento, uma vez por toque. tolower nos dois lados    */
/* evita tecla presa quando o Shift muda entre pressionar e soltar.   */
/* As setas usam o mesmo esquema em especiais[].                      */
/* ------------------------------------------------------------------ */

int teclas[256];        /* teclas de caractere: 1 pressionada, 0 solta */
int especiais[256];     /* teclas especiais (setas, F5): idem */
void teclado(unsigned char tecla, int x, int y)
{
    (void)x; (void)y;                       /* posição do mouse: não usada */

    teclas[tolower(tecla)] = 1;             /* estado: para as ações contínuas */

    if (menuAberto) {                       /* com o menu, só duas teclas valem */
        if (tecla == 27) fechaMenu();       /* 27 é o código da tecla ESC */
        else if (tolower(tecla) == 'q') exit(0);
        return;
    }

    switch (tolower(tecla)) {               /* evento: para as ações discretas */
        case 27:  abreMenu(); break;        /* ESC: menu de pausa */
        case 9:   alternaCaptura(); break;  /* 9 é o código da tecla Tab */
        case 'p':
            projecaoOrtogonal = !projecaoOrtogonal;
            aplicaProjecao();
            break;
        case 'v':
            modoCamera = (modoCamera == PRIMEIRA_PESSOA) ? TERCEIRA_PESSOA
                                                         : PRIMEIRA_PESSOA;
            break;
        case '+': case '=': mudaZoom(+1); break;    /* '=' é o + sem Shift */
        case '-': case '_': mudaZoom(-1); break;
    }
}

void tecladoSolto(unsigned char tecla, int x, int y)
{
    (void)x; (void)y;
    teclas[tolower(tecla)] = 0;
}

void especial(int tecla, int x, int y)
{
    (void)x; (void)y;

    if (tecla >= 0 && tecla < 256) especiais[tecla] = 1;

    if (tecla == GLUT_KEY_F5 && !menuAberto)               /* evento: alterna a câmera */
        modoCamera = (modoCamera == PRIMEIRA_PESSOA) ? TERCEIRA_PESSOA
                                                     : PRIMEIRA_PESSOA;
}

void especialSolto(int tecla, int x, int y)
{
    (void)x; (void)y;
    if (tecla >= 0 && tecla < 256) especiais[tecla] = 0;
}

/* ------------------------------------------------------------------ */
/* Mouse: o olhar gira pelo deslocamento em relação ao centro da      */
/* janela. Em coordenadas de tela y cresce para baixo, por isso -dy   */
/* no pitch. Com o cursor capturado, ele volta ao centro a cada       */
/* movimento (glutWarpPointer); o evento desse reposicionamento chega */
/* com deslocamento zero e é ignorado. Registrada com e sem botão     */
/* pressionado.                                                       */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* O pitch fica entre -LIMITE_PITCH e LIMITE_PITCH (89 graus). Em 90, */
/* a direção do olhar fica paralela ao "cima" do gluLookAt, o produto */
/* vetorial que monta os eixos da câmera se anula e a imagem inverte. */
/* ------------------------------------------------------------------ */

int cursorCapturado = 0;    /* 1 enquanto o mouse controla o olhar */
int recentrar = 0;          /* 1: o próximo evento só leva o cursor ao centro */

void movimentoMouse(int x, int y)
{
    int cx = larguraJanela / 2, cy = alturaJanela / 2;
    int dx = x - cx, dy = y - cy;

    if (!cursorCapturado) return;

    if (recentrar) {                        /* cursor fora do centro: só */
        recentrar = 0;                      /* reposiciona, sem girar    */
        glutWarpPointer(cx, cy);
        return;
    }

    if (dx == 0 && dy == 0) return;         /* eco do glutWarpPointer */

    yaw   += dx * SENSIBILIDADE;
    pitch -= dy * SENSIBILIDADE;            /* y de tela cresce para baixo */

    if (pitch >  LIMITE_PITCH) pitch =  LIMITE_PITCH;
    if (pitch < -LIMITE_PITCH) pitch = -LIMITE_PITCH;

    glutWarpPointer(cx, cy);                /* devolve o cursor ao centro */
}

/* Captura e liberação do cursor. O jogo começa com o cursor capturado;
   Tab alterna, e um clique na janela captura de novo. Sem uma forma de
   liberar, o usuário não conseguiria sair da janela.
   capturaCursor() não move o cursor na hora: só o oculta e pede que o
   próximo movimento o leve ao centro. Assim funciona também no início,
   quando a janela talvez ainda nem esteja na tela, e o cursor já está
   oculto quando é reposicionado (em algumas sessões Wayland o
   reposicionamento só funciona com o cursor oculto). */
void capturaCursor(void)
{
    cursorCapturado = 1;
    recentrar = 1;
    glutSetCursor(GLUT_CURSOR_NONE);
}

void liberaCursor(void)
{
    cursorCapturado = 0;
    glutSetCursor(GLUT_CURSOR_INHERIT);
}

void alternaCaptura(void)
{
    if (cursorCapturado) liberaCursor();
    else                 capturaCursor();
}

/* Clique com o cursor liberado: captura de novo */
void cliqueMouse(int botao, int estado, int x, int y)
{
    (void)botao; (void)x; (void)y;
    if (estado == GLUT_DOWN && !cursorCapturado && !menuAberto)
        capturaCursor();
}

/* O cursor entrou na janela (por exemplo, depois de trocar de janela
   com Alt+Tab): ele chega pela borda, então o próximo movimento só o
   leva ao centro, sem girar o olhar */
void entradaJanela(int estado)
{
    if (estado == GLUT_ENTERED) recentrar = 1;
}

/* ------------------------------------------------------------------ */
/* Zoom: a roda e as teclas + e - mudam o campo de visão (trocam a    */
/* lente), sem mover a câmera. Passos de 5 graus, entre FOV_MIN e     */
/* FOV_MAX. glutMouseWheelFunc é extensão da freeglut, por isso       */
/* GL/freeglut.h.                                                     */
/* ------------------------------------------------------------------ */

/* +1 aproxima, -1 afasta; usada pela roda e pelas teclas + e - */
void mudaZoom(int direcao)
{
    campoDeVisao -= direcao * 5.0f;         /* fov menor = mais perto */
    if (campoDeVisao < FOV_MIN) campoDeVisao = FOV_MIN;
    if (campoDeVisao > FOV_MAX) campoDeVisao = FOV_MAX;

    aplicaProjecao();
}

void rodaMouse(int roda, int direcao, int x, int y)
{
    (void)roda; (void)x; (void)y;
    if (!menuAberto)
        mudaZoom(direcao);                  /* roda para cima aproxima */
}

/* ------------------------------------------------------------------ */
/* Atualização, a cada quadro (glutIdleFunc). dt é o tempo desde o    */
/* quadro anterior, limitado a DT_MAXIMO; posição += velocidade x dt  */
/* deixa a velocidade igual em qualquer taxa de quadros.              */
/*                                                                    */
/* A frente e a direita saem só do yaw, então o avanço fica no plano  */
/* do chão. A direção das teclas é normalizada (a diagonal não fica   */
/* mais rápida), R multiplica o passo e moveJogador() aplica a        */
/* colisão. No fim, a animação de caminhada avança com a distância    */
/* realmente andada.                                                  */
/* ------------------------------------------------------------------ */

int ultimoTempo = 0;    /* tempo da atualização anterior, em ms */

float faseCaminhada = 0.0f;      /* radianos; avança com a distância andada */
float amplitudeCaminhada = 0.0f; /* 0 parado, 1 andando; suaviza o balanço */

void atualiza(void)
{
    int agora = glutGet(GLUT_ELAPSED_TIME);
    float dt = (agora - ultimoTempo) / 1000.0f;
    ultimoTempo = agora;
    if (dt > DT_MAXIMO) dt = DT_MAXIMO;

    if (menuAberto) {                       /* pausa: o relógio continua sendo */
        glutPostRedisplay();                /* lido, para o primeiro dt depois */
        return;                             /* de fechar não ser enorme */
    }

    float passo = VELOCIDADE * dt;
    float giro  = VELOCIDADE_GIRO * dt;
    if (teclas['r']) passo *= FATOR_CORRIDA;    /* correr */

    if (especiais[GLUT_KEY_LEFT])  yaw -= giro;
    if (especiais[GLUT_KEY_RIGHT]) yaw += giro;

    float rad = yaw * PI / 180.0f;
    float frenteX  =  sinf(rad), frenteZ  = -cosf(rad);
    float direitaX =  cosf(rad), direitaZ =  sinf(rad);

    /* direção desejada no chão: cada tecla vale 1 (pressionada) ou 0 */
    float frente = (float)(teclas['w'] - teclas['s']);
    float lado   = (float)(teclas['d'] - teclas['a']);
    float mx = frenteX * frente + direitaX * lado;
    float mz = frenteZ * frente + direitaZ * lado;

    float comprimento = sqrtf(mx * mx + mz * mz);
    if (comprimento > 0.0f) {               /* normaliza: na diagonal o */
        mx /= comprimento;                  /* comprimento seria 1.41   */
        mz /= comprimento;
    }

    /* Shift: a GLUT original não avisa quando só o Shift é pressionado,
       mas a freeglut (a partir da 3.0) o entrega como tecla especial,
       GLUT_KEY_SHIFT_L ou _R, e ele entra em especiais[] como as setas */
    int sobe  = especiais[GLUT_KEY_UP]   || teclas[' '];
    int desce = especiais[GLUT_KEY_DOWN] || especiais[GLUT_KEY_SHIFT_L]
                                         || especiais[GLUT_KEY_SHIFT_R];
    float subir = (float)(sobe - desce);

    float antesX = jogadorX, antesZ = jogadorZ;
    moveJogador(mx * passo, subir * passo, mz * passo);

    /* animação: quanto o personagem realmente andou no chão (a colisão
       pode ter recusado parte do movimento) */
    float andou = sqrtf((jogadorX - antesX) * (jogadorX - antesX) +
                        (jogadorZ - antesZ) * (jogadorZ - antesZ));
    faseCaminhada += andou * FASE_POR_BLOCO;
    if (andou > 0.0f) amplitudeCaminhada += VELOCIDADE_AMPLITUDE * dt;
    else              amplitudeCaminhada -= VELOCIDADE_AMPLITUDE * dt;
    if (amplitudeCaminhada > 1.0f) amplitudeCaminhada = 1.0f;
    if (amplitudeCaminhada < 0.0f) amplitudeCaminhada = 0.0f;

    glutPostRedisplay();
}
