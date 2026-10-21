/*
 * camera.h
 *
 * Orientação do olhar, lente (projeção), posição da câmera em primeira
 * e terceira pessoa e desenho do personagem.
 */

#ifndef CAMERA_H
#define CAMERA_H

extern float yaw, pitch;            /* orientação do olhar, em graus */
extern int   modoCamera;            /* PRIMEIRA_PESSOA ou TERCEIRA_PESSOA */

extern int   larguraJanela, alturaJanela;
extern int   projecaoOrtogonal;     /* 0 perspectiva, 1 ortográfica */
extern float campoDeVisao;          /* graus */

void aplicaProjecao(void);
void redimensiona(int largura, int altura);
void direcaoDoOlhar(float *dx, float *dy, float *dz);
void desenhaJogador(void);
void posicionaCamera(void);

#endif
