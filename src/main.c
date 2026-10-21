/*
 * Projeto Minecraft (main.c)
 *
 * Reúne o que foi visto nas semanas 1 a 5:
 *   Semana 1: janela, laço principal, tempo de quadro, FPS no título, ESC
 *   Semana 2: primitivas (GL_QUADS) e cor por face
 *   Semana 3: cubo 3D, buffer de profundidade, descarte de faces,
 *             transformações e instâncias com glPushMatrix/glPopMatrix
 *   Semana 4: perspectiva, correção de proporção, câmera com gluLookAt
 *             e navegação pelo teclado
 *   Semana 5: teclado lido por estado, movimento escalado pelo tempo de
 *             quadro, mouse com yaw e pitch e captura do cursor, colisão
 *             por caixa alinhada aos eixos, câmera em primeira e
 *             terceira pessoa e aproximação pela roda do mouse
 *
 * Cenário: terreno 16 x 16 com uma montanha, uma praia e o mar,
 * uma árvore e duas nuvens. Tudo é feito com um único cubo de lado 1,
 * desenhado várias vezes em posições diferentes.
 *
 * Eixos do mundo:
 *   x  esquerda (0) para a direita (15)
 *   y  altura: 0 é o fundo, cresce para cima
 *   z  fundo do mapa (0) para a frente (15), onde fica o mar
 *
 * Controles:
 *   W / S        andar para frente / para trás
 *   A / D        andar para a esquerda / para a direita
 *   R            correr (segurando, junto com W, A, S, D)
 *   Setas < >    girar o olhar
 *   Espaço ou ^  subir
 *   Shift ou v   descer
 *   Mouse        direção do olhar (já começa capturado)
 *   Roda ou + -  aproximar / afastar (campo de visão)
 *   Tab          liberar / capturar o cursor; clique também captura
 *   F5 ou V      alternar terceira (início) / primeira pessoa
 *   P            alternar perspectiva / ortográfica
 *   ESC          abrir / fechar o menu (pausa, instruções)
 *   Q            sair (com o menu aberto)
 *
 * Organização do código (ver o comentário MÓDULOS logo abaixo):
 *   src/main.c          janela, callbacks, desenho do quadro e FPS
 *   src/mundo.c         mapa, matriz do mundo, construção e desenho dos
 *                       blocos
 *   src/colisao.c       posição do personagem e colisão com o mundo
 *   src/camera.c        projeção, direção do olhar, primeira e terceira
 *                       pessoa, desenho do personagem
 *   src/entrada.c       teclado, mouse, roda e atualização do movimento
 *   src/menu.c          menu de pausa com as instruções (ESC)
 *   include/config.h    constantes do jogo
 *   include/graficos.h  cabeçalhos do OpenGL, GLU e GLUT
 *   include/            também um cabeçalho para cada módulo de src/
 *
 * Compilar e rodar:
 *   make run
 */

/* ------------------------------------------------------------------ */
/* Módulos: cada .c é compilado separadamente e o ligador junta os .o */
/* no executável. Os .h têm só declarações (funções e variáveis       */
/* extern), para outro .c poder usá-las; cada definição fica num      */
/* único .c. As guardas #ifndef evitam incluir o mesmo .h duas vezes. */
/* ------------------------------------------------------------------ */

#include <stdio.h>              /* printf, sprintf */

#include "graficos.h"
#include "config.h"
#include "mundo.h"
#include "camera.h"
#include "entrada.h"
#include "menu.h"

/* ------------------------------------------------------------------ */
/* Quadro: a GLUT chama esta função sempre que a janela precisa ser   */
/* redesenhada. A cada chamada:                                       */
/*   1. apaga a imagem anterior (cor e profundidade)                  */
/*   2. posiciona a câmera (primeira ou terceira pessoa)              */
/*   3. desenha a cena e, em terceira pessoa, o personagem            */
/*   4. mostra o resultado na tela                                    */
/*   5. atualiza a contagem de FPS                                    */
/* ------------------------------------------------------------------ */

int inicioContagem = 0;   /* início da contagem de FPS (semana 1), em ms */
int quadros = 0;          /* quadros desenhados desde inicioContagem */

void desenha(void)
{
    /* apaga a cor (fica o azul do glClearColor) e o buffer de
       profundidade, que guarda a distância de cada pixel */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    posicionaCamera();

    desenhaCena();
    if (modoCamera == TERCEIRA_PESSOA)
        desenhaJogador();
    if (menuAberto)
        desenhaMenu();                      /* por cima de tudo */

    /* desenhamos num buffer escondido; aqui ele troca de lugar com o que
       está na tela, para não aparecer a imagem pela metade */
    glutSwapBuffers();

    /* conta os quadros e, a cada 1000 ms, mostra o total no título */
    quadros++;
    int agora = glutGet(GLUT_ELAPSED_TIME);
    if (agora - inicioContagem >= 1000) {
        char titulo[64];
        sprintf(titulo, "Minecraft | %d FPS", quadros);
        glutSetWindowTitle(titulo);
        quadros = 0;
        inicioContagem = agora;
    }
}

/* ------------------------------------------------------------------ */
/* Programa principal                                                 */
/* Cria a janela, registra as funções que a GLUT vai chamar em cada   */
/* situação (callbacks), liga os testes do OpenGL, constrói o mundo e */
/* entra no laço principal, de onde não sai mais.                     */
/* ------------------------------------------------------------------ */

int main(int argc, char **argv)
{
    /* janela com buffer duplo (GLUT_DOUBLE), cores RGB e buffer de
       profundidade (GLUT_DEPTH) */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(larguraJanela, alturaJanela);
    glutCreateWindow("Minecraft");

    /* a GLEW só pode ser iniciada depois que a janela existe */
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("Erro GLEW: %s\n", glewGetErrorString(err));
        return 1;
    }

    printf("OpenGL %s | %s\n", glGetString(GL_VERSION), glGetString(GL_RENDERER));

    /* as teclas são lidas por estado: a repetição automática do sistema
       operacional não acrescenta nada e é desligada */
    glutIgnoreKeyRepeat(1);

    /* callbacks: a GLUT chama cada função quando o evento acontece */
    glutReshapeFunc(redimensiona);          /* janela abriu ou mudou de tamanho */
    glutDisplayFunc(desenha);               /* hora de desenhar */
    glutKeyboardFunc(teclado);              /* tecla comum pressionada */
    glutKeyboardUpFunc(tecladoSolto);       /* tecla comum solta */
    glutSpecialFunc(especial);              /* tecla especial pressionada */
    glutSpecialUpFunc(especialSolto);       /* tecla especial solta */
    glutPassiveMotionFunc(movimentoMouse);  /* mouse movido sem botão */
    glutMotionFunc(movimentoMouse);         /* mouse movido com botão */
    glutMouseWheelFunc(rodaMouse);          /* roda do mouse (freeglut) */
    glutMouseFunc(cliqueMouse);             /* clique: captura o cursor */
    glutEntryFunc(entradaJanela);           /* cursor entrou na janela */

    glutIdleFunc(atualiza);                 /* nada acontecendo: atualiza */

    glEnable(GL_DEPTH_TEST);                /* o mais próximo cobre o mais distante,
                                               não importa a ordem de desenho */
    glEnable(GL_CULL_FACE);                 /* não desenha faces de costas */
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);   /* cor de fundo: azul do céu */

    construirMundo();                       /* preenche a matriz, uma vez só */
    capturaCursor();                        /* o mouse já começa controlando o olhar */

    ultimoTempo = glutGet(GLUT_ELAPSED_TIME);
    inicioContagem = ultimoTempo;

    glutMainLoop();                         /* laço principal: só sai com Q no menu */
    return 0;
}
