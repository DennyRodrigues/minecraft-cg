/*
 * menu.h
 *
 * Menu de pausa, aberto e fechado com ESC, com as instruções de
 * movimentação. Enquanto ele está aberto o jogo fica parado e o cursor
 * fica livre.
 */

#ifndef MENU_H
#define MENU_H

extern int menuAberto;              /* 1 enquanto o menu está na tela */

void abreMenu(void);
void fechaMenu(void);
void alternaMenu(void);
void desenhaMenu(void);             /* chamada por desenha(), depois da cena */

#endif
