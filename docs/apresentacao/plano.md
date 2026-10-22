PLANO DE AULA
Computação Gráfica, Semana 5
Controle por teclado e mouse, movimentação e colisão, das 19h00 às 21h00 (120 minutos)
1. Bloco 1, fundamentação teórica da entrada interativa (19h00 às 19h25, 25 min)
Bloco expositivo, conduzido em quadro e diagrama, sem código. Abrir executando a navegação da semana anterior e chamando atenção para dois defeitos que a turma provavelmente já percebeu: ao manter uma tecla pressionada, o movimento hesita antes de se tornar contínuo, e em máquinas mais rápidas a câmera anda mais depressa. Os dois defeitos têm a mesma origem, a forma como a entrada foi lida, e explicá-los organiza o bloco inteiro.
1.1 Fundamentação teórica
    • A entrada não chega à aplicação no momento em que o usuário age. O sistema operacional acumula os acontecimentos em uma fila, e na GLUT quem a esvazia é glutMainLoop, que assume o laço principal e chama as funções registradas pela aplicação para cada tipo de acontecimento. A aplicação não escreve o laço: ela registra funções e reage a elas.
    • Existem duas formas de ler a entrada. Por evento, uma função é chamada a cada acontecimento, como pressionar ou soltar uma tecla. Por estado, a aplicação verifica, a cada quadro, se a tecla está pressionada naquele instante. As duas são corretas, mas servem a propósitos distintos.
    • A GLUT oferece apenas a leitura por evento: não existe função para perguntar se uma tecla está pressionada. O estado precisa ser construído pela própria aplicação, com um vetor de teclas marcado como pressionado na função de pressionar e como solto na função de soltar. É um bom exercício de modelagem, pois deixa explícito que estado é uma memória dos eventos passados.
    • O evento de repetição de tecla é gerado pelo sistema operacional com um atraso inicial e uma cadência própria, pensados para digitação de texto. Movimento conduzido por esse evento herda o atraso e a cadência, o que explica a hesitação observada na abertura. Quando o estado é construído a partir dos eventos, a repetição não acrescenta informação e deve ser desativada por glutIgnoreKeyRepeat.
    • Movimento contínuo deve ser lido por estado e escalado pelo tempo de quadro, obtido pela diferença entre duas leituras de glutGet(GLUT_ELAPSED_TIME), que informa milissegundos desde o início do programa. Somar um deslocamento fixo por quadro faz a velocidade depender da taxa de quadros da máquina; multiplicar a velocidade pelo tempo decorrido torna o movimento igual em qualquer equipamento. A atualização é feita na função registrada por glutIdleFunc, que termina pedindo um novo desenho por glutPostRedisplay.
1.2 Quando usar cada forma de leitura
Forma de leitura
Adequada para
Exemplos na disciplina
Por evento
Ações discretas, que acontecem uma vez por toque
Alternar entre primeira e terceira pessoa, alternar projeção, encerrar a aplicação, liberar o cursor.
Por estado
Ações contínuas, que duram enquanto a tecla estiver pressionada
Deslocamento da câmera, giro contínuo, aceleração.

1.3 Referências do bloco
    • Recursos avançados de teclado na GLUT, com glutIgnoreKeyRepeat e as funções de soltura de tecla, material em inglês: https://www.lighthouse3d.com/tutorials/glut-tutorial/advanced-keyboard-features/
    • Documentação da API da freeglut, seções de funções de resposta e de glutGet, material em inglês: https://freeglut.sourceforge.net/docs/api.php
2. Bloco 2, mouse e orientação da câmera (19h25 às 19h50, 25 min)
Segundo bloco teórico, que conecta a entrada do mouse ao modelo de câmera da semana anterior.

2.1 Conteúdo
    • A posição do cursor é informada em coordenadas de tela, com origem no canto superior esquerdo e eixo vertical crescendo para baixo, ao contrário das coordenadas normalizadas vistas na semana 2. Convém registrar o contraste, pois a inversão do eixo vertical é fonte recorrente de visada invertida.
    • A GLUT distingue o movimento com botão pressionado, tratado por glutMotionFunc, do movimento sem botão, tratado por glutPassiveMotionFunc. Para uma câmera que acompanha o mouse livremente, as duas funções devem ser registradas, ou a visada congela enquanto um botão estiver pressionado.
    • Para orientar a câmera não interessa a posição absoluta do cursor, e sim o deslocamento. A GLUT não oferece captura do cursor; o efeito é obtido ocultando-o por glutSetCursor(GLUT_CURSOR_NONE) e devolvendo-o ao centro da janela por glutWarpPointer após cada movimento. Calculando o deslocamento sempre em relação ao centro, e não à última posição, o evento provocado pelo próprio reposicionamento resulta em deslocamento nulo e não interfere no giro. A captura precisa de uma tecla de liberação, sob pena de o usuário não conseguir sair da janela.
    • A orientação é representada por dois ângulos, designados pelos termos em inglês de uso corrente na área: yaw, o giro em torno do eixo vertical, e pitch, a inclinação para cima ou para baixo. O deslocamento horizontal do mouse altera o yaw, o vertical altera o pitch, e o vetor de direção é obtido a partir dos dois ângulos por seno e cosseno, sendo então repassado a gluLookAt como ponto observado.
    • O pitch precisa ser limitado a pouco menos de noventa graus. No limite, a direção de visada se torna paralela ao vetor de orientação vertical, situação exatamente correspondente à segunda pergunta de verificação da semana anterior. O problema que lá era teórico aparece aqui como defeito concreto.
    • Primeira e terceira pessoa compartilham a mesma orientação por yaw e pitch e diferem apenas nos argumentos de gluLookAt. Em primeira pessoa, a câmera ocupa a posição do personagem e observa um ponto à frente dele. Em terceira pessoa, o personagem passa a ser o ponto observado, e a câmera é recuada ao longo da direção oposta ao olhar e elevada acima dele, a uma distância fixa. O personagem precisa então ser desenhado, o que não ocorre em primeira pessoa, pois ocuparia o próprio ponto de vista.
    • A roda do mouse altera o campo de visão da projeção perspectiva, produzindo efeito de aproximação sem deslocar o observador, o que retoma a distinção entre mover a câmera e trocar a lente. A roda é tratada por glutMouseWheelFunc, extensão da freeglut que exige incluir o cabeçalho GL/freeglut.h.
2.2 Quadro de referência da entrada
Operação
Chamada em C
Efeito
Pressionar tecla
glutKeyboardFunc(funcao)
Teclas de caractere; recebe a tecla e a posição do cursor.
Soltar tecla
glutKeyboardUpFunc(funcao)
Necessária para construir o estado das teclas.
Teclas especiais
glutSpecialFunc e glutSpecialUpFunc
Setas e teclas de função, pressionar e soltar.
Repetição
glutIgnoreKeyRepeat(1)
Desativa a repetição automática de tecla.
Botões do mouse
glutMouseFunc(funcao)
Chamada a cada clique ou soltura.
Movimento do mouse
glutMotionFunc e glutPassiveMotionFunc
Com e sem botão pressionado, respectivamente.
Roda do mouse
glutMouseWheelFunc(funcao)
Extensão da freeglut, em GL/freeglut.h.
Ocultar cursor
glutSetCursor(GLUT_CURSOR_NONE)
Torna o cursor invisível.
Reposicionar cursor
glutWarpPointer(x, y)
Devolve o cursor ao centro, simulando a captura.
Atualização
glutIdleFunc(funcao)
Chamada quando não há eventos; local da atualização do movimento.
Tempo decorrido
glutGet(GLUT_ELAPSED_TIME)
Milissegundos desde o início; base do tempo de quadro.

2.3 Referências do bloco
    • O mouse na GLUT, com cliques e movimento ativo e passivo, material em inglês: https://www.lighthouse3d.com/tutorials/glut-tutorial/the-mouse/
    • Especificação da GLUT, seção de glutSetCursor, material em inglês: https://www.opengl.org/resources/libraries/glut/spec3/node28.html
    • Câmera virtual, seções sobre ângulos de orientação, entrada do mouse e aproximação, material em inglês baseado em outra biblioteca de janelas, mas com os mesmos conceitos: https://learnopengl.com/Getting-started/Camera
3. Bloco 3, teclado por estado e tempo de quadro (19h50 às 20h05, 15 min)
3.1 O que precisa ser desenhado na tela
O cenário da semana anterior percorrido pelo teclado com movimento imediato e contínuo, sem hesitação inicial, e com a mesma velocidade independentemente da taxa de quadros.
3.2 O que precisa ser alcançado no código
Vetor de estado das teclas atualizado pelas funções de pressionar e soltar, com a repetição desativada, e deslocamento aplicado na função de atualização registrada por glutIdleFunc, com velocidade multiplicada pelo tempo de quadro. A função de pressionar tecla continua respondendo diretamente às ações discretas, como alternar a projeção e encerrar a aplicação.
3.3 Condução sugerida
Antes da refatoração, substituir a função ociosa por glutTimerFunc com intervalos distintos, por exemplo 16 e 50 milissegundos, simulando uma máquina rápida e uma lenta, e pedir que a turma compare a velocidade da câmera. Repetir o experimento após a correção. A comparação torna evidente que o problema estava na leitura da entrada, e não no desenho.
3.4 Referências do bloco
    • Movimento da câmera com teclas soltas e pressionadas na GLUT, material em inglês: https://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world-ii/
    • Consulta rápida da documentação de funções durante a prática: https://docs.gl
4. Bloco 4, colisão com o mundo (20h05 às 20h25, 20 min)
Fundamentação curta em quadro, seguida de prática. Até aqui o observador atravessa colunas, troncos e o próprio chão. A colisão é o que transforma a câmera em personagem.
4.1 Fundamentação teórica
    • Colisão tem duas etapas distintas: a detecção, que descobre se duas formas se sobrepõem, e a resposta, que decide o que fazer quando se sobrepõem. Nesta aula a resposta é a mais simples possível: impedir o movimento.
    • O personagem é representado por uma caixa alinhada aos eixos, com largura e altura fixas e sem rotação. Por isso o giro do yaw não altera a caixa. Duas caixas desse tipo colidem se, e somente se, seus intervalos se sobrepõem nos três eixos ao mesmo tempo, teste que se resume a comparações.
    • Em um mundo de blocos não é preciso comparar o personagem com cada bloco existente. As coordenadas do personagem, arredondadas para baixo, já são os índices da matriz do mundo, e basta consultar as poucas posições que a caixa ocupa. É aqui que a matriz construída pelos grupos como tarefa do projeto deixa de ser apenas organização de dados: colisão passa a ser uma consulta à matriz.
    • O teste é feito antes do movimento: calcula-se a posição pretendida, verifica-se se ela está livre e o deslocamento só é aplicado em caso positivo.
    • O deslocamento é testado eixo a eixo, primeiro em X e depois em Z. Se apenas um eixo estiver bloqueado, o outro ainda é aplicado, e o personagem desliza rente à parede em vez de travar ao encostar nela em diagonal.
    • Um deslocamento por quadro maior que um bloco pode saltar por cima de uma parede fina sem que ela seja detectada, fenômeno conhecido como tunelamento. A ligação com o tempo de quadro do bloco anterior é direta: um quadro lento produz um passo grande, e por isso convém limitar o deslocamento máximo por quadro.
4.2 O que precisa ser desenhado na tela
O personagem percorrendo o mundo sem atravessar colunas, troncos ou paredes, e deslizando ao longo delas quando caminha em diagonal contra uma superfície.
4.3 O que precisa ser alcançado no código
Uma função que informa se uma posição do mundo está ocupada por bloco sólido, consultando a matriz e tratando posições fora dos limites como sólidas, para manter o personagem dentro do mundo. Antes de aplicar cada eixo do deslocamento, a função é consultada nos pontos que delimitam a caixa do personagem, considerando sua largura e as alturas dos pés e da cabeça.
4.4 Condução sugerida
Implementar primeiro o teste apenas no ponto central do personagem e deixar a turma observar que ele entra pela metade nos blocos antes de parar. Em seguida, considerar a largura da caixa. Por fim, testar os dois eixos juntos, observar o travamento ao encostar em diagonal e corrigir com o teste eixo a eixo. Cada etapa corrige um defeito visível, o que mantém a progressão concreta.
4.5 Referências do bloco
    • Detecção de colisão 2D, MDN Web Docs, em português, com a caixa delimitadora alinhada por eixo: https://developer.mozilla.org/pt-BR/docs/Games/Techniques/2D_collision_detection
    • Detecção de colisão 3D, MDN Web Docs, material em inglês, com o teste entre caixas nos três eixos: https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
5. Bloco 5, câmera em primeira e terceira pessoa (20h25 às 21h00, 35 min)
Bloco de síntese, no qual teclado e mouse passam a operar juntos sobre a câmera.
5.1 O que precisa ser desenhado na tela
O cenário percorrido por um personagem simples, representado por um dos sólidos da semana 3, com teclado para deslocamento à frente, atrás e para os lados, mouse para a direção do olhar e uma tecla que alterna entre primeira pessoa, em que a cena é vista dos olhos do personagem, e terceira pessoa, em que o personagem é visto por trás e de cima. Completam a interação a roda para aproximação e uma tecla para capturar e liberar o cursor.
5.2 O que precisa ser alcançado no código
Posição do personagem e ângulos de yaw e pitch armazenados em variáveis próprias, com os ângulos atualizados pelo deslocamento relativo do mouse e o pitch limitado. O avanço segue a direção do olhar projetada no plano horizontal, de modo que olhar para cima não faça o personagem sair do chão. Uma variável de modo, alternada por evento de tecla, decide os argumentos de gluLookAt a cada quadro: em primeira pessoa, posição do personagem e ponto à frente; em terceira pessoa, posição recuada e elevada e o próprio personagem como alvo. O personagem é desenhado, transladado para sua posição e girado pelo yaw, somente em terceira pessoa. O campo de visão é ajustado pela roda e reaplicado à matriz de projeção.
5.3 Condução sugerida
Implementar primeiro a primeira pessoa, apenas com o yaw, verificar que o avanço acompanha a nova direção, e só então acrescentar o pitch com seu limite. Remover propositalmente o limite e deixar a turma observar a inversão brusca da imagem ao olhar diretamente para cima, retomando o motivo apresentado no Bloco 2. A terceira pessoa vem por último, como variação dos mesmos dados: nada no movimento muda, apenas os argumentos de gluLookAt e o desenho do personagem, o que evidencia que o modo de câmera é uma decisão de visualização, e não de lógica de movimento. Convém pedir que a turma alterne os modos durante o deslocamento e verifique que o personagem permanece no mesmo lugar.
5.4 Referências do bloco
    • Câmera virtual, material em inglês: https://learnopengl.com/Getting-started/Camera
    • Consulta rápida da documentação de funções: https://docs.gl
6. Atualização do projeto do jogo
O grupo responsável pela apresentação desta semana deve atualizar o projeto do jogo com o código desenvolvido nesta aula, incorporando a movimentação contínua, a colisão com o mundo e a câmera em primeira e terceira pessoa, e demonstrar em sala a evolução obtida.
