/*
 * mundo.h
 *
 * O cenário: o mapa de alturas, a matriz 3D com o tipo de cada bloco,
 * a construção do mundo a partir do mapa e o desenho dos blocos.
 */

#ifndef MUNDO_H
#define MUNDO_H

#include "config.h"

/* tipo de cada bloco, indexado como mundo[x][y][z] (ver mundo.c) */
extern unsigned char mundo[LADO][ALTURA_MUNDO][LADO];

/* desenho de um bloco */
void corDaFace(int tipo, int ehTopo, float brilho, float tom);
void desenhaBloco(int tipo, float tom);     /* cubo de lado 1 na origem */
void blocoEm(int tipo, int x, int y, int z);

/* acesso à matriz */
int  alturaDoTerreno(int x, int z);
int  dentroDoMundo(int x, int y, int z);
void colocaBloco(int tipo, int x, int y, int z);
int  blocoNoMundo(int x, int y, int z);

/* construção (uma vez, no início) e desenho (a cada quadro) */
void colunaEm(int x, int z);
void arvoreEm(int x, int z);
void nuvemEm(int x, int z, int largura, int fundo);
void construirMundo(void);
void desenhaCena(void);

#endif
