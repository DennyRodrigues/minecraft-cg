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

/* ==================================================================
 * BLOCOSOLIDO: diz se uma célula da grade impede a passagem
 *
 * O problema
 *   "Existe um bloco aqui?" e "posso passar por aqui?" são perguntas
 *   diferentes. A água existe (é desenhada), mas não deve bloquear. E
 *   fora da matriz não existe bloco nenhum, mas o personagem não deve
 *   poder sair do mundo nem cair por baixo dele.
 *
 * A teoria
 *   A colisão só precisa de uma propriedade de cada célula: sólida ou
 *   não. Concentrar essa decisão numa função deixa o resto da colisão
 *   independente dos tipos de bloco. Tratar fora do mundo como sólido é
 *   uma regra só que substitui quatro paredes invisíveis nas bordas, um
 *   piso embaixo de y = 0 e um teto acima de y = ALTURA_MUNDO.
 *
 * Como o código produz o efeito
 *   Primeiro o teste de limites (fora: sólido). Dentro, VAZIO e AGUA não
 *   são sólidos; qualquer outro tipo é.
 *
 * Pergunta provável
 *   "Como faria uma porta, que às vezes bloqueia e às vezes não?"
 *   Um tipo novo e uma condição aqui; nenhuma outra função muda.
 * ================================================================== */

int blocoSolido(int x, int y, int z)
{
    int tipo;

    if (!dentroDoMundo(x, y, z)) return 1;  /* fora do mundo: parede */

    tipo = mundo[x][y][z];
    return tipo != VAZIO && tipo != AGUA;
}

/* ==================================================================
 * POSICAOLIVRE: DETECÇÃO de colisão entre o personagem e o mundo
 *
 * O problema
 *   Antes de mover o personagem é preciso saber se a posição pretendida
 *   está livre, isto é, se o corpo dele naquela posição não ocupa o
 *   mesmo espaço que algum bloco sólido.
 *
 * A teoria
 *   Colisão tem duas etapas: DETECÇÃO (as formas se sobrepõem?) e
 *   RESPOSTA (o que fazer se sobrepõem). Esta função é a detecção; a
 *   resposta está em moveJogador().
 *
 *   O personagem é uma caixa alinhada aos eixos (em inglês, AABB, axis-
 *   aligned bounding box): largura e altura fixas e SEM rotação. Os
 *   blocos também são AABB, de lado 1. Duas AABB se sobrepõem se, e
 *   somente se, os intervalos delas se sobrepõem nos TRÊS eixos ao
 *   mesmo tempo. Num eixo, dois intervalos [aMin, aMax) e [bMin, bMax)
 *   se sobrepõem quando
 *
 *       aMin < bMax  e  bMin < aMax
 *
 *       eixo x:     [====== jogador ======)
 *                               [============ bloco ============)
 *                  aMin        bMin      aMax                 bMax
 *
 *   Basta um eixo sem sobreposição para as caixas estarem separadas.
 *   Por isso a caixa não gira com o yaw: girar estragaria essa conta,
 *   que é feita só com comparações.
 *
 *   Comparar o personagem com cada um dos milhares de blocos seria caro.
 *   Mas, numa grade, os únicos blocos que podem tocar a caixa são os
 *   das células que ela cobre: em cada eixo, de floor(min) até
 *   floor(max). Isso troca uma busca em todos os blocos por poucas
 *   consultas diretas à matriz (ver o comentário de MUNDO). Aqui a
 *   colisão deixa de ser geometria e vira uma consulta à matriz.
 *
 * Como o código produz o efeito
 *   min e max da caixa: x - meia até x + meia, y (pés) até y + altura,
 *   z - meia até z + meia. Cada limite é convertido em célula com
 *   floorf, e o laço triplo pergunta blocoSolido() a cada uma. Achou
 *   uma sólida: a posição não está livre.
 *
 *   Dois detalhes:
 *     floorf, e não (int): (int)-0.5f dá 0, mas a célula de -0.5 é -1
 *       (fora do mundo, sólida). Com (int), a borda esquerda vazaria.
 *     FOLGA no limite máximo: um personagem encostado na face de um
 *       bloco tem x + meia == 5.0 exatamente. floor(5.0) = 5 diria que
 *       ele ocupa a célula 5, e ele ficaria preso. Os intervalos são
 *       semiabertos: [4, 5) e [5, 6) se tocam, mas não se sobrepõem.
 *
 *   Com VERSAO_COLISAO 1, só a célula do ponto central (os pés) é
 *   testada: o personagem só para quando o centro chega na parede, ou
 *   seja, com meio corpo dentro dela. Em primeira pessoa isso aparece
 *   como a parede sumindo: o olho chega mais perto dela que o plano
 *   próximo da projeção (0.1) e a face é cortada.
 *
 * Pergunta provável
 *   "Quantas células são testadas no máximo?"
 *   Com largura 0.6, a caixa cobre no máximo 2 células em x e 2 em z;
 *   com altura 1.8, no máximo 3 em y. São 2 x 3 x 2 = 12 consultas, não
 *   importa o tamanho do mundo.
 * ================================================================== */

#define FOLGA 0.001f    /* encostar não é sobrepor */

int posicaoLivre(float x, float y, float z)
{
#if VERSAO_COLISAO == 1
    /* só o ponto central dos pés */
    return !blocoSolido((int)floorf(x), (int)floorf(y), (int)floorf(z));
#else
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
#endif
}

/* ==================================================================
 * MOVEJOGADOR: RESPOSTA à colisão, aplicando o movimento eixo a eixo
 *
 * O problema
 *   Detectar não basta: é preciso decidir o que fazer. E a decisão mais
 *   ingênua, "se a posição final colide, não anda", faz o personagem
 *   travar sempre que encosta numa parede andando na diagonal.
 *
 * A teoria
 *   A resposta usada é a mais simples: impedir o movimento. O teste é
 *   feito ANTES de mover: calcula-se a posição pretendida, pergunta-se
 *   se ela está livre e só então ela é aplicada. Testar depois exigiria
 *   desfazer o movimento ou empurrar o personagem para fora do bloco.
 *
 *   Um deslocamento diagonal é a soma de um deslocamento em x com um em
 *   z. Se só um dos dois leva para dentro da parede, o outro continua
 *   válido. Testando cada eixo separado, o válido é aplicado e o
 *   bloqueado não, e o personagem desliza rente à parede:
 *
 *        ################  parede
 *        ################
 *              ^   /         a tentativa diagonal entraria na parede
 *           dz |  /          dz é recusado (entraria na parede)
 *              | /           dx é aceito
 *              o ----->      resultado: anda só em x, rente à parede
 *                 dx
 *
 *   Tunelamento: o teste só olha a posição final, não o caminho. Se um
 *   passo fosse maior que a espessura de uma parede, o personagem
 *   apareceria do outro lado sem que a colisão fosse detectada. Um
 *   quadro lento produz um passo grande (e correr dobra o passo), por
 *   isso o deslocamento máximo testado de uma vez é limitado a
 *   PASSO_MAXIMO = 0.5 bloco: um deslocamento maior é dividido em
 *   pedaços iguais, e cada pedaço é testado e aplicado em sequência.
 *   Assim o personagem continua andando na velocidade certa, mas
 *   nenhum teste salta mais que meio bloco, menos que qualquer parede.
 *
 * Como o código produz o efeito
 *   moveJogador() calcula em quantos pedaços dividir o deslocamento
 *   (comprimento / PASSO_MAXIMO, arredondado para cima) e chama
 *   movePedaco() para cada um. Em movePedaco(), três if, um por eixo,
 *   na ordem x, z, y. Cada um testa a posição com SÓ aquele eixo
 *   alterado, já partindo do resultado do eixo anterior. O eixo y
 *   existe porque, sem gravidade, as setas sobem e descem; o mesmo
 *   teste impede atravessar o chão e as nuvens.
 *
 *   Com VERSAO_COLISAO 1 a 3, os três eixos são testados juntos, para
 *   mostrar o travamento na diagonal.
 *
 *   Limitação: quando um passo é recusado, o personagem fica onde
 *   estava, e sobra uma pequena folga até a parede (no máximo um passo:
 *   a 60 quadros por segundo, cerca de 0.08 bloco andando e 0.17
 *   correndo).
 *
 * Pergunta provável
 *   "Por que o personagem não trava ao encostar na diagonal?"
 *   Porque a componente que entra na parede é recusada sozinha, e a
 *   componente paralela à parede continua sendo aplicada.
 * ================================================================== */

/* Aplica um pedaço do deslocamento, já pequeno o bastante para não
   atravessar paredes */
static void movePedaco(float dx, float dy, float dz)
{
#if VERSAO_COLISAO >= 4
    if (posicaoLivre(jogadorX + dx, jogadorY, jogadorZ)) jogadorX += dx;
    if (posicaoLivre(jogadorX, jogadorY, jogadorZ + dz)) jogadorZ += dz;
    if (posicaoLivre(jogadorX, jogadorY + dy, jogadorZ)) jogadorY += dy;
#else
    if (posicaoLivre(jogadorX + dx, jogadorY + dy, jogadorZ + dz)) {
        jogadorX += dx;
        jogadorY += dy;
        jogadorZ += dz;
    }
#endif
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
