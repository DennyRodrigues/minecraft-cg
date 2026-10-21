/*
 * colisao.h
 *
 * A posição do personagem e a colisão dele com o mundo.
 */

#ifndef COLISAO_H
#define COLISAO_H

/* posição do personagem; jogadorY é a altura dos PÉS (ver colisao.c) */
extern float jogadorX, jogadorY, jogadorZ;

int  blocoSolido(int x, int y, int z);
int  posicaoLivre(float x, float y, float z);
void moveJogador(float dx, float dy, float dz);

#endif
