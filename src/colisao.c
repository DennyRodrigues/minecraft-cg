/*
 * colisao.c
 *
 * A posição do personagem e a colisão com o mundo: que blocos são
 * sólidos, se a caixa do personagem cabe numa posição e como aplicar
 * um deslocamento sem atravessar paredes.
 */

#include <math.h>               /* floorf, sqrtf, ceilf */

#include "config.h"
#include "mundo.h"
#include "colisao.h"

/* ------------------------------------------------------------------ */
/* Posição do personagem                                              */
/* jogadorY é a altura dos PÉS; os olhos ficam ALTURA_OLHOS acima.    */
/* Este módulo é o dono da posição: só moveJogador() a altera, sempre */
/* depois de testar a colisão.                                        */
/* ------------------------------------------------------------------ */

float jogadorX = 8.5f;  /* começa no gramado, no meio em x,        */
float jogadorY = 3.0f;  /* com os pés sobre a grama (coluna de     */
float jogadorZ = 10.5f; /* altura 2, chão em y = 3); com yaw 0,    */
                        /* olha para a montanha                    */

/* ------------------------------------------------------------------ */
/* Uma célula impede a passagem? Fora do mundo conta como sólido      */
/* (paredes nas bordas, piso e teto); dentro, VAZIO e AGUA deixam     */
/* passar.                                                            */
/* ------------------------------------------------------------------ */

int blocoSolido(int x, int y, int z)
{
    int tipo;

    if (!dentroDoMundo(x, y, z)) return 1;  /* fora do mundo: parede */

    tipo = mundo[x][y][z];
    return tipo != VAZIO && tipo != AGUA;
}

/* ------------------------------------------------------------------ */
/* Detecção de colisão: a caixa do personagem (AABB, alinhada aos     */
/* eixos, sem girar) com os pés em (x, y, z) toca algum bloco sólido? */
/* Em cada eixo a caixa cobre as células de floor(min) a floor(max),  */
/* e basta consultar essas células na matriz (no máximo 12).          */
/*                                                                    */
/* floorf, e não (int), porque o cast erra para coordenadas           */
/* negativas. FOLGA para que encostar numa face não conte como        */
/* sobrepor.                                                          */
/* ------------------------------------------------------------------ */

#define FOLGA 0.001f    /* encostar não é sobrepor */

int posicaoLivre(float x, float y, float z)
{
    float meia = LARGURA_JOGADOR / 2.0f;
    int x0 = (int)floorf(x - meia), x1 = (int)floorf(x + meia - FOLGA);
    int y0 = (int)floorf(y),        y1 = (int)floorf(y + ALTURA_JOGADOR - FOLGA);
    int z0 = (int)floorf(z - meia), z1 = (int)floorf(z + meia - FOLGA);
    int cx, cy, cz;

    for (cx = x0; cx <= x1; cx++)
        for (cy = y0; cy <= y1; cy++)
            for (cz = z0; cz <= z1; cz++)
                if (blocoSolido(cx, cy, cz))
                    return 0;               /* a caixa toca um bloco sólido */
    return 1;
}

/* ------------------------------------------------------------------ */
/* Resposta à colisão: o movimento é testado antes de ser aplicado,   */
/* um eixo por vez (x, z, y). Se um eixo bloqueia, os outros          */
/* continuam, e o personagem desliza rente à parede em vez de travar  */
/* na diagonal.                                                       */
/*                                                                    */
/* Deslocamentos maiores que PASSO_MAXIMO são divididos em pedaços,   */
/* para nenhum teste saltar por cima de uma parede (tunelamento).     */
/* ------------------------------------------------------------------ */

/* Aplica um pedaço do deslocamento, já pequeno o bastante para não
   atravessar paredes */
static void movePedaco(float dx, float dy, float dz)
{
    if (posicaoLivre(jogadorX + dx, jogadorY, jogadorZ)) jogadorX += dx;
    if (posicaoLivre(jogadorX, jogadorY, jogadorZ + dz)) jogadorZ += dz;
    if (posicaoLivre(jogadorX, jogadorY + dy, jogadorZ)) jogadorY += dy;
}

void moveJogador(float dx, float dy, float dz)
{
    float comprimento = sqrtf(dx * dx + dy * dy + dz * dz);
    int pedacos = (int)ceilf(comprimento / PASSO_MAXIMO);
    int i;

    if (pedacos < 1) pedacos = 1;           /* parado: um pedaço nulo */

    for (i = 0; i < pedacos; i++)
        movePedaco(dx / pedacos, dy / pedacos, dz / pedacos);
}
