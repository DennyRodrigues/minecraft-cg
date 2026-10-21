/*
 * entrada.c
 *
 * Teclado (por evento e por estado), mouse (giro do olhar e captura do
 * cursor), roda e teclas + e - (zoom), e a atualização do movimento a
 * cada quadro, escalada pelo tempo de quadro.
 */

#include <stdlib.h>             /* exit */
#include <math.h>               /* sinf, cosf */
#include <ctype.h>              /* tolower */

#include "graficos.h"
#include "config.h"
#include "colisao.h"
#include "camera.h"
#include "entrada.h"
#include "menu.h"

/* ==================================================================
 * TECLADO E TECLADOSOLTO: transformam eventos de tecla em estado
 *
 * O problema
 *   Na semana 4 cada evento de tecla movia a câmera um passo fixo. Ao
 *   segurar W, o primeiro passo saía na hora, depois havia uma pausa e
 *   só então os passos seguiam numa cadência regular. Essa hesitação não
 *   vinha do nosso código: vinha da repetição de tecla do sistema
 *   operacional, pensada para digitação (um atraso inicial, para não
 *   repetir letras sem querer, e depois uma cadência própria). Movimento
 *   conduzido por esse evento herda o atraso e a cadência.
 *
 * A teoria
 *   A entrada não chega à aplicação no instante em que o usuário age. O
 *   sistema operacional coloca cada acontecimento (tecla pressionada,
 *   tecla solta, mouse movido) numa fila. Na GLUT, quem esvazia a fila é
 *   glutMainLoop: para cada item, ele chama a função que a aplicação
 *   registrou para aquele tipo de acontecimento. A aplicação não escreve
 *   o laço; ela registra funções e reage a elas.
 *
 *   Há duas formas de ler a entrada:
 *     por evento  uma função é chamada a cada acontecimento. Serve para
 *                 ações discretas, que acontecem uma vez por toque:
 *                 sair, alternar projeção, alternar câmera.
 *     por estado  a cada quadro, pergunta-se "esta tecla está
 *                 pressionada agora?". Serve para ações contínuas, que
 *                 duram enquanto a tecla estiver pressionada: andar,
 *                 girar.
 *
 *   A GLUT só oferece eventos; não há função que responda se uma tecla
 *   está pressionada. O estado precisa ser construído pela aplicação:
 *   um vetor com uma posição por tecla, posto em 1 no evento de
 *   pressionar e em 0 no de soltar. O estado é a memória dos eventos
 *   passados: entre um pressionar e um soltar, a tecla "está"
 *   pressionada.
 *
 * Como o código produz o efeito
 *   teclado() e tecladoSolto() só escrevem em teclas[]; quem lê o vetor
 *   e move o personagem é atualiza(), uma vez por quadro. Com o estado
 *   construído assim, os eventos de repetição não acrescentam nada (a
 *   tecla já está marcada), e main() os desliga com glutIgnoreKeyRepeat.
 *
 *   As ações discretas continuam no switch de teclado(), porque devem
 *   acontecer uma vez por toque. Se P fosse lido por estado, a projeção
 *   alternaria a cada quadro enquanto o dedo estivesse na tecla.
 *
 *   tolower() aparece nos dois lados porque a GLUT entrega 'w' ou 'W'
 *   conforme o Shift. Se o usuário pressiona w, aperta Shift e solta o
 *   w, o evento de soltar chega como 'W'; sem converter, teclas['w']
 *   ficaria em 1 para sempre.
 *
 *   As setas chegam por outro par de funções (especial e especialSolto),
 *   com o mesmo esquema num segundo vetor.
 *
 * Pergunta provável
 *   "Por que não mover o personagem direto dentro de teclado()?"
 *   Porque teclado() só roda quando chega um evento, e os eventos de
 *   tecla segurada chegam no ritmo do sistema operacional, não no dos
 *   quadros. Movendo em atualiza(), o movimento acontece em todo
 *   quadro, começa no instante do pressionar e para no do soltar.
 * ================================================================== */

int teclas[256];        /* teclas de caractere: 1 pressionada, 0 solta */
int especiais[256];     /* teclas especiais (setas, F5): idem */
void teclado(unsigned char tecla, int x, int y)
{
    (void)x; (void)y;                       /* posição do mouse: não usada */

    teclas[tolower(tecla)] = 1;             /* estado: para as ações contínuas */

    if (menuAberto) {                       /* com o menu, só duas teclas valem */
        if (tecla == 27) fechaMenu();       /* 27 é o código da tecla ESC */
        else if (tolower(tecla) == 'q') exit(0);
        return;
    }

    switch (tolower(tecla)) {               /* evento: para as ações discretas */
        case 27:  abreMenu(); break;        /* ESC: menu de pausa */
        case 9:   alternaCaptura(); break;  /* 9 é o código da tecla Tab */
        case 'p':
            projecaoOrtogonal = !projecaoOrtogonal;
            aplicaProjecao();
            break;
        case 'v':
            modoCamera = (modoCamera == PRIMEIRA_PESSOA) ? TERCEIRA_PESSOA
                                                         : PRIMEIRA_PESSOA;
            break;
        case '+': case '=': mudaZoom(+1); break;    /* '=' é o + sem Shift */
        case '-': case '_': mudaZoom(-1); break;
    }
}

void tecladoSolto(unsigned char tecla, int x, int y)
{
    (void)x; (void)y;
    teclas[tolower(tecla)] = 0;
}

void especial(int tecla, int x, int y)
{
    (void)x; (void)y;

    if (tecla >= 0 && tecla < 256) especiais[tecla] = 1;

    if (tecla == GLUT_KEY_F5 && !menuAberto)               /* evento: alterna a câmera */
        modoCamera = (modoCamera == PRIMEIRA_PESSOA) ? TERCEIRA_PESSOA
                                                     : PRIMEIRA_PESSOA;
}

void especialSolto(int tecla, int x, int y)
{
    (void)x; (void)y;
    if (tecla >= 0 && tecla < 256) especiais[tecla] = 0;
}

/* ==================================================================
 * MOVIMENTOMOUSE: o deslocamento do mouse gira o olhar
 *
 * O problema
 *   Para orientar a câmera não interessa onde o cursor está, e sim
 *   quanto ele andou: se a posição absoluta decidisse o ângulo, o giro
 *   acabaria na borda da janela. E a GLUT não oferece captura do
 *   cursor, aquele modo em que o mouse anda sem fim e o cursor some.
 *
 * A teoria
 *   A posição do cursor chega em coordenadas de TELA: origem no canto
 *   superior esquerdo e y crescendo para BAIXO, ao contrário das
 *   coordenadas normalizadas da semana 2 (y para cima). Por isso o
 *   pitch usa -dy: mouse para cima dá dy negativo, e o olhar deve subir.
 *   Esquecer essa inversão é a causa clássica de visada invertida.
 *
 *   A captura é simulada em dois passos: o cursor é ocultado
 *   (glutSetCursor(GLUT_CURSOR_NONE)) e, depois de cada movimento,
 *   devolvido ao centro da janela (glutWarpPointer). Assim ele nunca
 *   chega à borda.
 *
 *   O ponto delicado: reposicionar o cursor também gera um evento de
 *   movimento. Se o deslocamento fosse medido em relação à ÚLTIMA
 *   posição, esse evento seria lido como um movimento de volta e
 *   desfaria o giro. Medindo sempre em relação ao CENTRO, o evento
 *   provocado pelo reposicionamento chega exatamente no centro, dá
 *   deslocamento zero e é ignorado.
 *
 *   A GLUT separa o movimento com botão pressionado (glutMotionFunc) do
 *   movimento sem botão (glutPassiveMotionFunc). Esta função é
 *   registrada nas duas, senão o olhar congelaria enquanto um botão
 *   estivesse pressionado.
 *
 * Como o código produz o efeito
 *   Sem captura, nada acontece. Logo depois de capturar (no início do
 *   jogo, com Tab ou com um clique) e quando o cursor volta a entrar na
 *   janela, o cursor pode estar em qualquer lugar: o primeiro evento só
 *   o leva ao centro, sem girar, senão a distância até o centro viraria
 *   um tranco no olhar. Daí em diante: (dx, dy) é a distância ao centro;
 *   zero é o eco do reposicionamento e é descartado. A SENSIBILIDADE
 *   converte pixels em graus. O pitch é limitado (ver o comentário
 *   seguinte) e o cursor volta ao centro.
 *
 * Pergunta provável
 *   "O reposicionamento não gera um evento que gira a câmera de volta?"
 *   Gera o evento, mas ele chega com o cursor no centro, então o
 *   deslocamento calculado é zero e a função retorna antes de girar.
 * ================================================================== */

/* ==================================================================
 * LIMITE DO PITCH: por que a inclinação para em 89 graus
 *
 * O problema
 *   Sem limite (LIMITAR_PITCH 0), ao olhar diretamente para cima a
 *   imagem gira de forma brusca e, passando dos 90 graus, fica de
 *   cabeça para baixo.
 *
 * A teoria
 *   O gluLookAt monta os eixos da câmera a partir da direção do olhar e
 *   do vetor "cima" (0, 1, 0). O eixo "direita" da câmera é o produto
 *   vetorial direção x cima. O produto vetorial de dois vetores
 *   paralelos é o vetor nulo: quando a direção fica paralela a (0, 1, 0)
 *   (pitch 90), não existe "direita" e os eixos da câmera deixam de
 *   existir. Logo depois dos 90, a "direita" troca de sinal e a imagem
 *   vira. É o caso da segunda pergunta de verificação da semana 4, que
 *   lá era teórico e aqui aparece como defeito concreto.
 *
 * Como o código produz o efeito
 *   Dois if mantêm o pitch entre -89 e 89. Não é 90 porque em 90 a
 *   conta já degenera; com 89 a direção nunca fica paralela a "cima".
 * ================================================================== */

int cursorCapturado = 0;    /* 1 enquanto o mouse controla o olhar */
int recentrar = 0;          /* 1: o próximo evento só leva o cursor ao centro */

void movimentoMouse(int x, int y)
{
    int cx = larguraJanela / 2, cy = alturaJanela / 2;
    int dx = x - cx, dy = y - cy;

    if (!cursorCapturado) return;

    if (recentrar) {                        /* cursor fora do centro: só */
        recentrar = 0;                      /* reposiciona, sem girar    */
        glutWarpPointer(cx, cy);
        return;
    }

    if (dx == 0 && dy == 0) return;         /* eco do glutWarpPointer */

    yaw   += dx * SENSIBILIDADE;
    pitch -= dy * SENSIBILIDADE;            /* y de tela cresce para baixo */

#if LIMITAR_PITCH
    if (pitch >  LIMITE_PITCH) pitch =  LIMITE_PITCH;
    if (pitch < -LIMITE_PITCH) pitch = -LIMITE_PITCH;
#endif

    glutWarpPointer(cx, cy);                /* devolve o cursor ao centro */
}

/* Captura e liberação do cursor. O jogo começa com o cursor capturado;
   Tab alterna, e um clique na janela captura de novo. Sem uma forma de
   liberar, o usuário não conseguiria sair da janela.
   capturaCursor() não move o cursor na hora: só o oculta e pede que o
   próximo movimento o leve ao centro. Assim funciona também no início,
   quando a janela talvez ainda nem esteja na tela, e o cursor já está
   oculto quando é reposicionado (em algumas sessões Wayland o
   reposicionamento só funciona com o cursor oculto). */
void capturaCursor(void)
{
    cursorCapturado = 1;
    recentrar = 1;
    glutSetCursor(GLUT_CURSOR_NONE);
}

void liberaCursor(void)
{
    cursorCapturado = 0;
    glutSetCursor(GLUT_CURSOR_INHERIT);
}

void alternaCaptura(void)
{
    if (cursorCapturado) liberaCursor();
    else                 capturaCursor();
}

/* Clique com o cursor liberado: captura de novo */
void cliqueMouse(int botao, int estado, int x, int y)
{
    (void)botao; (void)x; (void)y;
    if (estado == GLUT_DOWN && !cursorCapturado && !menuAberto)
        capturaCursor();
}

/* O cursor entrou na janela (por exemplo, depois de trocar de janela
   com Alt+Tab): ele chega pela borda, então o próximo movimento só o
   leva ao centro, sem girar o olhar */
void entradaJanela(int estado)
{
    if (estado == GLUT_ENTERED) recentrar = 1;
}

/* ==================================================================
 * RODAMOUSE: a roda troca a lente, não move a câmera
 *
 * O problema
 *   Queremos aproximar a imagem (zoom) sem tirar o personagem do lugar.
 *
 * A teoria
 *   Existem duas formas de algo parecer maior na tela: chegar mais perto
 *   (mudar a matriz de visualização) ou trocar a lente (mudar a matriz
 *   de projeção). A roda faz a segunda: diminui o campo de visão, a
 *   abertura do frustum (ver APLICAPROJECAO). Com uma abertura menor, a
 *   mesma tela mostra um pedaço menor do mundo, e esse pedaço aparece
 *   maior: a escala na tela é proporcional a 1 / tan(fov / 2).
 *
 *   A diferença se vê na imagem: andando para a frente, o que está perto
 *   cresce muito mais que o que está longe; com o zoom, perto e longe
 *   crescem na mesma proporção, como numa luneta.
 *
 * Como o código produz o efeito
 *   A roda e as teclas + e - chamam a mesma função, mudaZoom(), para as
 *   duas entradas terem exatamente o mesmo efeito. Na roda, direcao vale
 *   +1 (para cima) ou -1 (para baixo); no teclado, + vale +1 e - vale
 *   -1. As teclas são tratadas por evento (uma vez por toque), como um
 *   clique da roda. Cada passo
 *   muda o fov em 5 graus, limitado entre FOV_MIN e FOV_MAX: perto de 0
 *   a imagem vira uma luneta, perto de 180 tudo se distorce. Depois a
 *   lente é refeita com aplicaProjecao(). Na projeção ortográfica o fov
 *   não tem efeito, porque ela não tem abertura.
 *
 *   A roda não faz parte da GLUT original: glutMouseWheelFunc é uma
 *   extensão da freeglut, e por isso o arquivo inclui GL/freeglut.h.
 *
 * Pergunta provável
 *   "Qual a diferença entre o zoom e andar para a frente?"
 *   Andar muda a posição do olho e, com ela, a perspectiva (o que está
 *   perto se aproxima mais rápido); o zoom não muda a posição e amplia
 *   tudo por igual.
 * ================================================================== */

/* +1 aproxima, -1 afasta; usada pela roda e pelas teclas + e - */
void mudaZoom(int direcao)
{
    campoDeVisao -= direcao * 5.0f;         /* fov menor = mais perto */
    if (campoDeVisao < FOV_MIN) campoDeVisao = FOV_MIN;
    if (campoDeVisao > FOV_MAX) campoDeVisao = FOV_MAX;

    aplicaProjecao();
}

void rodaMouse(int roda, int direcao, int x, int y)
{
    (void)roda; (void)x; (void)y;
    if (!menuAberto)
        mudaZoom(direcao);                  /* roda para cima aproxima */
}

/* ==================================================================
 * ATUALIZA: move o personagem a cada quadro, escalado pelo tempo
 *
 * O problema
 *   Com a tecla lida por estado, o movimento acontece a cada quadro.
 *   Mas, se cada quadro andar um passo fixo, a velocidade passa a
 *   depender da máquina: com 0.1 bloco por quadro, quem roda a 60 FPS
 *   anda 6 blocos por segundo, e quem roda a 20 FPS anda só 2.
 *   (Experimente: USAR_DT 0 com EXPERIMENTO_TIMER 16 e depois 50.)
 *
 * A teoria
 *   O tempo de quadro, dt, é o tempo que passou desde a atualização
 *   anterior, obtido pela diferença entre duas leituras do relógio
 *   (glutGet(GLUT_ELAPSED_TIME), em milissegundos desde o início).
 *   Em vez de "um passo por quadro", o movimento passa a ser
 *
 *       posição = posição + velocidade x dt
 *
 *   que é o movimento uniforme da física, s = s0 + v·t, aplicado aos
 *   pedaços. A 60 FPS são 60 pedaços de 1/60 s; a 20 FPS, 20 pedaços de
 *   1/20 s. Nos dois casos a soma, em um segundo, é a VELOCIDADE.
 *
 *   dt é limitado a DT_MAXIMO. Se a janela travar por um segundo (ao
 *   ser arrastada, por exemplo), o próximo quadro não dá um salto de 5
 *   blocos, que atravessaria paredes (ver tunelamento em MOVEJOGADOR).
 *
 *   A GLUT chama esta função quando não tem eventos para tratar
 *   (glutIdleFunc), ou seja, o mais rápido possível. Ela atualiza o
 *   estado e termina pedindo um novo desenho com glutPostRedisplay:
 *   atualizar e desenhar ficam em funções separadas.
 *
 * Como o código produz o efeito
 *   1. Mede dt e limita a DT_MAXIMO.
 *   2. Calcula o passo (blocos) e o giro (graus) deste quadro. Segurar R
 *      multiplica o passo por FATOR_CORRIDA: a "aceleração" do quadro
 *      1.2 do plano de aula, lida por estado como o próprio andar.
 *   3. As setas laterais giram o yaw.
 *   4. A partir do yaw, as direções "frente" e "direita" no chão. No
 *      círculo unitário, (sen(yaw), -cos(yaw)) aponta para onde o
 *      personagem olha (yaw 0 olha para -z); a direita é a frente girada
 *      90 graus, (cos(yaw), sen(yaw)).
 *      Só o yaw entra aqui, e não o pitch: o avanço é a direção do olhar
 *      projetada no plano do chão. Por isso olhar para cima não faz o
 *      personagem sair do chão.
 *   5. Cada tecla vale 1 ou 0, então W - S diz se o personagem vai para
 *      a frente (1), para trás (-1) ou nenhum dos dois (0); D - A, o
 *      mesmo para os lados. A soma frente·(W-S) + direita·(D-A) é a
 *      direção desejada no chão. Na diagonal, ela é a soma de dois
 *      vetores unitários perpendiculares e mede raiz(2) = 1.41: sem
 *      correção, andar na diagonal seria 41% mais rápido. Dividir o
 *      vetor pelo próprio comprimento (normalizar) o deixa com
 *      comprimento 1 em qualquer direção, e só então ele é multiplicado
 *      pelo passo.
 *   6. Entrega o deslocamento a moveJogador(), que testa a colisão.
 *   7. Atualiza a animação de caminhada a partir do quanto o personagem
 *      REALMENTE andou (ver desenhaJogador): a fase avança com a
 *      distância, e a amplitude do balanço sobe enquanto ele anda e
 *      desce até zero quando ele para.
 *
 * Pergunta provável
 *   "Por que dividir por 1000.0f e não por 1000?"
 *   GLUT_ELAPSED_TIME é inteiro. Dividindo por 1000 (inteiro), 16 ms
 *   virariam 0 segundos e o personagem nunca andaria.
 * ================================================================== */

int ultimoTempo = 0;    /* tempo da atualização anterior, em ms */

float faseCaminhada = 0.0f;      /* radianos; avança com a distância andada */
float amplitudeCaminhada = 0.0f; /* 0 parado, 1 andando; suaviza o balanço */

void atualiza(void)
{
    int agora = glutGet(GLUT_ELAPSED_TIME);
    float dt = (agora - ultimoTempo) / 1000.0f;
    ultimoTempo = agora;
    if (dt > DT_MAXIMO) dt = DT_MAXIMO;

    if (menuAberto) {                       /* pausa: o relógio continua sendo */
        glutPostRedisplay();                /* lido, para o primeiro dt depois */
        return;                             /* de fechar não ser enorme */
    }

#if USAR_DT
    float passo = VELOCIDADE * dt;
    float giro  = VELOCIDADE_GIRO * dt;
#else
    float passo = PASSO_POR_QUADRO;         /* depende da taxa de quadros */
    float giro  = GIRO_POR_QUADRO;
#endif
    if (teclas['r']) passo *= FATOR_CORRIDA;    /* correr */

    if (especiais[GLUT_KEY_LEFT])  yaw -= giro;
    if (especiais[GLUT_KEY_RIGHT]) yaw += giro;

    float rad = yaw * PI / 180.0f;
    float frenteX  =  sinf(rad), frenteZ  = -cosf(rad);
    float direitaX =  cosf(rad), direitaZ =  sinf(rad);

    /* direção desejada no chão: cada tecla vale 1 (pressionada) ou 0 */
    float frente = (float)(teclas['w'] - teclas['s']);
    float lado   = (float)(teclas['d'] - teclas['a']);
    float mx = frenteX * frente + direitaX * lado;
    float mz = frenteZ * frente + direitaZ * lado;

    float comprimento = sqrtf(mx * mx + mz * mz);
    if (comprimento > 0.0f) {               /* normaliza: na diagonal o */
        mx /= comprimento;                  /* comprimento seria 1.41   */
        mz /= comprimento;
    }

    /* Shift: a GLUT original não avisa quando só o Shift é pressionado,
       mas a freeglut (a partir da 3.0) o entrega como tecla especial,
       GLUT_KEY_SHIFT_L ou _R, e ele entra em especiais[] como as setas */
    int sobe  = especiais[GLUT_KEY_UP]   || teclas[' '];
    int desce = especiais[GLUT_KEY_DOWN] || especiais[GLUT_KEY_SHIFT_L]
                                         || especiais[GLUT_KEY_SHIFT_R];
    float subir = (float)(sobe - desce);

    float antesX = jogadorX, antesZ = jogadorZ;
    moveJogador(mx * passo, subir * passo, mz * passo);

    /* animação: quanto o personagem realmente andou no chão (a colisão
       pode ter recusado parte do movimento) */
    float andou = sqrtf((jogadorX - antesX) * (jogadorX - antesX) +
                        (jogadorZ - antesZ) * (jogadorZ - antesZ));
    faseCaminhada += andou * FASE_POR_BLOCO;
    if (andou > 0.0f) amplitudeCaminhada += VELOCIDADE_AMPLITUDE * dt;
    else              amplitudeCaminhada -= VELOCIDADE_AMPLITUDE * dt;
    if (amplitudeCaminhada > 1.0f) amplitudeCaminhada = 1.0f;
    if (amplitudeCaminhada < 0.0f) amplitudeCaminhada = 0.0f;

    glutPostRedisplay();
}

/* Só com EXPERIMENTO_TIMER: atualiza em intervalos fixos, simulando uma
   máquina que só consegue EXPERIMENTO_TIMER ms por quadro, e se
   re-agenda para o próximo intervalo. */
void temporizador(int valor)
{
    (void)valor;
    atualiza();
    glutTimerFunc(EXPERIMENTO_TIMER, temporizador, 0);
}
