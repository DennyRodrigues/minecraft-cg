/*
 * graficos.h
 *
 * Cabeçalhos do OpenGL, da GLU e da GLUT, os mesmos para todos os
 * módulos que desenham ou recebem entrada. Ficam num lugar só para a
 * ordem de inclusão (a GLEW precisa vir antes do OpenGL) não ser
 * repetida, e talvez errada, em cada arquivo.
 */

#ifndef GRAFICOS_H
#define GRAFICOS_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>            /* carrega as funções do OpenGL */
#include <GL/freeglut.h>        /* janela, teclado, mouse e laço principal;
                                   a versão freeglut traz a roda do mouse */
#include <GL/glu.h>             /* gluPerspective e gluLookAt */

#endif
