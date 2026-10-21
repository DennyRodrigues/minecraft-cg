# Apresentação de Computação Gráfica • UNIFAP

Apresentação de slides técnica, editorial e minimalista em HTML5 e CSS3 (proporção 16:9), estruturada conforme o documento [`visual-design-guidelines.md`](visual-design-guidelines.md).

---

## 🎨 Paleta de Cores e Variáveis (`style.css`)

**100% das cores são controladas por variáveis no bloco `:root` de `style.css`:**

| Variável | Cor | Amostra | Função |
|---|---|---|---|
| `--color-graphite` | Grafite (`#20272D`) | Fundo escuro | Títulos, textos principais de alto contraste, capas e transições. |
| `--color-background` | Branco quente (`#F4F1EA`) | Fundo claro | Fundo padrão de todos os slides de conteúdo (70% do tempo). |
| `--color-primary` | Azul-petróleo (`#176B70`) | Destaque 1 | Termos-chave, divisores, marcadores e chamadas técnicas. |
| `--color-accent` | Terracota (`#D28B51`) | Destaque 2 | Alertas, defeitos comparados e ênfases secundárias. |
| `--color-secondary` | Cinza azulado (`#68747B`) | Texto de apoio | Subtítulos, metadados, legendas e referências. |

---

## ✍️ Tipografia

- **Texto e títulos:** `Inter` (pesos 400, 500 e 600) via Google Fonts (com fallback do sistema).
- **Notação e código:** `IBM Plex Mono` (pesos 400 e 500) para trechos em C, comandos de terminal e termos de API.

---

## 🚀 Como Visualizar e Apresentar

Abra o arquivo diretamente no navegador (não exige Node.js, compilação ou internet para abrir):

```bash
xdg-open docs/slides/index.html
# ou
google-chrome docs/slides/index.html
```

---

## 🖨 Como Exportar para PDF

As regras `@page` e `@media print` já estão configuradas para o formato 16:9 widescreen:

1. No navegador, pressione **`P`** ou **`Ctrl + P`**.
2. Na caixa de diálogo de impressão:
   - **Destino:** *Salvar como PDF*
   - **Layout / Orientação:** *Paisagem* (*Landscape*)
   - **Margens:** *Nenhuma* (*None*)
   - **Opções:** Marque **`Gráficos de segundo plano`** (*Background graphics*)
3. Clique em **Salvar**.

---

## ⌨️ Atalhos de Apresentação

| Tecla | Ação |
|---|---|
| **`→`**, **`Espaço`**, **`Enter`** | Próximo slide |
| **`←`**, **`Backspace`** | Slide anterior |
| **`Home`** / **`End`** | Primeiro / Último slide |
| **`F`** | Alternar tela cheia (*Fullscreen*) |
| **`O`** | Visão geral em grade de miniaturas (*Overview*) |
| **`P`** | Exportar para PDF / Imprimir |
| **`?`** | Janela de atalhos de teclado |
| **`ESC`** | Fechar janelas ou sair da visão geral |
