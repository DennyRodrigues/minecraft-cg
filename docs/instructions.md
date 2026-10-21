# Instruções do jogo

Todos os atalhos e movimentos disponíveis em `mine`.

## Compilar e abrir

```bash
make run
```

O personagem começa no gramado, olhando para a montanha, com a câmera em terceira pessoa (atrás e acima dele). **F5** ou **V** passa para a primeira pessoa. O mouse já começa controlando o olhar, com o cursor oculto. Para usar o cursor fora do jogo, aperte **Tab**.

## Movimentação

Estas teclas funcionam enquanto estiverem pressionadas, e várias podem ser usadas ao mesmo tempo.

| Tecla | Ação |
|---|---|
| **W** | Andar para frente |
| **S** | Andar para trás |
| **A** | Andar para a esquerda |
| **D** | Andar para a direita |
| **R** (segurando) | Correr: dobra a velocidade |
| **Espaço** ou **↑** | Subir |
| **Shift** ou **↓** | Descer |
| **←** | Girar o olhar para a esquerda |
| **→** | Girar o olhar para a direita |

- A velocidade é de 5 blocos por segundo para andar e de 90 graus por segundo para girar, qualquer que seja o FPS.
- Duas teclas juntas fazem andar na diagonal, por exemplo **W + D** (frente e direita), na mesma velocidade que em linha reta.
- Segurar **R** junto com as teclas de movimento faz o personagem correr, com o dobro da velocidade (10 blocos por segundo).
- A direção de "frente" segue apenas o giro horizontal do olhar. Olhar para cima ou para baixo e apertar W não faz o personagem subir nem descer.
- Maiúsculas e minúsculas funcionam igual, e segurar Shift não atrapalha.

## Olhar com o mouse

| Entrada | Ação |
|---|---|
| **Tab** | Libera o cursor, que volta a aparecer e deixa de controlar o olhar. Apertar de novo captura outra vez. |
| **Clique na janela** | Com o cursor liberado, captura de novo |
| **Mover o mouse para os lados** | Girar o olhar para a esquerda ou para a direita |
| **Mover o mouse para cima e para baixo** | Inclinar o olhar para cima ou para baixo, até 89° |

- O mouse também controla o olhar com um botão pressionado.
- Com o cursor liberado, o mouse não mexe no olhar, e dá para sair da janela normalmente.
- As setas ← e → continuam girando o olhar mesmo com o mouse capturado.

## Câmera e visualização

| Entrada | Ação |
|---|---|
| **F5** ou **V** | Alterna entre primeira pessoa (vista dos olhos) e terceira pessoa (personagem visto de trás e de cima) |
| **Roda do mouse para cima** ou **+** | Aproximar (diminui o campo de visão) |
| **Roda do mouse para baixo** ou **-** | Afastar (aumenta o campo de visão) |
| **P** | Alterna entre projeção perspectiva e ortográfica |

- A troca de câmera pode ser feita a qualquer momento, inclusive andando. A posição e o movimento não mudam, só o ponto de vista.
- Em terceira pessoa a câmera fica 4 blocos atrás e 1,5 bloco acima da cabeça. Mover o mouse para cima ou para baixo só inclina a vista; a câmera não muda de altura.
- O zoom vai de 20° (mais perto) a 90° (mais longe), em passos de 5°. O padrão é 60°. Ele não move o personagem.
- Roda e teclado têm o mesmo efeito: cada toque em **+** ou **-** equivale a um clique da roda. Segurar a tecla não repete; aperte de novo para continuar. Valem o **+** e o **-** da linha de números (o **+** funciona com ou sem Shift, pela tecla **=**) e os do teclado numérico.
- Na projeção ortográfica os objetos não diminuem com a distância. Nela, o zoom pela roda não tem efeito.

## Outros

| Tecla | Ação |
|---|---|
| **ESC** | Abre o menu de pausa, com as instruções de movimentação. O jogo para e o cursor fica livre. ESC de novo volta ao jogo. |
| **Q** (com o menu aberto) | Fecha o jogo |

O título da janela mostra o FPS e quais versões de demonstração estão ativas.

## Como o personagem interage com o mundo

- **Colisão:** não é possível atravessar terra, pedra, grama, neve, areia, tronco, folhas nem nuvens.
- **Deslizar:** andar na diagonal contra uma parede faz o personagem deslizar rente a ela em vez de parar.
- **Bordas:** os limites do mapa funcionam como paredes invisíveis, e acima de y = 16 há um teto.
- **Água:** dá para entrar na água, porque ela não bloqueia, mas não para atravessar a areia do fundo.
- **Sem gravidade:** o personagem flutua na altura em que está. Para subir um degrau de um bloco, aperte **Espaço** (ou **↑**) e depois ande. Ao sair de uma borda, ele não cai; use **Shift** (ou **↓**) para descer.
- **Animação:** em terceira pessoa, braços e pernas balançam enquanto o personagem anda (mais rápido correndo) e voltam ao lugar quando ele para.
- **Tamanho do personagem:** 0,6 bloco de largura e 1,8 de altura, com os olhos a 1,6 do chão. Ele cabe em passagens de 1 bloco de largura e 2 de altura.

## Versões de demonstração

Estas opções não são teclas: escolhem, na compilação, uma versão "errada" do código para mostrar em sala o defeito que a versão final corrige. Passe-as ao `make run`; ele recompila sozinho.

| Comando | Efeito no jogo |
|---|---|
| `make run EXPERIMENTO_TIMER=16` ou `=50` | Atualiza a cada 16 ou 50 ms, simulando uma máquina rápida e uma lenta |
| `make run USAR_DT=0` | Passo fixo por quadro: a velocidade passa a depender do FPS |
| `make run VERSAO_COLISAO=1` | Colisão só no ponto central: o personagem entra pela metade nos blocos, e a parede some ao chegar perto |
| `make run VERSAO_COLISAO=2` | Colisão com a caixa inteira, mas sem deslizar |
| `make run VERSAO_COLISAO=3` | Testa os eixos juntos: o personagem trava ao encostar na diagonal |
| `make run LIMITAR_PITCH=0` | Sem limite de inclinação: ao olhar direto para cima a imagem inverte |

As chaves podem ser combinadas, por exemplo `make run USAR_DT=0 EXPERIMENTO_TIMER=50`. `make run` sem chaves volta à versão final, e `make ajuda` lista todas as opções.
