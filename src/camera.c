/*
 * camera.c
 *
 * Tudo o que decide COMO a cena é vista: a lente (projeção), a
 * direção do olhar a partir de yaw e pitch, a posição da câmera em
 * primeira e terceira pessoa e o desenho do personagem, que só aparece
 * em terceira pessoa.
 */

#include <math.h>               /* sinf, cosf */

#include "graficos.h"
#include "config.h"
#include "mundo.h"              /* desenhaBloco, para o personagem */
#include "colisao.h"            /* posição do personagem */
#include "entrada.h"            /* estado da animação de caminhada */
#include "camera.h"

/* ------------------------------------------------------------------ */
/* Orientação e modo de câmera                                        */
/* yaw é o giro em torno do eixo vertical (0 olha para -z) e pitch a  */
/* inclinação (0 olha na horizontal, positivo para cima). O teclado   */
/* e o mouse os alteram; posicionaCamera() os usa a cada quadro.      */
/* ------------------------------------------------------------------ */

float yaw   = 0.0f;
float pitch = 0.0f;
int modoCamera = TERCEIRA_PESSOA;    /* começa vendo o personagem */

/* ------------------------------------------------------------------ */
/* Janela e projeção                                                  */
/* ------------------------------------------------------------------ */

int larguraJanela = 1024;       /* atualizados em redimensiona() */
int alturaJanela  = 768;
int projecaoOrtogonal = 0;      /* 0 perspectiva, 1 ortográfica (tecla P) */
float campoDeVisao = 60.0f;     /* graus; alterado pela roda do mouse */

/* ------------------------------------------------------------------ */
/* Projeção: monta a "lente" da câmera na matriz GL_PROJECTION.       */
/* Perspectiva (gluPerspective): campo de visão, proporção da janela, */
/* plano próximo e distante; o que está longe fica menor. Ortográfica */
/* (glOrtho): uma caixa, em que o tamanho não depende da distância. A */
/* proporção evita a imagem esticada. O plano próximo é 0.1 nos dois  */
/* modos: positivo, senão blocos atrás do olho entrariam na imagem.   */
/* Chamada ao redimensionar, na tecla P e no zoom.                    */
/* ------------------------------------------------------------------ */

void aplicaProjecao(void)
{
    double aspecto = (double)larguraJanela / alturaJanela;

    glMatrixMode(GL_PROJECTION);            /* as próximas operações mexem na lente */
    glLoadIdentity();

    if (projecaoOrtogonal)
        glOrtho(-10.0 * aspecto, 10.0 * aspecto, -10.0, 10.0, 0.1, 200.0);
    else
        gluPerspective(campoDeVisao, aspecto, 0.1, 200.0);

    glMatrixMode(GL_MODELVIEW);             /* volta para a matriz dos objetos */
}

/* ------------------------------------------------------------------ */
/* Redimensionamento (semana 4)                                       */
/* A GLUT chama esta função ao abrir a janela e sempre que ela muda   */
/* de tamanho. Guarda o tamanho (a projeção e o centro do mouse       */
/* dependem dele), ajusta a área de desenho e refaz a lente.          */
/* ------------------------------------------------------------------ */

void redimensiona(int largura, int altura)
{
    if (altura == 0) altura = 1;            /* evita divisão por zero */

    larguraJanela = largura;
    alturaJanela  = altura;

    glViewport(0, 0, largura, altura);      /* desenha na janela inteira */
    aplicaProjecao();
}

/* ------------------------------------------------------------------ */
/* Direção do olhar a partir de yaw e pitch (coordenadas esféricas).  */
/* O pitch divide o vetor unitário entre vertical, sen(pitch), e      */
/* horizontal, cos(pitch); o yaw distribui a parte horizontal entre x */
/* e z, com yaw 0 olhando para -z. Quem chama soma o vetor ao olho    */
/* para obter o alvo do gluLookAt.                                    */
/* ------------------------------------------------------------------ */

void direcaoDoOlhar(float *dx, float *dy, float *dz)
{
    float ry = yaw   * PI / 180.0f;
    float rp = pitch * PI / 180.0f;

    *dx =  sinf(ry) * cosf(rp);
    *dy =  sinf(rp);
    *dz = -cosf(ry) * cosf(rp);
}

/* ------------------------------------------------------------------ */
/* Personagem, desenhado só em terceira pessoa, como modelo           */
/* hierárquico. O corpo é levado aos pés do jogador e girado por      */
/* -yaw, para a frente local (-z) coincidir com a do olhar. Cada      */
/* braço e perna é desenhado pendurado abaixo da origem, girado ali e */
/* só então levado à articulação, para girar em torno do ombro ou do  */
/* quadril.                                                           */
/*                                                                    */
/* O balanço é BALANCO_MAXIMO x amplitude x sen(fase); a fase avança  */
/* com a distância andada (ver atualiza). Tudo fica a menos de 0.3 do */
/* eixo vertical, dentro da caixa de colisão em qualquer giro.        */
/* ------------------------------------------------------------------ */

/* Uma peça do boneco: caixa com o canto mínimo em (x, y, z) do sistema
   local atual e as medidas dadas */
static void caixa(int tipo, float x, float y, float z,
                  float largura, float altura, float profundidade)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(largura, altura, profundidade);
    desenhaBloco(tipo, 1.0f);
    glPopMatrix();
}

/* Braço ou perna pendurado na articulação (x, y), girado 'angulo' graus
   em torno do eixo x (positivo: para a frente). A parte de cima tem a
   cor tipoCima; os últimos alturaBaixo, a cor tipoBaixo. */
static void membro(float x, float y, float angulo, float largura,
                   int tipoCima, int tipoBaixo, float alturaBaixo)
{
    float meiaLargura = largura / 2.0f;
    float meiaProf = PROFUNDIDADE_CORPO / 2.0f;
    float alturaCima = COMPRIMENTO_MEMBRO - alturaBaixo;

    glPushMatrix();
    glTranslatef(x, y, 0.0f);                   /* 3. até a articulação */
    glRotatef(angulo, 1.0f, 0.0f, 0.0f);        /* 2. gira em torno dela */
    caixa(tipoCima,  -meiaLargura, -alturaCima, -meiaProf,       /* 1. pendurado */
          largura, alturaCima, PROFUNDIDADE_CORPO);
    caixa(tipoBaixo, -meiaLargura, -COMPRIMENTO_MEMBRO, -meiaProf,
          largura, alturaBaixo, PROFUNDIDADE_CORPO);
    glPopMatrix();
}

void desenhaJogador(void)
{
    float quadril = COMPRIMENTO_MEMBRO;                 /* altura das articulações */
    float ombro   = 2.0f * COMPRIMENTO_MEMBRO;
    float meiaProf = PROFUNDIDADE_CORPO / 2.0f;
    float meiaCabeca = LADO_CABECA / 2.0f;
    float xPerna = LARGURA_PERNA / 2.0f;                /* centro de cada perna */
    float xBraco = (LARGURA_TRONCO + LARGURA_BRACO) / 2.0f;
    float angulo = BALANCO_MAXIMO * amplitudeCaminhada * sinf(faseCaminhada);

    glPushMatrix();
    glTranslatef(jogadorX, jogadorY, jogadorZ);         /* pés, centro da caixa */
    glRotatef(-yaw, 0.0f, 1.0f, 0.0f);                  /* frente do boneco = -z local */

    /* pernas (x negativo é a esquerda, x positivo a direita) */
    membro(-xPerna, quadril,  angulo, LARGURA_PERNA, CALCA, SAPATO, 0.1f);
    membro( xPerna, quadril, -angulo, LARGURA_PERNA, CALCA, SAPATO, 0.1f);

    /* tronco */
    caixa(CAMISA, -LARGURA_TRONCO / 2.0f, quadril, -meiaProf,
          LARGURA_TRONCO, COMPRIMENTO_MEMBRO, PROFUNDIDADE_CORPO);

    /* braços: manga curta e mão, em sentido oposto à perna do mesmo lado */
    membro(-xBraco, ombro, -angulo, LARGURA_BRACO, CAMISA, PELE, 0.45f);
    membro( xBraco, ombro,  angulo, LARGURA_BRACO, CAMISA, PELE, 0.45f);

    /* cabeça, de ombro até ombro + LADO_CABECA (perto de ALTURA_JOGADOR) */
    caixa(PELE, -meiaCabeca, ombro, -meiaCabeca,
          LADO_CABECA, LADO_CABECA, LADO_CABECA);

    /* cabelo: casca 0.01 maior dos lados, em cima e atrás, da altura dos
       olhos para cima; a frente dela fica dentro da cabeça, atrás do rosto */
    caixa(CABELO, -meiaCabeca - 0.01f, ombro + 0.15f, -meiaCabeca + 0.01f,
          LADO_CABECA + 0.02f, LADO_CABECA - 0.14f, LADO_CABECA);

    /* franja: faixa de cabelo no alto da testa */
    caixa(CABELO, -meiaCabeca, ombro + LADO_CABECA - 0.05f, -meiaCabeca - 0.01f,
          LADO_CABECA, 0.05f, 0.01f);

    /* olhos, na altura ALTURA_OLHOS: branco por fora, íris por dentro */
    caixa(OLHO_BRANCO, -0.14f, ALTURA_OLHOS - 0.03f, -meiaCabeca - 0.01f,
          0.1f, 0.05f, 0.01f);
    caixa(OLHO_BRANCO,  0.04f, ALTURA_OLHOS - 0.03f, -meiaCabeca - 0.01f,
          0.1f, 0.05f, 0.01f);
    caixa(OLHO, -0.09f, ALTURA_OLHOS - 0.03f, -meiaCabeca - 0.015f,
          0.05f, 0.05f, 0.01f);
    caixa(OLHO,  0.04f, ALTURA_OLHOS - 0.03f, -meiaCabeca - 0.015f,
          0.05f, 0.05f, 0.01f);

    glPopMatrix();
}

/* ------------------------------------------------------------------ */
/* Câmera: gluLookAt(olho, alvo, cima) com os mesmos dados nos dois   */
/* modos. Primeira pessoa: olho na altura dos olhos, alvo logo à      */
/* frente na direção do olhar. Terceira pessoa: olho DISTANCIA_CAMERA */
/* atrás, só no plano do chão, e ELEVACAO_CAMERA acima, para a câmera */
/* nunca entrar no chão; o alvo é a cabeça, deslocada na vertical     */
/* pelo pitch.                                                        */
/* ------------------------------------------------------------------ */

void posicionaCamera(void)
{
    float dx, dy, dz;
    float cabecaX = jogadorX;
    float cabecaY = jogadorY + ALTURA_OLHOS;
    float cabecaZ = jogadorZ;

    direcaoDoOlhar(&dx, &dy, &dz);

    if (modoCamera == PRIMEIRA_PESSOA) {
        gluLookAt(cabecaX, cabecaY, cabecaZ,
                  cabecaX + dx, cabecaY + dy, cabecaZ + dz,
                  0.0, 1.0, 0.0);
    } else {
        float ry = yaw * PI / 180.0f;
        float frenteX = sinf(ry), frenteZ = -cosf(ry);  /* só o plano do chão */

        gluLookAt(cabecaX - frenteX * DISTANCIA_CAMERA,  /* atrás */
                  cabecaY + ELEVACAO_CAMERA,             /* acima, altura fixa */
                  cabecaZ - frenteZ * DISTANCIA_CAMERA,
                  cabecaX,                               /* olha o personagem, */
                  cabecaY + dy * DISTANCIA_CAMERA,       /* inclinado pelo pitch */
                  cabecaZ,
                  0.0, 1.0, 0.0);
    }
}
