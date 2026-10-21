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

/* ==================================================================
 * APLICAPROJECAO: monta a "lente" da câmera na matriz de projeção
 *
 * O problema
 *   A cena é 3D, a tela é 2D. Alguém precisa decidir como cada ponto do
 *   mundo cai num ponto da tela: o que é visível, o que fica maior ou
 *   menor, até onde se enxerga. Antes isso era fixo em redimensiona();
 *   agora três coisas mudam essa decisão durante o jogo: o tamanho da
 *   janela, a tecla P e a roda do mouse. Por isso virou uma função que
 *   todos chamam.
 *
 * A teoria
 *   O OpenGL do pipeline fixo usa duas matrizes. A de MODELO-VISUALIZAÇÃO
 *   (GL_MODELVIEW) diz onde as coisas estão em relação à câmera: é a
 *   posição da câmera. A de PROJEÇÃO (GL_PROJECTION) diz como o espaço à
 *   frente da câmera é achatado na tela: é a lente. Trocar a lente não
 *   move a câmera, e mover a câmera não troca a lente.
 *
 *   Na perspectiva, o volume visível é um tronco de pirâmide (em inglês,
 *   frustum) com o vértice no olho:
 *
 *            plano próximo        plano distante
 *                 |                     |
 *        olho  <  |                     |
 *               \ |       visível       |
 *        fov     >|                     |
 *               / |                     |
 *              <  |                     |
 *
 *   Um objeto a uma distância d aparece na tela com tamanho proporcional
 *   a 1 / d: por isso o que está longe fica pequeno. O campo de visão
 *   (fov) é a abertura vertical da pirâmide; a abertura horizontal sai
 *   da proporção largura / altura da janela. Se a proporção não entrasse
 *   na conta, a imagem seria esticada para ocupar a janela e o cubo
 *   viraria um paralelepípedo.
 *
 *   Na ortográfica, o volume visível é uma caixa: o tamanho na tela não
 *   depende da distância. É a vista de planta de um projeto, útil para
 *   medir, mas sem sensação de profundidade.
 *
 * Como o código produz o efeito
 *   glMatrixMode(GL_PROJECTION) e glLoadIdentity() começam uma lente do
 *   zero. gluPerspective recebe fov, proporção, plano próximo e plano
 *   distante; glOrtho recebe os seis lados da caixa, com a mesma
 *   proporção nos lados esquerdo e direito. No fim, a função volta para
 *   GL_MODELVIEW, porque é nela que desenha() e a cena trabalham.
 *
 *   O plano próximo é 0.1 nos dois modos. Na ortográfica ele poderia ser
 *   negativo, mas aí entrariam no volume blocos que estão atrás do olho,
 *   e, por estarem "mais perto", seriam desenhados por cima de tudo.
 *
 * Pergunta provável
 *   "Por que não usar plano próximo 0?"
 *   O buffer de profundidade não guarda a distância de forma linear: a
 *   precisão é muito maior perto do plano próximo e cai com a razão
 *   distante / próximo. Com o próximo em 0 essa razão vai ao infinito,
 *   não sobra precisão para as superfícies e faces vizinhas começam a
 *   disputar o mesmo pixel (o defeito chamado z-fighting). Na
 *   perspectiva, além disso, o próximo em 0 é uma divisão por zero.
 * ================================================================== */

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

/* ==================================================================
 * DIRECAODOOLHAR: vetor de direção a partir dos ângulos yaw e pitch
 *
 * O problema
 *   O mouse fornece dois ângulos, mas o gluLookAt não recebe ângulos:
 *   recebe um ponto para onde olhar. É preciso converter "girado tantos
 *   graus para o lado e inclinado tantos graus para cima" num vetor.
 *
 * A teoria
 *   A orientação é descrita por dois ângulos, com os nomes em inglês de
 *   uso corrente: yaw (guinada), o giro em torno do eixo vertical, e
 *   pitch (arfagem), a inclinação para cima ou para baixo. A conversão
 *   para vetor é a das coordenadas esféricas, feita em duas etapas.
 *
 *   1. O pitch divide o vetor de comprimento 1 entre "para cima" e
 *      "no chão" (vista de lado):
 *
 *                       . direção (comprimento 1)
 *                      /|
 *                     / | sen(pitch)   parte vertical
 *                    /  |
 *              olho o---+
 *                cos(pitch)            parte no chão
 *
 *   2. O yaw distribui a parte do chão entre x e z, exatamente como na
 *      semana 4 (vista de cima, yaw 0 olhando para -z):
 *
 *              -z
 *               ^   . (sen(yaw), -cos(yaw))
 *               |  /
 *               | / yaw
 *               o------> +x
 *
 *   Juntando: direção = ( sen(yaw)·cos(pitch),
 *                         sen(pitch),
 *                        -cos(yaw)·cos(pitch) )
 *
 *   O comprimento continua 1, porque
 *   cos²(pitch)·(sen²(yaw) + cos²(yaw)) + sen²(pitch) = cos² + sen² = 1.
 *
 * Como o código produz o efeito
 *   Os ângulos são guardados em graus (mais fáceis de ler) e convertidos
 *   para radianos, que é o que sinf e cosf esperam. As três linhas são a
 *   fórmula acima. Quem chama soma o vetor ao olho para obter o ponto
 *   observado do gluLookAt.
 *
 * Pergunta provável
 *   "Por que o cos(pitch) multiplica x e z?"
 *   Porque, quanto mais se olha para cima, menos do vetor sobra no chão.
 *   Olhando reto para cima (pitch 90), cos(pitch) = 0 e o vetor é só
 *   (0, 1, 0): o yaw deixa de importar.
 * ================================================================== */

void direcaoDoOlhar(float *dx, float *dy, float *dz)
{
    float ry = yaw   * PI / 180.0f;
    float rp = pitch * PI / 180.0f;

    *dx =  sinf(ry) * cosf(rp);
    *dy =  sinf(rp);
    *dz = -cosf(ry) * cosf(rp);
}

/* ==================================================================
 * DESENHAJOGADOR: o personagem, visível só em terceira pessoa
 *
 * O problema
 *   Em terceira pessoa a câmera sai da cabeça do personagem, e ele
 *   precisa aparecer na posição dele, virado para onde olha, com braços
 *   e pernas que balançam enquanto ele anda.
 *
 * A teoria
 *   1. Composição de transformações (semana 3). Na pilha de matrizes do
 *   OpenGL, a transformação escrita POR ÚLTIMO é a primeira a agir
 *   sobre os vértices. Para o boneco inteiro, lendo de baixo para cima:
 *
 *     peças (caixa, membro) montadas em volta da origem, pés em y = 0
 *     glRotatef(-yaw)       gira o boneco em torno do eixo vertical que
 *                           passa pelo centro dele
 *     glTranslatef(jogador) leva o boneco pronto até a posição no mundo
 *
 *   A ordem importa: girar ANTES de transladar gira o boneco em torno
 *   do próprio eixo; o contrário o faria orbitar a origem do mundo.
 *
 *   Por que -yaw: glRotatef(θ) em torno de y leva o vetor (0, 0, -1)
 *   para (-sen θ, 0, -cos θ). A frente do personagem é
 *   (sen(yaw), 0, -cos(yaw)), então θ precisa ser -yaw para a frente
 *   local do boneco (-z, onde está o rosto) coincidir com a dele.
 *
 *   2. Modelo hierárquico. O boneco é uma árvore de partes: o corpo
 *   inteiro gira com o yaw, e dentro dele cada braço e perna gira
 *   sozinho em torno da sua articulação. glPushMatrix/glPopMatrix
 *   delimitam cada ramo: o que acontece dentro de um membro (o giro
 *   dele) não afeta os outros, mas tudo herda o giro e a posição do
 *   corpo.
 *
 *   3. Girar em torno de um ponto que não é a origem. glRotatef sempre
 *   gira em torno da origem. Para a perna girar em torno do quadril, e
 *   não do centro dela, a perna é desenhada PENDURADA abaixo da origem
 *   (de y = 0 até y = -COMPRIMENTO_MEMBRO), girada ali, e só depois
 *   levada até o quadril:
 *
 *       glTranslatef(quadril)    3. leva a origem (com a perna) ao quadril
 *       glRotatef(angulo, x)     2. gira em torno da origem = do quadril
 *       caixa(... y = -0.7 ...)  1. perna pendurada abaixo da origem
 *
 *            o  quadril                 o  quadril
 *            |                           \
 *            |   perna parada             \   perna girada para a frente
 *            |                             \
 *
 *   Ângulo positivo em torno de x leva o ponto (0, -C, 0) para
 *   (0, -C·cos θ, -C·sen θ): z negativo, ou seja, para a FRENTE.
 *
 *   4. Animação da caminhada. O ângulo de cada membro é
 *
 *       BALANCO_MAXIMO x amplitude x sen(fase)
 *
 *   O seno faz o vaivém. A fase avança com a DISTÂNCIA andada, e não com
 *   o tempo (ver atualiza): o passo acompanha o deslocamento, correr
 *   balança mais rápido sozinho, e encostado numa parede, sem andar, o
 *   balanço não avança. A amplitude vai de 0 a 1 em poucos décimos de
 *   segundo quando ele começa a andar e volta a 0 quando para, para os
 *   membros não congelarem no meio do passo. Perna esquerda e braço
 *   direito usam +ângulo, os outros dois -ângulo: braço e perna do
 *   mesmo lado vão em sentidos opostos, como numa caminhada de verdade.
 *
 *   5. Por que o boneco cabe na caixa de colisão. A caixa não gira (ver
 *   POSICAOLIVRE), mas o desenho gira. Tudo o que é desenhado precisa
 *   ficar a menos de 0.3 (meia caixa) do eixo vertical, em qualquer
 *   ângulo, senão o boneco entra na parede. Tronco e braços juntos têm
 *   0.5 de largura e 0.2 de profundidade: os cantos ficam a
 *   raiz(0.25² + 0.1²) = 0.27 do eixo. A cabeça, de 0.38, tem cantos a
 *   0.19 x raiz(2) = 0.27. Só durante o balanço, mãos e pés passam
 *   alguns centímetros desse limite, o que não se nota.
 *
 * Como o código produz o efeito
 *   caixa() desenha uma peça: leva o cubo de lado 1 até o canto da peça
 *   e o estica com glScalef. membro() faz o esquema do item 3 para um
 *   braço ou uma perna de duas cores (manga e mão, calça e sapato).
 *   desenhaJogador() monta: pernas, tronco, braços, cabeça, cabelo
 *   (uma casca um pouco maior que a cabeça, que cobre topo, nuca e
 *   lados mas fica atrás do rosto), franja e olhos (caixas finas, 0.01
 *   à frente do rosto). glScalef com valores positivos não inverte o
 *   sentido dos vértices, então o GL_CULL_FACE continua correto.
 *
 * Pergunta provável
 *   "Por que a perna gira em torno do quadril, e não do centro dela?"
 *   Porque ela é desenhada com o topo na origem e só depois transladada
 *   até o quadril. O giro, que é sempre em torno da origem, acontece
 *   com o quadril na origem.
 * ================================================================== */

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

/* ==================================================================
 * POSICIONACAMERA: primeira e terceira pessoa com os mesmos dados
 *
 * O problema
 *   A mesma posição e a mesma orientação do personagem precisam virar
 *   duas vistas diferentes: dos olhos dele, ou de trás e de cima.
 *
 * A teoria
 *   gluLookAt(olho, alvo, cima) monta a MATRIZ DE VISUALIZAÇÃO. A
 *   câmera do OpenGL nunca sai da origem: o que gluLookAt faz é mover e
 *   girar o MUNDO inteiro ao contrário, até que o olho fique na origem
 *   e o alvo fique à frente dele. Por isso ela é chamada logo depois de
 *   glLoadIdentity e antes de desenhar qualquer coisa.
 *
 *   Os dois modos usam os mesmos dados (posição, yaw, pitch) e diferem
 *   só nos argumentos de gluLookAt:
 *
 *     primeira pessoa  olho na cabeça; alvo = cabeça + direção do olhar
 *     terceira pessoa  olho atrás e acima da cabeça; alvo = a cabeça
 *
 *   O modo de câmera é uma decisão de VISUALIZAÇÃO, não de lógica:
 *   nada no movimento muda, e por isso alternar o modo andando não muda
 *   a trajetória do personagem.
 *
 *   Na terceira pessoa o recuo é só HORIZONTAL, a uma altura fixa. Se a
 *   câmera recuasse ao longo da direção completa do olhar, ao olhar para
 *   cima ela iria para baixo da cabeça, e, com o pitch perto de 89,
 *   uns 3 blocos dentro do chão:
 *
 *       recuo pela direção do olhar        recuo horizontal (usado)
 *
 *                 / alvo                    câmera o
 *                /                                  \
 *       cabeça  o                            cabeça  o
 *       -------/------------ chão           ----------------- chão
 *             o câmera (enterrada)
 *
 *   O pitch entra só no alvo: ele sobe ou desce DISTANCIA x sen(pitch).
 *   Com pitch 0, o alvo é a própria cabeça. Como o vetor câmera -> alvo
 *   sempre tem 4 blocos na horizontal, ele nunca fica paralelo a "cima"
 *   e o gluLookAt não degenera (ver LIMITE DO PITCH).
 *
 * Como o código produz o efeito
 *   cabeca é a posição dos olhos. Em primeira pessoa, alvo = cabeça +
 *   direcaoDoOlhar(). Em terceira, a frente no chão sai só do yaw, o
 *   olho é cabeça - frente x DISTANCIA + ELEVACAO para cima, e o alvo é
 *   a cabeça deslocada verticalmente pelo pitch.
 *
 * Pergunta provável
 *   "Por que o personagem não é desenhado em primeira pessoa?"
 *   Porque ele ocupa o próprio ponto de vista: o olho estaria dentro do
 *   cubo do corpo, e as faces dele tapariam a tela.
 * ================================================================== */

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
