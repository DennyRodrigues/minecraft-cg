# Visual Design Guidelines • Apresentação de Computação Gráfica

Este documento estabelece as diretrizes visuais e técnicas para os slides da disciplina de **Computação Gráfica** (Ciência da Computação), com foco em sobriedade acadêmica, legibilidade técnica e rigor editorial.

---

## 1. Princípios de Design

1. **Editorial e Acadêmico:** Prioridade absoluta à clareza conceitual, precisão terminológica e hierarquia visual previsível.
2. **Sem Estética Genérica de IA:** Ficam estritamente proibidos efeitos de vidro (*glassmorphism*), sombras volumosas, gradientes multicoloridos, partículas decorativas, ilustrações 3D vazias e emojis em títulos ou marcadores.
3. **100% Baseado em Variáveis CSS:** Nenhuma cor, medida ou família tipográfica deve ser declarada de forma *hardcoded* nas regras de estilo. Toda alteração visual deve poder ser feita exclusivamente no bloco `:root`.
4. **Proporção da Paleta:**
   - **70%** Fundo claro (Branco quente `#F4F1EA`).
   - **20%** Grafite (`#20272D`) para texto principal, capas e transições.
   - **10%** Destaques técnicos com Azul-petróleo (`#176B70`) e Terracota (`#D28B51`).

---

## 2. Paleta de Cores e Variáveis

Todas as cores da apresentação são definidas pelas variáveis abaixo:

```css
:root {
  /* Cores Base das Diretrizes */
  --color-graphite: #20272D;     /* Cor escura principal / títulos / capas */
  --color-background: #F4F1EA;   /* Branco quente / fundo principal de conteúdo */
  --color-primary: #176B70;      /* Azul-petróleo / destaque principal / divisores */
  --color-accent: #D28B51;       /* Terracota / destaque secundário / alertas */
  --color-secondary: #68747B;    /* Cinza azulado / texto auxiliar / legendas */

  /* Cores Derivadas e Semânticas */
  --color-text-main: var(--color-graphite);
  --color-text-muted: var(--color-secondary);
  --color-text-light: var(--color-background);
  --color-text-dim: rgba(104, 116, 123, 0.7);

  --color-surface-light: #ECE7DD;
  --color-surface-dark: #2A333B;
  --color-border-subtle: rgba(32, 39, 45, 0.12);
  --color-border-strong: rgba(32, 39, 45, 0.25);
  --color-border-light: rgba(244, 241, 234, 0.18);

  --color-code-bg: #1B2227;
  --color-code-text: #F4F1EA;
  --color-code-border: #2D373F;
}
```

### Aplicação das Cores

| Elemento | Variável Recomendada | Justificativa |
|---|---|---|
| Fundo de slides de conteúdo | `--color-background` | Fundo claro padrão para leitura agradável e sem cansaço visual. |
| Fundo de capa e transições | `--color-graphite` | Cria quebra de ritmo e transição visual evidente entre blocos. |
| Títulos e textos de alto contraste | `--color-graphite` (conteúdo) / `--color-background` (transição) | Máxima legibilidade em projeção e impressão. |
| Marcadores, links e ênfases | `--color-primary` | Guiar a leitura técnica para conceitos-chave. |
| Alertas, defeitos e notas de atenção | `--color-accent` | Sinalização pontual de advertências ou limitações. |
| Subtítulos, metadados e rodapé | `--color-secondary` | Informações de apoio que não competem com o conteúdo. |

---

## 3. Tipografia

### Fontes

- **Principal (Texto, Títulos e Estrutura):** `Inter` (pesos 400, 500 e 600).
- **Complementar (Código, Matrizes e Notações):** `IBM Plex Mono` (pesos 400 e 500).

```css
:root {
  --font-primary: 'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
  --font-mono: 'IBM Plex Mono', 'SFMono-Regular', Consolas, monospace;
}
```

### Escala de Tamanhos (Referência Widescreen 1280×720)

| Nível Hierárquico | Variável | Tamanho Equivalente | Peso | Uso |
|---|---|---|---|---|
| Título da Capa | `--text-title-cover` | 42px (~32pt) | 600 | Abertura do tema da aula |
| Título de Slide | `--text-title-slide` | 32px (~24pt) | 600 | Cabeçalho de cada slide |
| Subtítulo | `--text-subtitle` | 20px (~15pt) | 400–500 | Contexto imediato abaixo do título |
| Corpo do Texto | `--text-body` | 18px (~13.5pt) | 400 | Explicações e listas |
| Código e Notação | `--text-code` | 15px (~11.5pt) | 400–500 | Trechos em C, comandos de terminal |
| Legendas e Rodapé | `--text-caption` | 13px (~10pt) | 400 | Metadados, notas e numeração |

---

## 4. Estrutura e Layout (16:9)

### Geometria do Slide

- **Largura nominal:** `1280px` (`--slide-width`)
- **Altura nominal:** `720px` (`--slide-height`)
- **Margens internas:** `48px` vertical, `64px` horizontal (`--slide-padding-y`, `--slide-padding-x`)
- **Alinhamento predominante:** À esquerda.

### Estrutura dos Slides (12 Slides Concisos)

A apresentação foi calibrada para exatamente **12 slides**, aplicando o padrão pedagógico de **Pergunta (Provocação/Dilema)** seguida imediatamente da **Resposta com Explicação Técnica e Diagrama**:

1. **Slide 01 (Capa):** Controle por Teclado e Mouse, Movimentação e Colisão.
2. **Slide 02 (Roteiro):** As 5 etapas da aula (120 minutos).
3. **Slide 03 (Transição Bloco 1):** Fundamentação Teórica da Entrada.
4. **Slide 04 (❓ Pergunta 01):** *Ao manter uma tecla pressionada, por que o movimento hesita antes de se tornar contínuo?*
5. **Slide 05 (💡 Resposta 01 + Explicação):** Repetição de tecla do SO vs leitura por estado (`glutIgnoreKeyRepeat` e vetor `teclas[256]`).
6. **Slide 06 (Delta Time):** Tempo de quadro e movimento independente da taxa de quadros (código em C).
7. **Slide 07 (Mouse e Câmera):** Captura virtual (`glutWarpPointer`) e trava de inclinação vertical (pitch em $\pm 89^\circ$).
8. **Slide 08 (❓ Pergunta 02):** *Por que o personagem trava ao encostar em uma parede caminhando em diagonal?*
9. **Slide 09 (💡 Resposta 02 + Explicação):** Colisão com a matriz e deslize eixo a eixo (com diagrama vetorial AABB).
10. **Slide 10 (Câmeras 1ª e 3ª Pessoa):** Ponto de vista em `gluLookAt` e zoom por campo de visão (FOV).
11. **Slide 11 (Referência GLUT):** Quadro de resumo das funções C da API.
12. **Slide 12 (Demonstração Prática):** Validação experimental com flags de compilação do `Makefile`.

---

## 5. Diretrizes para Elementos Gráficos

### Permitido e Incentivado:
- **Grafismos e Marcas d'Água Vetoriais de Fundo (SVG nas Bordas):**
  - Ilustrações geométricas sutis (cubos isométricos, eixos cartesianos, relógios de delta time, caixas AABB) posicionadas nas extremidades e cantos dos slides.
  - Opacidade semitransparente muito baixa (definida por `--opacity-watermark-light: 0.045` nos slides claros e `--opacity-watermark-dark: 0.08` nos slides escuros).
  - Servem para preencher e emoldurar as bordas dos slides de forma técnica e elegante, sem competir com a leitura do texto principal.
- **Ícones Vetoriais e Diagramas Geométricos (SVG):**
  - Cubo Isométrico / Voxel (representação do mundo 3D e estruturas discretas).
  - Computador / Monitor (representação de hardware, laço e pipeline gráfico).
  - Mouse / Cursor e Teclado (representação de entrada interativa).
  - Diagrama vetorial AABB (grade voxel discreta, caixa delimitadora e vetores de colisão).
  - *Regra estrita:* Todos os SVGs devem utilizar traço fino (`1.5px`), sem preenchimentos pesados, consumindo exclusivamente `currentColor` ou variáveis CSS (`var(--color-...)`).
- Linhas divisórias horizontais finas (`1px solid var(--color-border-subtle)`).
- Bordas laterais de destaque sólido (`2px solid var(--color-primary)` ou `var(--color-accent)`).
- Tabelas acadêmicas com cabeçalho sóbrio em `--color-surface` e tipografia monospace para termos de API.
- Blocos de código com fundo escuro técnico e visual limpo (sem botões de janela decorativos).
- Marcadores tipográficos limpos (`—`, `▪`, `1.`, `a)`).

### Estritamente Proibido:
- Nenhum gradiente colorido decorativo (estilo arco-íris, roxo-azulado fluorescente).
- Nenhuma sombra com raio superior a 8px ou efeito *glow*.
- Nenhum cartão flutuante com cantos excessivamente arredondados (`border-radius` máximo de 6px quando indispensável).
- Não transformar cada parágrafo em um "card".
- Nenhum emoji (`🎮`, `⏱`, `🚀`, `💡`, etc.) como ícone de interface ou lista.
- Nenhuma ilustração figurativa de IA (cérebros, robôs, engrenagens fluorescentes).

---

## 6. Checklist de Implementação

- [x] Todas as cores no CSS derivam de variáveis `:root`.
- [x] Slides de conteúdo usam o fundo `#F4F1EA` (Branco quente).
- [x] Capa e divisores usam o fundo `#20272D` (Grafite).
- [x] Tipografia utiliza exclusivamente `Inter` e `IBM Plex Mono`.
- [x] Emojis e decorações genéricas removidos.
- [x] Impressão via `@media print` gera PDF limpo e formatado em proporção 16:9.

