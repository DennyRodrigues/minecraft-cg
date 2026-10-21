/*
 * config.h
 *
 * Todas as constantes do jogo: chaves de demonstração, tipos de bloco,
 * tamanho do mundo, medidas do personagem e da câmera. Ficam juntas
 * para que mudar um valor (a velocidade, a altura do personagem) seja
 * mexer num lugar só, e para que todos os módulos vejam o mesmo valor.
 */

#ifndef CONFIG_H
#define CONFIG_H

#define PI 3.14159265f

/* ------------------------------------------------------------------ */
/* Demonstração (semana 5)                                            */
/* Cada chave abaixo troca uma parte do código por uma versão         */
/* "errada", para mostrar em sala o defeito que a versão final        */
/* corrige. O jeito mais simples de trocar é pelo make, por exemplo   */
/* "make run VERSAO_COLISAO=3"; o Makefile recompila tudo sozinho     */
/* quando as chaves mudam. O valor escrito aqui é o da entrega.       */
/*                                                                    */
/*   EXPERIMENTO_TIMER  0: atualiza sempre que a GLUT está ociosa     */
/*                      16 ou 50: atualiza a cada 16 ou 50 ms, para   */
/*                      simular uma máquina rápida e uma lenta        */
/*   USAR_DT            0: passo fixo por quadro (anda mais rápido    */
/*                      com mais FPS); 1: velocidade x tempo          */
/*   VERSAO_COLISAO     1: só o ponto central; 2: caixa inteira;      */
/*                      3: dois eixos juntos (trava na diagonal);     */
/*                      4: eixo a eixo (desliza na parede)            */
/*   LIMITAR_PITCH      0: sem limite (a imagem inverte ao olhar      */
/*                      para cima); 1: limitado a 89 graus            */
/*                                                                    */
/* A versão original, com movimento por evento de tecla, é o commit   */
/* 5c4bbd3 do repositório.                                            */
/* ------------------------------------------------------------------ */

#ifndef EXPERIMENTO_TIMER
#define EXPERIMENTO_TIMER 0
#endif

#ifndef USAR_DT
#define USAR_DT 1
#endif

#ifndef VERSAO_COLISAO
#define VERSAO_COLISAO 4
#endif

#ifndef LIMITAR_PITCH
#define LIMITAR_PITCH 1
#endif

/* ------------------------------------------------------------------ */
/* Tipos de bloco                                                     */
/* Cada tipo é só um número com nome. O número não muda a forma do    */
/* bloco (todos são o mesmo cubo); ele só escolhe a cor, lá em        */
/* corDaFace(). Para criar um tipo novo: um #define aqui e um case    */
/* com a cor em corDaFace().                                          */
/* VAZIO marca uma posição sem bloco na matriz do mundo. Os tipos de  */
/* PELE a OLHO não aparecem no mundo: são as cores do personagem.     */
/* ------------------------------------------------------------------ */

#define VAZIO   0
#define GRAMA   1
#define TERRA   2
#define PEDRA   3
#define TRONCO  4
#define FOLHA   5
#define AREIA   6
#define AGUA    7
#define NUVEM   8
#define NEVE    9
#define PELE    10
#define CABELO  11
#define CAMISA  12
#define CALCA   13
#define SAPATO  14
#define OLHO_BRANCO 15
#define OLHO    16

/* ------------------------------------------------------------------ */
/* Tamanho do mundo                                                   */
/* ------------------------------------------------------------------ */

#define LADO        16  /* o chão tem LADO x LADO quadrados (colunas) */
#define ALTURA_MUNDO 16 /* andares em y, de 0 a 15 */
#define NIVEL_AGUA  1   /* colunas com altura até este valor são praia ou
                           mar; acima do chão delas, até esta altura, vai
                           água */
#define NIVEL_NEVE  6   /* colunas com altura a partir deste valor têm
                           neve no topo em vez de grama */
#define ALTURA_NUVEM 14 /* altura (y) das nuvens; bem acima da neve, para
                           não se confundir com ela */

/* ------------------------------------------------------------------ */
/* Personagem                                                         */
/* Para a colisão, o personagem é uma caixa de LARGURA_JOGADOR x      */
/* ALTURA_JOGADOR x LARGURA_JOGADOR, centrada em x e z e apoiada nos  */
/* pés. Os olhos ficam ALTURA_OLHOS acima dos pés.                    */
/* ------------------------------------------------------------------ */

#define LARGURA_JOGADOR 0.6f    /* lado da caixa em x e z */
#define ALTURA_JOGADOR  1.8f    /* dos pés ao topo da cabeça */
#define ALTURA_OLHOS    1.6f    /* dos pés aos olhos */

#define VELOCIDADE       5.0f   /* blocos por segundo */
#define VELOCIDADE_GIRO  90.0f  /* graus por segundo (setas) */
#define PASSO_POR_QUADRO 0.1f   /* só com USAR_DT 0 */
#define GIRO_POR_QUADRO  2.0f   /* só com USAR_DT 0 */
#define DT_MAXIMO        0.1f   /* maior tempo de quadro aceito, em s */
#define FATOR_CORRIDA    2.0f   /* segurando R, a velocidade dobra */
#define PASSO_MAXIMO     0.5f   /* maior deslocamento testado de uma vez */

#define SENSIBILIDADE 0.1f      /* graus de giro por pixel de mouse */
#define LIMITE_PITCH  89.0f     /* inclinação máxima, para cima e para baixo */

/* ------------------------------------------------------------------ */
/* Câmera                                                             */
/* ------------------------------------------------------------------ */

#define PRIMEIRA_PESSOA 0
#define TERCEIRA_PESSOA 1

#define DISTANCIA_CAMERA 4.0f   /* recuo da câmera em terceira pessoa */
#define ELEVACAO_CAMERA  1.5f   /* altura da câmera acima da cabeça */

/* ------------------------------------------------------------------ */
/* Boneco (desenho do personagem em terceira pessoa)                  */
/* Medidas em blocos, no estilo do Minecraft: pernas, tronco e cabeça */
/* empilhados até ALTURA_JOGADOR, braços dos lados do tronco. Tudo    */
/* cabe num círculo de raio 0.3 em volta do eixo vertical, para o     */
/* boneco girado não sair da caixa de colisão (ver desenhaJogador).   */
/* ------------------------------------------------------------------ */

#define COMPRIMENTO_MEMBRO 0.7f /* braços e pernas */
#define PROFUNDIDADE_CORPO 0.2f /* espessura (z) de tronco e membros */
#define LARGURA_PERNA      0.15f
#define LARGURA_TRONCO     0.3f
#define LARGURA_BRACO      0.1f
#define LADO_CABECA        0.38f
#define BALANCO_MAXIMO     30.0f    /* graus de balanço de braços e pernas */
#define FASE_POR_BLOCO     3.9f     /* radianos de balanço por bloco andado */
#define VELOCIDADE_AMPLITUDE 6.0f   /* quão rápido o balanço começa e para */

#define FOV_MIN 20.0f
#define FOV_MAX 90.0f

#endif
