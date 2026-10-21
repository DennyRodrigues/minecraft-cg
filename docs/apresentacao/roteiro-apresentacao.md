# Roteiro de Apresentação — Semana 5
**Computação Gráfica · Controle por Teclado e Mouse, Movimentação e Colisão**
19h00 – 21h00 · 120 minutos

---

> **Como usar este roteiro**
> Cada bloco tem uma tensão central (o problema que motiva o conteúdo), perguntas para lançar à turma, a explicação que resolve a tensão e pelo menos um exemplo concreto ou experimento ao vivo. Não precisa seguir palavra por palavra — use como trilha.

---

## Bloco 1 — Fundamentos da Entrada Interativa
`19h00 – 19h25 · 25 min · Quadro e diagrama, sem código`

### Abertura: mostrar o defeito antes de explicar qualquer coisa

Abra o executável da semana anterior. Segure 'W' por alguns segundos.

> **Pergunta para a turma:**
> "O que vocês estão vendo de errado aqui?"

Deixe responderem. Eles vão citar a hesitação. Se não citarem a velocidade variável, abra em uma segunda máquina ou simule com `glutTimerFunc` em 16ms vs 50ms.

> "Dois defeitos, mesma origem. Vamos entender o porquê."

---

### 1.1 Como a entrada chega à aplicação

**Desenhe no quadro:** uma linha do tempo horizontal com "usuário aperta W" → "fila do SO" → "glutMainLoop esvazia a fila" → "função da aplicação é chamada".

> **Pergunta:**
> "Quem escreve o `while(true)` do jogo de vocês?"

Resposta esperada: "a gente". Corrija:

> "Na GLUT, não. Quem escreve o laço principal é o `glutMainLoop`. A aplicação apenas registra funções e reage a eventos. Isso é inversão de controle — o framework chama o seu código, não o contrário."

---

### 1.2 Evento vs Estado — a distinção que explica tudo

**Desenhe dois cenários lado a lado no quadro:**

```
POR EVENTO                    POR ESTADO
──────────────────────        ──────────────────────
tecla pressionada → dispara   todo quadro → verifica
uma função uma vez            se a tecla está ativa
```

> **Pergunta:**
> "Se eu quero abrir um menu ao pressionar ESC, qual modelo uso?"

Evento. Simples.

> "E se eu quero andar enquanto seguro W?"

Pausa. Deixe a turma pensar.

> "Estado — porque o movimento precisa durar, não acontecer uma vez."

**O problema:** a GLUT não tem uma função `glutEstaPresssionada('W')`. Ela só avisa quando algo acontece. Então o estado precisa ser construído pela aplicação.

**Mostre o modelo mental (quadro):**

```c
// pressionar → liga
teclas['w'] = 1;

// soltar → desliga
teclas['w'] = 0;

// a cada quadro → age enquanto estiver ativo
if (teclas['w']) mover_frente(dt);
```

> "O vetor `teclas[]` é uma memória dos eventos passados. Estado é sempre memória."

---

### 1.3 Por que a hesitação acontece — o mistério resolvido

Voltando ao defeito da abertura:

> **Pergunta:**
> "Quando vocês seguraram 'W' no editor de texto e a letra repetiu... teve aquela pausa antes de começar a repetir, lembram?"

Sim. Todo mundo lembra.

> "O SO faz isso de propósito — para não duplicar letras ao digitar rápido. Ele impõe um atraso de ~500ms antes de começar a disparar eventos repetidos. Se o movimento viver dentro do callback de teclado, ele herda esse atraso."

Solução: `glutIgnoreKeyRepeat(1)` — desativa a repetição — e usar o vetor de estado para guiar o movimento no `glutIdleFunc`.

---

### 1.4 Por que a velocidade varia entre máquinas

> **Pergunta:**
> "Se você somar 0,1 ao X do personagem a cada quadro, o que acontece em uma máquina que roda 120 FPS comparado a uma de 30 FPS?"

Resposta: "anda 4× mais rápido".

> "O movimento está amarrado à taxa de quadros. A correção é multiplicar a velocidade pelo tempo que o quadro levou — o *delta time*."

**Fórmula no quadro:**

```
deslocamento = velocidade × dt
dt = (tempo_atual − tempo_anterior) / 1000.0
```

> "Agora não importa se a máquina é um notebook de 2010 ou uma workstation. O personagem anda a mesma velocidade."

---

## Bloco 2 — Mouse e Orientação da Câmera
`19h25 – 19h50 · 25 min · Quadro e diagrama`

### Abertura: o que o mouse precisa fazer

> **Pergunta:**
> "O mouse devolve `x` e `y` em pixels. O que precisamos calcular a partir disso para girar a câmera?"

Não é a posição — é o **deslocamento** desde o último quadro.

---

### 2.1 O problema do cursor que foge da janela

> **Pergunta:**
> "Se o usuário arrasta o mouse para a direita sem parar para girar a câmera, o que acontece com o cursor depois de uns 3 segundos?"

Sai da janela. O movimento para.

> "Como resolver sem prender o cursor no SO — algo que a GLUT não suporta diretamente?"

Explique a **captura virtual**:

```
1. Esconder o cursor: glutSetCursor(GLUT_CURSOR_NONE)
2. A cada movimento, calcular o deslocamento em relação ao centro
3. Devolver o cursor ao centro: glutWarpPointer(cx, cy)
4. O evento gerado pelo reposicionamento resulta em deslocamento nulo → ignorado
```

**Atenção:** sempre oferecer uma tecla de liberação (ex: `TAB`). Sem ela, o usuário fica preso na janela.

---

### 2.2 Yaw e Pitch — dois ângulos, um olhar

**Desenhe no quadro:** personagem visto de cima, com um arco de yaw; personagem visto de lado, com um arco de pitch.

> "Mouse horizontal → yaw. Mouse vertical → pitch."

**Mostre a conversão para vetor:**

```
dir.x = cos(yaw) × cos(pitch)
dir.y = sin(pitch)
dir.z = sin(yaw) × cos(pitch)
```

> **Pergunta:**
> "O que acontece quando o pitch chega a exatamente 90°?"

Pausa. Deixe pensarem.

> "O vetor de direção fica paralelo ao vetor `Up (0,1,0)` que passamos ao `gluLookAt`. O produto vetorial entre eles dá zero — sem referência de orientação. A cena inverte de cabeça para baixo instantaneamente."

**Solução simples:** `pitch = clamp(pitch, -89°, 89°)`. Demonstre ao vivo removendo o clamp e olhando direto para cima.

---

### 2.3 Coordenadas de tela vs coordenadas do mundo

> **Pergunta rápida:**
> "O mouse reporta Y crescendo para baixo (origem no canto superior esquerdo). O mundo OpenGL tem Y crescendo para cima. O que isso causa se não tratarmos?"

Visada invertida verticalmente — mover o mouse para cima olha para baixo.

Correção: inverter o sinal do delta Y ao atualizar o pitch.

---

### 2.4 Zoom por FOV — mover a câmera vs trocar a lente

> **Pergunta:**
> "Qual a diferença entre aproximar o zoom e andar em direção ao objeto?"

Discuta brevemente. Mover altera perspectiva, zoom por FOV apenas estreita o campo de visão — efeito de "luneta".

> "A roda do mouse altera o FOV da projeção perspectiva via `glutMouseWheelFunc`. É uma decisão de visualização, não de posição."

---

## Bloco 3 — Teclado por Estado e Delta Time (Prática)
`19h50 – 20h05 · 15 min · Código ao vivo`

### Experimento antes de codar

Antes de escrever qualquer linha, substitua `glutIdleFunc` por `glutTimerFunc` com 16ms e depois com 50ms.

> **Pergunta:**
> "A câmera anda mais devagar ou mais rápido com 50ms?"

Mais devagar — menos quadros por segundo, menos chamadas ao callback.

> "Isso confirma que o problema está na leitura, não no desenho. Agora vamos corrigir."

---

### Sequência de implementação (ao vivo)

**Passo 1 — Vetor de estado:**

```c
static int teclas[256] = {0};

void ao_pressionar(unsigned char k, int x, int y) {
    teclas[k] = 1;
    // ações discretas ainda aqui:
    if (k == 27) exit(0);          // ESC encerra
    if (k == 'p') alternar_proj(); // P alterna projeção
}

void ao_soltar(unsigned char k, int x, int y) {
    teclas[k] = 0;
}
```

> **Pergunta para a turma:** "Por que ações como ESC e P ficam no callback de evento e não no loop de estado?"

Porque acontecem uma vez por toque — não devem ser repetidas 60× por segundo.

**Passo 2 — Delta time e movimento:**

```c
void ao_ficar_ocioso(void) {
    static int t_ant = 0;
    int t_atual = glutGet(GLUT_ELAPSED_TIME);
    if (t_ant == 0) { t_ant = t_atual; return; }

    float dt = (t_atual - t_ant) / 1000.0f;
    t_ant = t_atual;

    if (teclas['w']) mover_frente(dt);
    if (teclas['s']) mover_tras(dt);
    if (teclas['a']) mover_esquerda(dt);
    if (teclas['d']) mover_direita(dt);

    glutPostRedisplay();
}
```

Repita o experimento com 16ms e 50ms. A velocidade agora é idêntica.

---

## Bloco 4 — Colisão com o Mundo
`20h05 – 20h25 · 20 min · Quadro + código`

### Abertura: o que é colisão, de verdade

> **Pergunta:**
> "O que transforma uma câmera voadora em um personagem que anda pelo chão?"

Colisão. É ela que dá peso e presença ao personagem.

---

### 4.1 Dois problemas distintos: detecção e resposta

**No quadro:**

```
DETECÇÃO          →   RESPOSTA
"tem sobreposição?"    "o que fazer?"
```

> "Hoje a resposta é a mais simples: impedir o movimento. Mas a separação é importante — amanhã a resposta pode ser ricochete, dano, ou deslize."

---

### 4.2 AABB — a caixa mais simples que funciona

> **Pergunta:**
> "Por que representar o personagem como uma caixa alinhada aos eixos e não como uma esfera ou o modelo 3D real?"

Caixas têm colisão O(1) — basta comparar intervalos em X, Y e Z. Modelos reais exigem decomposição em triângulos.

**No quadro — teste AABB entre dois blocos:**

```
Colisão sse:
  |x_a − x_b| < (larg_a + larg_b) / 2
  |y_a − y_b| < (alt_a  + alt_b)  / 2
  |z_a − z_b| < (prof_a + prof_b) / 2
```

> "Em um mundo de blocos, as coordenadas do personagem arredondadas para baixo já são os índices da matriz. Não é preciso checar todos os blocos do mundo — só os que o personagem pode estar tocando."

---

### 4.3 Progressão de implementação — cada passo corrige um defeito visível

**Etapa 1 — Só o ponto central:**

```c
int colide(float x, float y, float z) {
    return mundo[(int)x][(int)y][(int)z] != 0;
}
```

Mostre ao vivo: o personagem entra pela metade nos blocos antes de parar.

> **Pergunta:** "O que está faltando?"

A **largura** da caixa. O ponto central passa, mas as bordas colidem.

**Etapa 2 — Considerar a largura:**

```c
// Checar os 4 cantos horizontais da caixa
colide(x + r, y, z + r) || colide(x - r, y, z + r) || ...
```

**Etapa 3 — Testar eixo a eixo (o deslize):**

> **Pergunta:**
> "Se eu testo X e Z juntos e os dois colidem, o que acontece ao andar em diagonal contra uma parede?"

O personagem para completamente — mesmo que Z estivesse livre.

> "A solução: testar e aplicar X primeiro, depois Z separadamente. Se X bloqueou mas Z está livre, o personagem desliza ao longo da parede."

```c
// Testa X
pos_nova.x = pos.x + dx;
if (!colide_caixa(pos_nova.x, pos.y, pos.z))
    pos.x = pos_nova.x;

// Testa Z separadamente
pos_nova.z = pos.z + dz;
if (!colide_caixa(pos.x, pos.y, pos_nova.z))
    pos.z = pos_nova.z;
```

---

### 4.4 Tunelamento — o limite do delta time

> **Pergunta:**
> "Se o dt for muito grande (quadro muito lento), o deslocamento por quadro pode ser maior que a largura de um bloco. O que acontece?"

O personagem atravessa paredes finas sem acionar a colisão — **tunelamento**.

> "Solução simples: limitar o deslocamento máximo por quadro. `dt = min(dt, 0.05f)` — não mais que 50ms por passo."

---

## Bloco 5 — Câmera em 1ª e 3ª Pessoa
`20h25 – 21h00 · 35 min · Código ao vivo`

### Abertura: síntese de tudo que veio antes

> **Pergunta:**
> "Teclado e mouse já funcionam separadamente. O que muda ao conectá-los?"

Nada no movimento. Só os argumentos do `gluLookAt` mudam.

---

### 5.1 Primeira pessoa — câmera nos olhos do personagem

```c
// Posição = onde o personagem está
// Alvo = ponto à frente, na direção do olhar
gluLookAt(
    pos.x, pos.y + altura_olho, pos.z,           // posição da câmera
    pos.x + dir.x, pos.y + altura_olho + dir.y, pos.z + dir.z, // alvo
    0, 1, 0                                       // up
);
```

> **Atenção:** o avanço deve seguir a direção projetada no plano horizontal (`dir.y = 0` ao andar). Caso contrário, olhar para cima faz o personagem subir voando.

---

### 5.2 Terceira pessoa — personagem como alvo

> **Pergunta:**
> "O que muda nos dados de posição e movimento ao alternar para terceira pessoa?"

**Nada.** Yaw, pitch, velocidade, colisão — tudo igual.

> "Muda apenas o `gluLookAt`: a câmera recua pela direção oposta ao olhar e sobe um pouco, e o alvo passa a ser o personagem."

```c
// Câmera recuada e elevada
float cam_x = pos.x - dir.x * distancia;
float cam_y = pos.y + altura_elevacao;
float cam_z = pos.z - dir.z * distancia;

gluLookAt(
    cam_x, cam_y, cam_z,
    pos.x, pos.y, pos.z, // observa o personagem
    0, 1, 0
);
// Desenhar o personagem (não aparece em 1ª pessoa)
desenhar_personagem(pos, yaw);
```

**Demonstre ao vivo:** alterne os modos enquanto anda. O personagem não se move — só o ponto de vista muda.

---

### 5.3 Remova o clamp do pitch — ao vivo

> "Vou remover o `clamp` do pitch. Olhem o que acontece ao inclinar direto para cima."

A cena inverte. Turma reage.

> "Esse é o problema teórico do Bloco 2 aparecendo como defeito concreto. Recoloquem o clamp."

---

### 5.4 Roda do mouse — zoom por FOV

```c
void ao_rolar_roda(int botao, int direcao, int x, int y) {
    fov -= direcao * 2.0f;
    fov = fmaxf(20.0f, fminf(120.0f, fov));
    // reaplicar na matriz de projeção no próximo quadro
}
```

> "Não é mover a câmera — é estreitar o ângulo de visão. Como uma luneta."

---

## Encerramento — Atualização do Projeto
`21h00`

> "O grupo responsável pela apresentação desta semana atualiza o repositório com: movimentação contínua por estado, colisão AABB eixo a eixo, câmera em primeira e terceira pessoa. Demonstrem na próxima aula."

**Perguntas finais para reflexão:**

- Se o personagem puder pular, a colisão em Y precisa de tratamento separado?
- O que acontece com a colisão se o bloco tiver formato não-cúbico (rampa)?
- Como adaptar a terceira pessoa para uma câmera que orbita o personagem com o mouse?

---

## Referências rápidas para consulta durante a prática

| Tópico | Link |
|---|---|
| Teclado avançado na GLUT | lighthouse3d.com/tutorials/glut-tutorial/advanced-keyboard-features |
| Mouse na GLUT | lighthouse3d.com/tutorials/glut-tutorial/the-mouse |
| Câmera virtual | learnopengl.com/Getting-started/Camera |
| Colisão AABB 2D (PT) | developer.mozilla.org/pt-BR/docs/Games/Techniques/2D_collision_detection |
| Colisão AABB 3D | developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection |
| Referência rápida OpenGL | docs.gl |
| API freeglut | freeglut.sourceforge.net/docs/api.php |

