# Projeto-SIN-211
Projeto de disciplina de SIN 211 - Estruturas de Dados (06/11/2019)

1) Introdução

A rotina dos estudantes da UFV é repleta de tensões e estresses diários por causa de suas obrigações,
com isso surge a necessidade de aliviar esse estresse. A tecnologia possibilita usarmos diversos
meios para nos entreter no tempo livre, contudo o mais utilizado e interessante para os jovens
são as redes sociais, entretanto elas consomem muito do nosso tempo sem agregar tanto no nosso
desenvolvimento.

2) Cenário de Aplicação

Com o intuito de criar algo divertido e estimulante para os estudantes resolvemos desenvolver o jogo
Simon Says. É muito simples, divertido e rápido de jogar, sendo assim uma atividade viável para
nós, além de trazer benefícios como o aprimoramento da percepção e fomentação da capacidade
de memorização. O jogo consiste em, dada uma sequência de cores, o jogador deve memorizá-las e
reproduzi-las, cada vez que ele acerta incrementa mais uma cor na sequência, o processo se repete
até que o jogador erre, dando fim ao jogo.

3) Solução do Problema

Para solucionarmos o problema da montagem do circuito do projeto usamos os seguintes componentes do Arduino:
• Placa Arduino (1);
• Placa de Ensaio (1);
• LEDs (4);
• Resistências de 330 Ohm (4);
• Resistências de 10 kOhm (4);
• Botões (4);
• Piezobuzzer (1);

A placa Arduino é empregada para rodar o código, os LEDs apresentam as cores da sequência,
os botões são utilizados para receber a resposta do jogador quando ele estiver reproduzindo a
sequência, e o buzzer é responsável por emitir um som correspondente cada vez que aparecer uma
cor ou caso o usúario perca o jogo. O código basicamente é constituido de três estados do jogo:
• Start (Quando o sistema está esperando o jogador iniciar o jogo);
• Play (Quando o jogo está sendo jogado);
• GameOver (Quando o jogador perde o jogo);

O estado Start, consiste na função esperarIniciar, que irá esperar o usuário apertar um botão qualquer para começar o jogo. Dentro do estado Play temos as funções mostrarSequencia e lerSequencia, que mostrará para o jogador a sequência de cores geradas e lerá a sua resposta,
comparando-a para ver se ele acertou. No estado GameOver temos a função piscarTodas e a atribuição do estado do jogo para Start, todos os LEDs vão piscar caso o jogador tenha perdido e vai se preparar para iniciar outro jogo.

Link: https://www.tinkercad.com/things/b4mbyM69C2Y
