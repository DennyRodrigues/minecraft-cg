# Roteiro de Apresentação — Semana 5 (revisado)

Oct 21, 2026 · @Denny

Roteiro de Computação Gráfica sobre controle por teclado e mouse, movimentação e colisão, alinhado ao plano de aula e ao código do projeto.

## Como usar este roteiro

Cada bloco tem uma tensão central (o defeito que motiva o conteúdo), perguntas para a turma, a explicação que resolve a tensão e ao menos um exemplo concreto. Use como trilha, não palavra por palavra. Os tempos são estimativas: o que importa é fechar o conteúdo de cada bloco, na ordem.

| Marcador | Significado |
| --- | --- |
| \[SLIDE S#\] | Trocar para o slide indicado (conteúdo no Mapa de slides). Vale até o próximo marcador |
| \[VÍDEO\] | Reproduzir clipe pré-gravado no slide |
| \[AO VIVO\] | Demonstrar no executável em tempo real |

Os clipes usam a tag `<video>` nativa do HTML: o arquivo .mp4 fica ao lado do index.html e é referenciado por caminho relativo, sem dependências. Há um único clipe, o V1, narrado, com cerca de 60 segundos.

Regra de distribuição: os Blocos 1 e 2 usam só slides (e o vídeo da abertura), sem código ao vivo. Os Blocos 3, 4 e 5 têm slides de código, com trechos reais do projeto (arquivo e linhas no cabeçalho do trecho), e demonstração ao vivo. Os problemas da versão 1 aparecem uma única vez, no vídeo da abertura, e não voltam a ser apontados nos blocos seguintes.

As demonstrações ao vivo usam as versões de demonstração do projeto, escolhidas na compilação (`make run CHAVE=valor`). O título da janela mostra qual versão está rodando, e `make run` sem chaves volta à versão final.

Antes de começar, retome a semana anterior: a pergunta de verificação sobre o vetor Up do gluLookAt volta no Bloco 2 como defeito concreto.

## Mapa de vídeos

Há um único vídeo, na abertura do Bloco 1, com a versão 1 do projeto e todos os problemas de uma vez. Nenhum outro bloco usa vídeo.

| # | Momento | O que mostrar | Duração |
| --- | --- | --- | --- |
| V1 | Bloco 1, abertura | Versão 1 do projeto (commit `5c4bbd3`) com narração por cima, mostrando os cinco problemas: hesitação, velocidade, sem diagonal, atravessa tudo, só primeira pessoa | \~60s |

## Mapa de slides

São 39 slides. Os de pergunta têm fundo escuro e só a pergunta na tela; a resposta vem no slide seguinte. A coluna da direita é o mínimo que o slide deve conter: texto curto na tela, explicação por conta de quem apresenta. Os slides falam só com a turma: instruções para quem apresenta (o que demonstrar, quando, que comando rodar) ficam neste roteiro e não aparecem na tela. Nos slides de código, os comandos `make run` aparecem como "versão de demonstração", para a turma poder reproduzir.

| # | Momento | Tipo | Slide | Deve conter |
| --- | --- | --- | --- | --- |
| S1 | Antes de começar | Capa | Capa | Título da aula, disciplina e semana |
| S2 | Bloco 1, abertura | Vídeo | Vídeo da versão 1 | O vídeo V1 em tela cheia, sem texto sobreposto |
| S3 | Bloco 1, abertura | Pergunta | Quais têm a mesma causa? | A pergunta e os cinco problemas em uma linha |
| S4 | Bloco 1, abertura | Conteúdo | Três defeitos, mesma origem | Tabela com os cinco problemas, a origem de cada um (leitura da entrada ou funcionalidade nova) e o bloco que o resolve |
| S5 | Bloco 1, §1.1 | Conteúdo | Caminho da entrada | Quatro caixas ligadas por setas: usuário aperta W, fila do SO, `glutMainLoop` esvazia a fila, função registrada é chamada. A pergunta do `while(true)` e o rótulo "inversão de controle" |
| S6 | Bloco 1, §1.2 | Conteúdo | Evento contra estado | Duas colunas (por evento, por estado) com um exemplo cada (ESC; andar com W), a tabela de formas de leitura e o trecho real do vetor `teclas[]` |
| S7 | Bloco 1, §1.3 | Conteúdo | Por que a hesitação acontece | Repetição do SO pensada para digitação; o estado resolve; `glutIgnoreKeyRepeat` e `glutKeyboardUpFunc`; a diagonal como dois valores 1 no vetor |
| S8 | Bloco 1, §1.4 | Pergunta | 0,1 por quadro em 120 e 30 FPS | A pergunta |
| S9 | Bloco 1, §1.4 | Conteúdo | Delta time | As duas fórmulas e o exemplo de 60 contra 20 FPS com velocidade 5: 0,083 e 0,25 por quadro, 5 unidades em 1 s |
| S10 | Bloco 2, abertura | Pergunta | Bloco 2: o que calcular a partir dos pixels? | Rótulo do bloco e a pergunta |
| S11 | Bloco 2, §2.1 e §2.3 | Conteúdo | Coordenadas de tela e o eixo Y | Desenho das coordenadas de tela (y para baixo) contra as normalizadas (y para cima), `glutMotionFunc` e `glutPassiveMotionFunc`, e a inversão do sinal no pitch |
| S12 | Bloco 2, §2.1 | Pergunta | O cursor que foge | As duas perguntas: o que acontece ao arrastar sem parar, e como resolver sem prender o cursor no SO |
| S13 | Bloco 2, §2.1 | Conteúdo | Captura virtual | Os quatro passos, o exemplo com números (centro em (512, 384), mouse em (522, 384), dx = 10, yaw + 1°) e o aviso da tecla de liberação (TAB) |
| S14 | Bloco 2, §2.2 | Conteúdo | Yaw e pitch | Vista de cima com o arco do yaw, vista de lado com o arco do pitch e as três equações do vetor de direção |
| S15 | Bloco 2, §2.2 | Pergunta | E depois de 90°? | A pergunta, com a lembrança da pergunta de verificação da semana anterior |
| S16 | Bloco 2, §2.2 | Conteúdo | Visada paralela ao Up | Produto vetorial nulo, inversão da cena e a regra do limite (−89° a 89°) |
| S17 | Bloco 2, §2.5 | Consulta | Tabela de referência da entrada | A tabela de funções da seção 2.5, para ficar aberta durante a prática |
| S18 | Bloco 3, experimento | Código | Bloco 3: experimento antes de codar | Trecho real de `USAR_DT` e `temporizador()`, os comandos `make run` com 16 e 50 ms e a pergunta "mais devagar ou mais rápido?" |
| S19 | Bloco 3, passo 1 | Código | Vetor de estado | Trecho real de `teclado()`, `tecladoSolto()` e dos registros em `main()`; por que `tolower` |
| S20 | Bloco 3, passo 2 | Código | Direção, diagonal e delta time | Dois trechos reais: direção e normalização em `atualiza()`, e o cálculo do dt com o registro do idle ou do timer |
| S21 | Bloco 4, abertura | Pergunta | Bloco 4: câmera voadora ou personagem? | Rótulo do bloco, a pergunta e o gancho da matriz do mundo |
| S22 | Bloco 4, §4.1 | Conteúdo | Detecção e resposta | Detecção e resposta lado a lado e "testar antes de mover" |
| S23 | Bloco 4, §4.2 | Pergunta | Por que uma caixa alinhada aos eixos? | A pergunta |
| S24 | Bloco 4, §4.2 | Conteúdo | AABB | As três desigualdades do teste AABB e como ele vira consulta às células da matriz |
| S25 | Bloco 4, §4.3 e Etapa 1 | Código | Consulta ao mundo e ponto central | Trechos reais de `blocoSolido()` e da versão 1 de `posicaoLivre()`, o comando `make run VERSAO_COLISAO=1` e a pergunta "o que está faltando?" |
| S26 | Bloco 4, Etapa 2 | Código | Largura e altura | Trecho real da caixa em `posicaoLivre()`, o comando da versão 2 e o desenho da caixa na fronteira entre dois blocos, com as células que ela cobre |
| S27 | Bloco 4, Etapa 3 | Pergunta | Diagonal contra a parede | A pergunta |
| S28 | Bloco 4, Etapa 3 | Código | Deslize eixo a eixo | Desenho de cima: parede, personagem e a seta `(dx, dz)` decomposta em X e Z; trecho real de `movePedaco()`; comandos das versões 3 (trava) e 4 (desliza) |
| S29 | Bloco 4, §4.4 | Pergunta | dt muito grande | A pergunta |
| S30 | Bloco 4, §4.4 | Código | Tunelamento | Trechos reais do limite do dt e de `moveJogador()`, que divide o deslocamento; a ressalva da câmera lenta |
| S31 | Bloco 5, abertura | Pergunta | Bloco 5: o que muda ao conectar teclado e mouse? | Rótulo do bloco e a pergunta |
| S32 | Bloco 5, antes do código | Conteúdo | A câmera: `gluLookAt` | Tabela olho, alvo e up com a pergunta que cada um responde, a ordem em cada quadro e a ordem de implementação ao vivo |
| S33 | Bloco 5, §5.1 | Código | O mouse | Trechos reais de `movimentoMouse()`, `capturaCursor()`, `liberaCursor()` e dos registros em `main()` |
| S34 | Bloco 5, §5.2 | Código | Primeira pessoa | Trechos reais de `direcaoDoOlhar()` e do ramo de primeira pessoa de `posicionaCamera()`; o aviso do avanço no plano horizontal |
| S35 | Bloco 5, §5.3 | Conteúdo | Primeira e terceira pessoa | Dois desenhos de cima lado a lado: primeira pessoa (olho no personagem, alvo na ponta da seta) e terceira (olho atrás, alvo no personagem), com a linha tracejada olho-alvo. A pergunta "o que muda entre os modos?" |
| S36 | Bloco 5, §5.3 e §5.4 | Código | Terceira pessoa | Trechos reais do ramo de terceira pessoa de `posicionaCamera()` e do início de `desenhaJogador()`; os momentos ao vivo de alternar o modo e de remover o limite do pitch |
| S37 | Bloco 5, §5.5 | Código | Zoom por FOV | A pergunta "zoom ou andar?", trechos reais de `mudaZoom()`, `rodaMouse()` e da linha do `gluPerspective` |
| S38 | Bloco 5, §5.6 | Consulta | Resumo dos controles | A tabela de controles da seção 5.6 |
| S39 | Encerramento | Conteúdo | O que construímos | Os três resultados da aula e as cinco perguntas finais |

## Antes de começar

**\[SLIDE S1\]** Capa. Retome a pergunta de verificação da semana anterior sobre o vetor Up do `gluLookAt`: ela volta no Bloco 2.

## Bloco 1: Fundamentos da entrada interativa

Só slides, sem código ao vivo. A aula parte da versão 1 do projeto: os problemas aparecem logo no início, em vídeo, junto com o que vamos construir. O bloco explica a origem dos três primeiros (hesitação, velocidade e diagonal): a forma como a entrada foi lida.

### Abertura: a versão 1 e o que vamos fazer

**\[SLIDE S2\]** **\[VÍDEO V1\]** Reproduza a versão 1 do projeto com narração por cima, apontando cada problema enquanto ele acontece:

1. Hesitação: ao segurar a tecla, o personagem dá um passo, pausa cerca de 500 ms e só então anda.
2. Velocidade que varia: o mesmo código anda em ritmos diferentes conforme a máquina.
3. Sem diagonal: ao segurar duas teclas (W e D), só uma vale.
4. Atravessa tudo: colunas, troncos, paredes e o próprio chão.
5. Só existe primeira pessoa: o personagem nunca aparece.

Comente os problemas aqui, uma única vez, sem voltar a apontá-los nos blocos seguintes.

**\[SLIDE S3\]** **Pergunta:** "Dos problemas que vocês acabaram de ver, quais têm a mesma causa?"

**\[SLIDE S4\]** Hesitação, velocidade e diagonal: os três vêm de como a entrada foi lida. Colisão e terceira pessoa são funcionalidades novas, que só funcionam bem depois que a leitura estiver certa. O slide também diz o que a aula entrega e onde cada item é resolvido:

| Problema | Origem | Onde resolvemos |
| --- | --- | --- |
| Hesitação | Leitura da entrada | Blocos 1 e 3 |
| Velocidade que varia | Leitura da entrada | Blocos 1 e 3 |
| Sem diagonal | Leitura da entrada | Bloco 3 |
| Atravessa tudo | Funcionalidade nova: colisão | Bloco 4 |
| Só primeira pessoa | Funcionalidade nova: terceira pessoa | Bloco 5 |

O experimento com `glutTimerFunc` fica para o Bloco 3.

"Três defeitos, mesma origem. Vamos entender o porquê."

### 1.1 Como a entrada chega à aplicação

**\[SLIDE S5\]** Mostre o caminho da entrada em quatro caixas ligadas por setas: "usuário aperta W", "fila do SO", "glutMainLoop esvazia a fila", "função registrada pela aplicação é chamada". A entrada não chega no instante em que o usuário age: o sistema operacional a acumula em uma fila.

**Pergunta:** "Quem escreve o `while(true)` do jogo de vocês?" Resposta esperada: "a gente". Corrija:

"Na GLUT, não. Quem escreve o laço principal é o `glutMainLoop`. A aplicação registra funções e reage a eventos. Isso é inversão de controle: o framework chama o seu código, não o contrário."

### 1.2 Evento e estado: a distinção que explica tudo

**\[SLIDE S6\]** Dois cenários lado a lado:

| Por evento | Por estado |
| --- | --- |
| A tecla é pressionada e dispara uma função, uma vez | A cada quadro, verifica-se se a tecla está ativa naquele instante |

**Pergunta:** "Se eu quero abrir um menu ao pressionar ESC, qual modelo uso?" Evento. (No projeto, o ESC abre justamente o menu de pausa.)

**Pergunta:** "E se eu quero andar enquanto seguro W?" Pausa. Deixe a turma pensar. Estado, porque o movimento precisa durar, não acontecer uma vez.

| Forma de leitura | Adequada para | Exemplos na disciplina |
| --- | --- | --- |
| Por evento | Ações discretas, uma vez por toque | Alternar 1ª e 3ª pessoa, alternar projeção, abrir o menu, liberar o cursor |
| Por estado | Ações contínuas, enquanto a tecla estiver pressionada | Deslocamento, giro contínuo, aceleração (correr com R) |

O problema: a GLUT só oferece leitura por evento. Não existe uma função que pergunte se a tecla está pressionada. O estado precisa ser construído pela aplicação. No projeto (`src/entrada.c`, linhas 88, 115 e 434):

```c
/* em teclado(): o evento de pressionar liga */
teclas[tolower(tecla)] = 1;             /* estado: para as ações contínuas */

/* em tecladoSolto(): o evento de soltar desliga */
teclas[tolower(tecla)] = 0;

/* em atualiza(), a cada quadro: age enquanto estiver ativo */
float frente = (float)(teclas['w'] - teclas['s']);
```

"O vetor `teclas[]` é uma memória dos eventos passados. Estado é sempre memória."

### 1.3 Por que a hesitação acontece

**\[SLIDE S7\]** A causa da hesitação vista na abertura:

**Pergunta:** "Quando vocês seguraram uma tecla no editor de texto e a letra repetiu, houve uma pausa antes de começar a repetir, lembram?" Todo mundo lembra.

"O atraso inicial e a cadência da repetição são definidos pelo sistema operacional, pensados para digitação de texto. Se o movimento vive dentro do callback de teclado, ele herda esse atraso e essa cadência."

O que resolve a hesitação é o estado: com `teclas[]` guiando o movimento a cada quadro, o evento de repetição não acrescenta informação (só reescreve `teclas[k] = 1`). Por isso ele é desativado com `glutIgnoreKeyRepeat(1)`, como limpeza, e a soltura da tecla é registrada com `glutKeyboardUpFunc`, que é quem zera o estado.

A diagonal também sai daí: duas teclas ativas são só dois valores 1 no vetor. Pela repetição do SO isso não funciona, porque em geral ela vale só para a última tecla pressionada.

### 1.4 Por que a velocidade varia entre máquinas

**\[SLIDE S8\]** **Pergunta:** "Se eu somar 0,1 ao X do personagem a cada quadro, o que acontece em uma máquina de 120 FPS comparada a uma de 30 FPS?" Anda 4 vezes mais rápido.

**\[SLIDE S9\]** "O movimento está amarrado à taxa de quadros. A correção é multiplicar a velocidade pelo tempo que o quadro levou, o *delta time*."

```latex
\text{deslocamento} = \text{velocidade} \times dt
```

```latex
dt = \frac{t_{\text{atual}} - t_{\text{anterior}}}{1000}
```

Exemplo com números, para a velocidade do projeto, 5 unidades por segundo: a 60 FPS (dt ≈ 0,017 s) o personagem anda 0,083 unidade por quadro; a 20 FPS (dt = 0,05 s) anda 0,25 por quadro. Os passos são diferentes, mas em 1 segundo os dois somam 5 unidades. Quadros menos frequentes dão passos maiores, e o resultado por segundo não muda.

O tempo vem de `glutGet(GLUT_ELAPSED_TIME)`, que informa milissegundos desde o início do programa. A atualização acontece em `glutIdleFunc` e termina com `glutPostRedisplay`.

"Agora não importa se a máquina é um notebook de 2010 ou uma workstation: o personagem anda na mesma velocidade."

## Bloco 2: Mouse e orientação da câmera

Só slides. Este bloco liga a entrada do mouse ao modelo de câmera da semana anterior. O código do callback do mouse aparece no Bloco 5.

### Abertura: o que o mouse precisa fazer

**\[SLIDE S10\]** **Pergunta:** "O mouse devolve x e y em pixels. O que precisamos calcular a partir disso para girar a câmera?" Não é a posição, é o deslocamento.

### 2.1 Coordenadas de tela e 2.3 o eixo Y do mouse

**\[SLIDE S11\]** A posição do cursor vem em coordenadas de tela: origem no canto superior esquerdo, eixo vertical crescendo para baixo. É o contrário das coordenadas normalizadas da semana 2. Registre o contraste, porque a inversão do eixo vertical é fonte recorrente de visada invertida.

**Pergunta:** "O mouse reporta Y crescendo para baixo. O mundo OpenGL tem Y crescendo para cima. O que isso causa se não tratarmos?" Visada invertida verticalmente: mover o mouse para cima olha para baixo. Correção: inverter o sinal do deslocamento em Y ao atualizar o pitch.

A GLUT separa dois eventos de movimento: `glutMotionFunc`, com botão pressionado, e `glutPassiveMotionFunc`, sem botão. Para uma câmera que acompanha o mouse livremente, registre as duas. Se só uma estiver registrada, a visada congela enquanto um botão estiver pressionado.

### 2.1 O cursor que foge da janela

**\[SLIDE S12\]** **Pergunta:** "Se o usuário arrasta o mouse para a direita sem parar, o que acontece com o cursor depois de uns 3 segundos?" Sai da janela e o movimento para.

**Pergunta:** "Como resolver sem prender o cursor no SO, algo que a GLUT não suporta?"

**\[SLIDE S13\]** Explique a captura virtual:

1. Esconder o cursor com `glutSetCursor(GLUT_CURSOR_NONE)`.
2. A cada movimento, calcular o deslocamento em relação ao centro da janela, não à última posição.
3. Devolver o cursor ao centro com `glutWarpPointer(cx, cy)`.
4. O evento gerado pelo próprio reposicionamento resulta em deslocamento nulo e não interfere no giro.

Exemplo com números, na janela do projeto (1024 por 768, centro em (512, 384)): o mouse vai para (522, 384), o deslocamento é dx = 10 e, com sensibilidade de 0,1° por pixel, o yaw aumenta 1°. O cursor é devolvido a (512, 384). O evento gerado por esse reposicionamento chega com deslocamento 0 e é descartado. Como a referência é sempre o centro, o cursor nunca chega à borda da janela.

Atenção: a captura precisa de uma tecla de liberação (no projeto, TAB), que captura e libera o cursor. Sem ela, o usuário não consegue sair da janela.

### 2.2 Yaw e pitch: dois ângulos, um olhar

**\[SLIDE S14\]** Mostre o personagem visto de cima, com um arco de yaw, e visto de lado, com um arco de pitch. Os termos em inglês são os de uso corrente na área.

"Mouse horizontal altera o yaw (giro em torno do eixo vertical). Mouse vertical altera o pitch (inclinação para cima ou para baixo)." O vetor de direção sai dos dois ângulos e é passado ao `gluLookAt` como ponto observado. Na convenção do projeto (ângulos em graus, yaw 0 olhando para −z):

```latex
\begin{aligned} dir_x &= \sin(\text{yaw})\,\cos(\text{pitch}) \\ dir_y &= \sin(\text{pitch}) \\ dir_z &= -\cos(\text{yaw})\,\cos(\text{pitch}) \end{aligned}
```

Como ler a fórmula: o pitch reparte a direção entre vertical e horizontal. A parte vertical é `sin(pitch)`. A parte horizontal tem comprimento `cos(pitch)` e é dividida entre X e Z pelo yaw. Com pitch 0 sobra só o círculo horizontal `(sin yaw, −cos yaw)`; com pitch 90° a parte horizontal some e a direção vira `(0, 1, 0)`, igual ao Up. O vetor sempre tem comprimento 1.

**\[SLIDE S15\]** **Pergunta:** "O que acontece quando o pitch passa de 90°?" Pausa. Deixe pensarem.

**\[SLIDE S16\]** "A direção de visada se torna paralela ao vetor Up (0, 1, 0) que passamos ao `gluLookAt`. No valor exato, o produto vetorial entre os dois é zero e não há referência de orientação. Ao ultrapassar, a câmera passa por cima do zênite, o Up continua apontando para cima e a cena inverte de cabeça para baixo de uma vez." Este é o problema teórico da pergunta de verificação da semana anterior.

Solução: limitar o pitch a pouco menos de 90°, entre −89° e 89°. A demonstração ao vivo, removendo o limite, fica para o Bloco 5 (§5.4), onde o mouse já está funcionando.

### 2.5 Tabela de referência da entrada

**\[SLIDE S17\]** Deixe esta tabela aberta durante a prática.

| Operação | Chamada em C | Efeito |
| --- | --- | --- |
| Pressionar tecla | `glutKeyboardFunc(funcao)` | Teclas de caractere; recebe a tecla e a posição do cursor |
| Soltar tecla | `glutKeyboardUpFunc(funcao)` | Necessária para construir o estado das teclas |
| Teclas especiais | `glutSpecialFunc` e `glutSpecialUpFunc` | Setas e teclas de função, pressionar e soltar |
| Repetição | `glutIgnoreKeyRepeat(1)` | Desativa a repetição automática de tecla |
| Botões do mouse | `glutMouseFunc(funcao)` | Chamada a cada clique ou soltura |
| Movimento do mouse | `glutMotionFunc` e `glutPassiveMotionFunc` | Com e sem botão pressionado, respectivamente |
| Roda do mouse | `glutMouseWheelFunc(funcao)` | Extensão da freeglut, em `GL/freeglut.h` |
| Ocultar cursor | `glutSetCursor(GLUT_CURSOR_NONE)` | Torna o cursor invisível |
| Reposicionar cursor | `glutWarpPointer(x, y)` | Devolve o cursor ao centro, simulando a captura |
| Atualização | `glutIdleFunc(funcao)` | Chamada quando não há eventos; local da atualização do movimento |
| Tempo decorrido | `glutGet(GLUT_ELAPSED_TIME)` | Milissegundos desde o início; base do tempo de quadro |

## Bloco 3: Teclado por estado e delta time

Código em slide e ao vivo. O resultado visível: o cenário percorrido pelo teclado com movimento imediato, contínuo, sem hesitação inicial e na mesma velocidade em qualquer taxa de quadros.

### Experimento antes de codar

**\[SLIDE S18\]** **\[AO VIVO\]** Rode a versão com deslocamento fixo por quadro (`USAR_DT=0`) e troque a função ociosa por `glutTimerFunc`, primeiro com 16 ms e depois com 50 ms:

```bash
make run USAR_DT=0 EXPERIMENTO_TIMER=16
make run USAR_DT=0 EXPERIMENTO_TIMER=50
```

Atenção: `glutTimerFunc` dispara uma única vez, então o callback precisa se registrar de novo, senão o movimento para no primeiro passo. No projeto (`src/entrada.c`, linhas 417 a 423 e 472 a 477):

```c
#if USAR_DT
    float passo = VELOCIDADE * dt;
    float giro  = VELOCIDADE_GIRO * dt;
#else
    float passo = PASSO_POR_QUADRO;         /* depende da taxa de quadros */
    float giro  = GIRO_POR_QUADRO;
#endif

void temporizador(int valor)
{
    (void)valor;
    atualiza();
    glutTimerFunc(EXPERIMENTO_TIMER, temporizador, 0);
}
```

**Pergunta:** "A câmera anda mais devagar ou mais rápido com 50 ms?" Mais devagar: menos quadros por segundo, menos chamadas ao callback.

"Isso confirma que o problema está na leitura, não no desenho. Agora vamos corrigir."

### Passo 1: vetor de estado

**\[SLIDE S19\]** No projeto (`src/entrada.c`, linhas 84 a 116, resumido, e `src/main.c`, linhas 202, 207 e 208):

```c
void teclado(unsigned char tecla, int x, int y)
{
    teclas[tolower(tecla)] = 1;             /* estado: para as ações contínuas */
    /* ... menu de pausa aberto: ver menu.c ... */
    switch (tolower(tecla)) {               /* evento: para as ações discretas */
        case 27:  abreMenu(); break;        /* ESC: menu de pausa */
        case 9:   alternaCaptura(); break;  /* 9 é o código da tecla Tab */
        case 'p':
            projecaoOrtogonal = !projecaoOrtogonal;
            aplicaProjecao();
            break;
        /* ... V (câmera), + e - (zoom) ... */
    }
}

void tecladoSolto(unsigned char tecla, int x, int y)
{
    teclas[tolower(tecla)] = 0;
}

/* em main: */
glutIgnoreKeyRepeat(1);
glutKeyboardFunc(teclado);              /* tecla comum pressionada */
glutKeyboardUpFunc(tecladoSolto);       /* tecla comum solta */
```

Por que `tolower`: com Caps Lock ligado a tecla chega como `'W'`, que não bate com `teclas['w']`. E se o Shift muda entre pressionar e soltar, a soltura chega com outro caractere e o movimento ficaria preso. Normalizar nos dois lados evita os dois problemas. Demonstre com Caps Lock, não com Shift: no projeto, o Shift faz descer.

**Pergunta:** "Por que ESC e P ficam no callback de evento e não no laço de estado?" Porque acontecem uma vez por toque. Não devem ser repetidos 60 vezes por segundo.

### Passo 2: direção, diagonal e delta time

**\[SLIDE S20\]** O movimento usa a direção do olhar projetada no plano horizontal, só a partir do yaw: frente = `(sen, −cos)`, com yaw 0 olhando para −z, e direita = a frente girada 90°, `(cos, sen)`, que é frente × Up. W − S e D − A valem 1, 0 ou −1, então duas teclas juntas viram diagonal. Na diagonal a soma mede √2; normalizar mantém a mesma velocidade em qualquer direção. No projeto (`src/entrada.c`, linhas 429 a 443, 453 e 454):

```c
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

float antesX = jogadorX, antesZ = jogadorZ;
moveJogador(mx * passo, subir * passo, mz * passo);
```

O tempo de quadro e o registro da atualização, no mesmo slide (`src/entrada.c`, linhas 405 a 410 e 466, resumido, e `src/main.c`, linhas 217 a 221):

```c
void atualiza(void)
{
    int agora = glutGet(GLUT_ELAPSED_TIME);
    float dt = (agora - ultimoTempo) / 1000.0f;
    ultimoTempo = agora;
    if (dt > DT_MAXIMO) dt = DT_MAXIMO;
    /* ... passo = VELOCIDADE * dt; teclas; moveJogador ... */
    glutPostRedisplay();
}

/* em main: */
#if EXPERIMENTO_TIMER
    glutTimerFunc(EXPERIMENTO_TIMER, temporizador, 0);  /* máquina simulada */
#else
    glutIdleFunc(atualiza);                 /* nada acontecendo: atualiza */
#endif
```

`ultimoTempo` é inicializado em `main()`, para o primeiro dt não ser o tempo desde o início do programa.

**\[AO VIVO\]** Repita o experimento com 16 ms e 50 ms, agora com o tempo de quadro. A velocidade é idêntica: o que muda é só a suavidade.

```bash
make run EXPERIMENTO_TIMER=16
make run EXPERIMENTO_TIMER=50
```

O que a turma deve ver ao final: arranque imediato, movimento contínuo, diagonal na mesma velocidade, mesma velocidade nas duas taxas, e uma tecla de ação discreta (ESC, P) respondendo uma única vez.

## Bloco 4: Colisão com o mundo

Slides e código. Até aqui o observador atravessa colunas, troncos e o próprio chão. A colisão é o que transforma a câmera voadora em personagem. Resultado visível: o personagem percorre o mundo sem atravessar paredes e desliza ao longo delas quando anda em diagonal contra uma superfície.

### Abertura: o que é colisão, de verdade

**\[SLIDE S21\]** **Pergunta:** "O que transforma uma câmera voadora em um personagem com corpo?" Colisão. É ela que dá peso e presença ao personagem.

Gancho com o projeto: a matriz do mundo que os grupos construíram deixa de ser só organização de dados. A colisão passa a ser uma consulta a ela.

### 4.1 Dois problemas distintos: detecção e resposta

**\[SLIDE S22\]** Detecção e resposta, lado a lado:

| Detecção | Resposta |
| --- | --- |
| "Tem sobreposição?" | "O que fazer?" |

"Hoje a resposta é a mais simples: impedir o movimento. A separação importa porque amanhã a resposta pode ser ricochete, dano ou deslize." O teste é feito antes do movimento: calcula-se a posição pretendida, verifica-se se está livre e o deslocamento só é aplicado em caso positivo.

### 4.2 AABB: a caixa mais simples que funciona

**\[SLIDE S23\]** **Pergunta:** "Por que representar o personagem como uma caixa alinhada aos eixos, e não como uma esfera ou o modelo 3D real?"

**\[SLIDE S24\]** Caixas alinhadas aos eixos não giram, então o yaw não altera a caixa, e o teste se resume a comparar intervalos. Modelos reais exigem decomposição em triângulos.

Há colisão se, e somente se, os intervalos se sobrepõem nos três eixos ao mesmo tempo:

```latex
\begin{aligned} |x_a - x_b| &< (larg_a + larg_b)/2 \\ |y_a - y_b| &< (alt_a + alt_b)/2 \\ |z_a - z_b| &< (prof_a + prof_b)/2 \end{aligned}
```

Em um mundo de blocos não é preciso comparar o personagem com cada bloco. As coordenadas do personagem, arredondadas para baixo, já são os índices da matriz, e basta consultar as poucas posições que a caixa ocupa. No projeto, em cada eixo a caixa cobre as células de `floor(min)` a `floor(max)`; testar todas elas é o teste de intervalos feito contra a grade, com no máximo 2 × 3 × 2 = 12 consultas.

### 4.3 Progressão de implementação: cada passo corrige um defeito visível

**\[SLIDE S25\]** Primeiro, a consulta ao mundo. Posições fora dos limites contam como sólidas, para manter o personagem dentro do mundo: paredes nas bordas, piso embaixo e teto em cima. "Existe bloco" e "bloqueia" são perguntas diferentes: a água é desenhada, mas não é sólida. No projeto (`src/colisao.c`, linhas 52 a 60):

```c
int blocoSolido(int x, int y, int z)
{
    int tipo;

    if (!dentroDoMundo(x, y, z)) return 1;  /* fora do mundo: parede */

    tipo = mundo[x][y][z];
    return tipo != VAZIO && tipo != AGUA;
}
```

Quem chama converte as coordenadas com `floorf`, não com `(int)x`: o cast para `int` trunca em direção a zero e erra para coordenadas negativas. No projeto o personagem voa (Espaço sobe, Shift desce), então a colisão também testa o eixo Y.

**Etapa 1: só o ponto central.** No mesmo slide (`src/colisao.c`, linhas 128 a 132):

```c
int posicaoLivre(float x, float y, float z)
{
#if VERSAO_COLISAO == 1
    /* só o ponto central dos pés */
    return !blocoSolido((int)floorf(x), (int)floorf(y), (int)floorf(z));
```

**\[AO VIVO\]** `make run VERSAO_COLISAO=1`, em terceira pessoa: o personagem entra pela metade nos blocos antes de parar.

**Pergunta:** "O que está faltando?" A largura da caixa. O ponto central passa, mas as bordas já estão dentro do bloco.

**Etapa 2: largura e altura.**

**\[SLIDE S26\]** Mostre, em vista de cima, uma grade de blocos, um bloco sólido e o personagem como um quadrado de lado 0,6 (`LARGURA_JOGADOR`), na fronteira entre dois blocos: ele cobre duas células, por isso testar só o centro não basta. No mesmo slide, o personagem de lado: com altura 1,8 ele cobre de duas a três células na vertical. No projeto (`src/colisao.c`, linhas 126 a 147, resumido):

```c
#define FOLGA 0.001f    /* encostar não é sobrepor */

    float meia = LARGURA_JOGADOR / 2.0f;
    int x0 = (int)floorf(x - meia), x1 = (int)floorf(x + meia - FOLGA);
    int y0 = (int)floorf(y),        y1 = (int)floorf(y + ALTURA_JOGADOR - FOLGA);
    int z0 = (int)floorf(z - meia), z1 = (int)floorf(z + meia - FOLGA);
    int cx, cy, cz;

    for (cx = x0; cx <= x1; cx++)
        for (cy = y0; cy <= y1; cy++)
            for (cz = z0; cz <= z1; cz++)
                if (blocoSolido(cx, cy, cz))
                    return 0;               /* a caixa toca um bloco sólido */
    return 1;
```

A `FOLGA` existe porque encostar não é sobrepor: com o personagem exatamente na face de um bloco, `floor(5.0)` diria que ele ocupa a célula 5 e ele ficaria preso.

**\[AO VIVO\]** `make run VERSAO_COLISAO=2`: o personagem para rente à parede.

**Etapa 3: testar eixo a eixo (o deslize).**

**\[SLIDE S27\]** **Pergunta:** "Se eu testo o deslocamento (dx, dz) como um só e a posição final colide, o que acontece ao andar em diagonal contra uma parede?"

**\[SLIDE S28\]** O personagem para completamente, mesmo que o eixo paralelo à parede estivesse livre. "A solução: aplicar X primeiro, depois Z, cada um com seu próprio teste. Se um eixo bloqueou mas o outro está livre, o personagem desliza ao longo da parede."

Mostre, em vista de cima, uma parede e o personagem encostado nela, com o deslocamento `(dx, dz)` como uma seta diagonal decomposta em dois segmentos: o que bate na parede e o que corre paralelo a ela. No teste conjunto, o segmento bloqueado barra a seta inteira; no teste eixo a eixo, só ele é descartado e o outro continua. No projeto (`src/colisao.c`, linhas 211 a 224):

```c
static void movePedaco(float dx, float dy, float dz)
{
#if VERSAO_COLISAO >= 4
    if (posicaoLivre(jogadorX + dx, jogadorY, jogadorZ)) jogadorX += dx;
    if (posicaoLivre(jogadorX, jogadorY, jogadorZ + dz)) jogadorZ += dz;
    if (posicaoLivre(jogadorX, jogadorY + dy, jogadorZ)) jogadorY += dy;
#else
    if (posicaoLivre(jogadorX + dx, jogadorY + dy, jogadorZ + dz)) {
        jogadorX += dx;
        jogadorY += dy;
        jogadorZ += dz;
    }
#endif
}
```

Cada teste parte do resultado do eixo anterior.

**\[AO VIVO\]** `make run VERSAO_COLISAO=3` (eixos juntos: trava na diagonal) e depois `make run VERSAO_COLISAO=4` (eixo a eixo: desliza).

### 4.4 Tunelamento: o limite do delta time

**\[SLIDE S29\]** **Pergunta:** "Se o `dt` for muito grande, o deslocamento por quadro pode ser maior que um bloco. O que acontece?"

**\[SLIDE S30\]** O personagem atravessa paredes finas sem acionar a colisão: tunelamento. O teste só vê a posição final, não o caminho. O projeto faz duas coisas (`src/entrada.c`, linha 410, e `src/colisao.c`, linhas 226 a 236):

```c
if (dt > DT_MAXIMO) dt = DT_MAXIMO;         /* DT_MAXIMO = 0.1 s */

void moveJogador(float dx, float dy, float dz)
{
    float comprimento = sqrtf(dx * dx + dy * dy + dz * dz);
    int pedacos = (int)ceilf(comprimento / PASSO_MAXIMO);
    int i;

    if (pedacos < 1) pedacos = 1;           /* parado: um pedaço nulo */

    for (i = 0; i < pedacos; i++)
        movePedaco(dx / pedacos, dy / pedacos, dz / pedacos);
}
```

- Limitar o dt evita que um travamento da janela vire um salto. Faça a ressalva em voz alta, porque isso contradiz o Bloco 1: abaixo de 10 FPS o jogo passa a andar em câmera lenta, em vez de manter a velocidade. É uma troca consciente de precisão de tempo por correção da colisão.
- Dividir o deslocamento em pedaços de no máximo `PASSO_MAXIMO` (0,5 bloco) é a alternativa que preserva a velocidade: nenhum teste salta mais que meio bloco, nem correndo.

## Bloco 5: Câmera em primeira e terceira pessoa

Código em slide e ao vivo, bloco de síntese: teclado e mouse passam a operar juntos sobre a câmera. Resultado visível: o cenário percorrido pelo personagem, com teclado para deslocamento, mouse para a direção do olhar, uma tecla que alterna entre primeira e terceira pessoa, a roda para aproximação e uma tecla para capturar e liberar o cursor.

### Abertura: síntese de tudo que veio antes

**\[SLIDE S31\]** **Pergunta:** "Teclado e mouse já funcionam separadamente. O que muda ao conectá-los?" Nada no movimento. Só os argumentos do `gluLookAt` mudam.

### Antes de codar: o que é a câmera

**\[SLIDE S32\]** No OpenGL clássico não existe um objeto câmera. "Câmera" é a matriz de visão: a cada quadro, `gluLookAt` recalcula a transformação que coloca o mundo diante de um observador. O mundo fica parado; o que muda, no código, são três argumentos. Pense em um fotógrafo:

| Argumento | O que é | Pergunta que responde |
| --- | --- | --- |
| Olho (`eye`) | Posição da câmera | Onde está o fotógrafo? |
| Alvo (`center`) | Ponto para o qual ela olha | Para onde ele aponta? |
| Up | Vetor que indica onde é "cima" | Qual lado da foto é o céu? Aqui sempre `(0, 1, 0)` |

O alvo é um ponto, não uma direção. A direção do olhar é alvo menos olho.

A ordem em cada quadro importa:

1. `glLoadIdentity()` na matriz MODELVIEW, apagando a câmera do quadro anterior.
2. `gluLookAt(...)` com os valores deste quadro.
3. Desenhar o mundo.
4. Em terceira pessoa, desenhar o personagem.

O que for desenhado antes do `gluLookAt` não passa pela câmera nova.

Ordem de implementação ao vivo: primeira pessoa só com yaw (verifique que o avanço acompanha a nova direção), depois o pitch com seu limite, depois a captura do cursor e, por último, a terceira pessoa.

### 5.1 O mouse: captura, yaw, pitch e limite

**\[SLIDE S33\]** Este é o código do Bloco 2 no projeto (`src/entrada.c`, linhas 210 a 234):

```c
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
```

E a captura (`src/entrada.c`, linhas 244 a 255, e `src/main.c`, linhas 211 e 212):

```c
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

/* em main: as duas, senão a visada congela */
glutPassiveMotionFunc(movimentoMouse);  /* mouse movido sem botão */
glutMotionFunc(movimentoMouse);         /* mouse movido com botão */
```

O deslocamento é sempre calculado em relação ao centro da janela, então o evento provocado pelo `glutWarpPointer` chega com deslocamento nulo e não gira nada. O `pitch -= dy` é a inversão do eixo Y da seção 2.3. `recentrar` evita um tranco: logo depois de capturar, o cursor pode estar longe do centro, e o primeiro movimento só o leva até lá. O TAB (código 9) chama `alternaCaptura()` no `teclado()`, junto com ESC e P, porque é uma ação discreta. O jogo já começa com o cursor capturado.

### 5.2 Primeira pessoa: câmera nos olhos do personagem

**\[SLIDE S34\]** No projeto (`src/camera.c`, linhas 186 a 194 e 427 a 436):

```c
void direcaoDoOlhar(float *dx, float *dy, float *dz)
{
    float ry = yaw   * PI / 180.0f;
    float rp = pitch * PI / 180.0f;

    *dx =  sinf(ry) * cosf(rp);
    *dy =  sinf(rp);
    *dz = -cosf(ry) * cosf(rp);
}

/* em posicionaCamera(): */
float cabecaX = jogadorX;
float cabecaY = jogadorY + ALTURA_OLHOS;
float cabecaZ = jogadorZ;

direcaoDoOlhar(&dx, &dy, &dz);

if (modoCamera == PRIMEIRA_PESSOA) {
    gluLookAt(cabecaX, cabecaY, cabecaZ,
              cabecaX + dx, cabecaY + dy, cabecaZ + dz,
              0.0, 1.0, 0.0);
```

Leitura do código: o olho fica na altura dos olhos (`jogadorY + ALTURA_OLHOS`, 1,6 acima dos pés), não nos pés. `dx`, `dy`, `dz` é a direção do olhar com comprimento 1, então somar ao olho põe o alvo 1 unidade à frente. A distância do alvo não importa, só o sentido. Com yaw 0 e pitch 0 o personagem olha para −z.

Atenção: o avanço usa a direção do olhar projetada no plano horizontal (`frenteX`, `frenteZ` de `atualiza()`, sem componente Y). Se usasse o vetor completo, olhar para cima faria o personagem subir voando. O olhar usa pitch; o deslocamento não.

### 5.3 Terceira pessoa: o personagem como alvo

**\[SLIDE S35\]** Mostre o personagem visto de cima, como um círculo com uma seta saindo dele (a direção do olhar, o yaw), em duas versões lado a lado:

- Primeira pessoa: o olho dentro do círculo e o alvo na ponta da seta.
- Terceira pessoa: o olho atrás do círculo, na linha da seta em sentido oposto, e o alvo no próprio círculo.

Em ambas, olho e alvo são ligados por uma linha tracejada. Ela aponta para o mesmo lado nos dois desenhos: o que muda é quem é olho e quem é alvo.

**Pergunta:** "Nos dois modos o personagem anda e olha para o mesmo lado. O que muda nos dados de posição e movimento?" Nada. Yaw, pitch, velocidade e colisão são os mesmos. Só a posição do olho e do alvo muda. E o personagem precisa ser desenhado, o que não ocorre em primeira pessoa, porque ele ocuparia o próprio ponto de vista.

**\[SLIDE S36\]** No projeto (`src/camera.c`, linhas 437 a 448 e 328 a 330):

```c
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

/* em desenhaJogador(): */
glPushMatrix();
glTranslatef(jogadorX, jogadorY, jogadorZ);         /* pés, centro da caixa */
glRotatef(-yaw, 0.0f, 1.0f, 0.0f);                  /* frente do boneco = -z local */
```

Leitura do código, linha a linha:

- Olho em X e Z: parte da cabeça e volta `DISTANCIA_CAMERA` (4 blocos) no sentido contrário ao olhar. É o mesmo vetor horizontal do avanço, com o sinal trocado.
- Olho em Y: sobe `ELEVACAO_CAMERA` (1,5) acima dos olhos, para ver o personagem de cima e por cima de obstáculos baixos.
- Alvo: a cabeça, deslocada na vertical por `dy * DISTANCIA_CAMERA`. Com pitch 0 o alvo é a própria cabeça; olhar para cima ou para baixo só inclina a vista.
- `glPushMatrix` e `glPopMatrix` (no fim de `desenhaJogador()`): a translação e a rotação valem só para o personagem. Sem eles, afetariam tudo que fosse desenhado depois.
- `glRotatef(-yaw, ...)`: o personagem gira junto com o olhar, então a câmera, que está sempre atrás da direção do olhar, o vê de costas. O sinal é negativo porque, nessa convenção, girar `-yaw` leva a frente do boneco (−z local) para a direção do olhar.

O recuo usa só o plano horizontal de propósito. Com o vetor de direção completo, a câmera desceria ao olhar para cima e chegaria a entrar no chão. O custo: em terceira pessoa o pitch não afeta a posição da câmera, só o alvo. A câmera orbital fica como pergunta final.

**\[AO VIVO\]** Alterne os modos com V (ou F5) enquanto anda. O personagem não se move; só o ponto de vista muda.

### 5.4 Remova o limite do pitch, ao vivo

**\[AO VIVO\]** Ainda no slide S36: "Vou remover o limite do pitch. Olhem o que acontece ao inclinar direto para cima."

```bash
make run LIMITAR_PITCH=0
```

A cena inverte de uma vez e a turma reage. "Esse é o problema teórico do Bloco 2 aparecendo como defeito concreto. Recoloquem o limite." Volte com `make run`. Esta é a única vez que o defeito é mostrado; no Bloco 2 ele é só explicado nos slides.

### 5.5 Zoom por FOV: mover a câmera ou trocar a lente

**\[SLIDE S37\]** **Pergunta:** "Qual a diferença entre aproximar o zoom e andar em direção ao objeto?" Discuta brevemente. Andar muda a perspectiva; o zoom por FOV só estreita o campo de visão, efeito de luneta. É uma decisão de visualização, não de posição.

A roda do mouse altera o FOV da projeção perspectiva via `glutMouseWheelFunc`, extensão da freeglut que exige incluir `GL/freeglut.h`. No projeto (`src/entrada.c`, linhas 319 a 333, e `src/camera.c`, linha 110):

```c
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

/* em aplicaProjecao(): */
gluPerspective(campoDeVisao, aspecto, 0.1, 200.0);
```

Passos de 5°, de 20° a 90°, com a lente refeita na hora. As teclas + e − chamam a mesma `mudaZoom()`.

"Não é mover a câmera, é estreitar o ângulo de visão. Como uma luneta."

### 5.6 Resumo dos controles

**\[SLIDE S38\]** Mostre a tabela de controles do projeto.

| Entrada | Ação | Tipo de leitura |
| --- | --- | --- |
| W, A, S, D | Deslocamento (frente, esquerda, trás, direita) | Estado |
| R (segurando) | Correr: dobra a velocidade | Estado |
| Espaço ou ↑ / Shift ou ↓ | Subir / descer | Estado |
| ← / → | Girar o olhar | Estado |
| Mouse | Direção do olhar (yaw e pitch) | Deslocamento relativo ao centro |
| V ou F5 | Alterna terceira (início) e primeira pessoa | Evento |
| TAB / clique | Libera ou captura o cursor | Evento |
| P | Alterna perspectiva e ortográfica | Evento |
| Roda ou + / − | Zoom por FOV | Evento |
| ESC | Abre e fecha o menu de pausa | Evento |
| Q (com o menu aberto) | Encerra | Evento |

## Encerramento: atualização do projeto

**\[SLIDE S39\]** O slide mostra o que a aula construiu (movimentação contínua, colisão, câmera em primeira e terceira pessoa) e as perguntas finais.

"O grupo responsável pela apresentação desta semana atualiza o repositório do jogo com o código desenvolvido na aula: movimentação contínua por estado com delta time, colisão com o mundo (consulta à matriz, eixo a eixo) e câmera em primeira e terceira pessoa. Demonstrem a evolução em sala na próxima aula."

Perguntas finais para reflexão:

- Se o personagem puder pular, a colisão em Y precisa de tratamento separado?
- O que acontece com a colisão se o bloco tiver formato não cúbico, como uma rampa?
- Como adaptar a terceira pessoa para uma câmera que orbita o personagem com o mouse, usando o pitch?
- Em terceira pessoa a câmera atravessa paredes. Como evitar, e o que muda na detecção?
- Se o `dt` limitado deixa o jogo lento em máquinas fracas, quando vale dividir em subpassos em vez de limitar?

## Referências

Links do plano de aula e do roteiro original, para consulta durante a prática. Não têm slide próprio.

| Tópico | Link |
| --- | --- |
| Teclado avançado na GLUT, com `glutIgnoreKeyRepeat` e soltura de tecla (inglês) | [lighthouse3d: advanced keyboard features](https://www.lighthouse3d.com/tutorials/glut-tutorial/advanced-keyboard-features/) |
| Movimento da câmera com teclas soltas e pressionadas na GLUT (inglês) | [lighthouse3d: keyboard example, moving around the world II](https://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world-ii/) |
| Mouse na GLUT, com movimento ativo e passivo (inglês) | [lighthouse3d: the mouse](https://www.lighthouse3d.com/tutorials/glut-tutorial/the-mouse/) |
| Especificação da GLUT, seção de `glutSetCursor` (inglês) | [opengl.org: GLUT spec 3, glutSetCursor](https://www.opengl.org/resources/libraries/glut/spec3/node28.html) |
| API da freeglut, funções de resposta e `glutGet` (inglês) | [freeglut: API](https://freeglut.sourceforge.net/docs/api.php) |
| Câmera virtual: ângulos, mouse e aproximação (inglês; outra biblioteca de janelas, mesmos conceitos) | [learnopengl: camera](https://learnopengl.com/Getting-started/Camera) |
| Colisão AABB 2D (português) | [MDN: detecção de colisão 2D](https://developer.mozilla.org/pt-BR/docs/Games/Techniques/2D_collision_detection) |
| Colisão AABB 3D (inglês) | [MDN: 3D collision detection](https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection) |
| Referência rápida das funções OpenGL | [docs.gl](https://docs.gl) |
