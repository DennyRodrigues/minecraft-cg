/*
 * entrada.h
 *
 * Teclado, mouse e roda, e a atualização do movimento a cada quadro.
 * As funções de callback são registradas na GLUT por main().
 */

#ifndef ENTRADA_H
#define ENTRADA_H

extern int ultimoTempo;             /* tempo da atualização anterior, em ms */

/* animação de caminhada, atualizada em atualiza() e usada pelo desenho
   do personagem */
extern float faseCaminhada;         /* radianos; avança com a distância */
extern float amplitudeCaminhada;    /* 0 parado, 1 andando */

/* teclado */
void teclado(unsigned char tecla, int x, int y);
void tecladoSolto(unsigned char tecla, int x, int y);
void especial(int tecla, int x, int y);
void especialSolto(int tecla, int x, int y);

/* mouse */
void movimentoMouse(int x, int y);
void capturaCursor(void);
void liberaCursor(void);
void alternaCaptura(void);
void cliqueMouse(int botao, int estado, int x, int y);
void entradaJanela(int estado);
void mudaZoom(int direcao);
void rodaMouse(int roda, int direcao, int x, int y);

/* atualização */
void atualiza(void);

#endif
