// Definindo todos os pinos dos LEDs e dos botões na seguinte sequência: {Vermelho, Verde, Amarelo, Azul}
int botoes[] = {3, 5, 7, 9};
int leds[] = {2, 4, 6, 8};
int frequencias[] = {310, 415, 252, 209}; //Frequências dos sinais sonoros

//Definição da estrutura: Lista Simplesmente Encadeada
typedef struct sCELULA{
  int info;
  struct sCELULA *next;
}CELULA;

//Assinatura das funções da Lista Simplesmente Encadeada
void init(CELULA **lista);
void inserir_final(CELULA **lista, int x);
void remover_inicio(CELULA **lista);
void mostrarSequencia(CELULA **lista);
void lerSequencia(CELULA *lista);

//Definição dos estados do jogo
const int START = 0;    //O sistema está esperando o jogador iniciar o jogo ao pressionar um botão qualquer
const int PLAY = 1;     //O jogo está sendo jogado, mostrando uma sequência de LEDs que devem ser repetidos na ordem correta
const int GAMEOVER = 2; //A sequência foi errada ou o jogador demorou demais para responder: GAMEOVER

//Variável que receberá o estado atual do jogo
int estadoJogo;

//Variáveis que irão servir como cronômetro
long milisegAtual = 0;
long milisegAnterior = 0;

//Ponteiro externo
CELULA *ptrlista;


//Função: Inicializar o ponteiro externo
void init(CELULA **lista){
  *lista = NULL;
}

//Função: Inserir elemento no fim da sequência
void inserir_final(CELULA **lista, int x){
  CELULA *q;
  CELULA *aux;

  q = (CELULA*)malloc(sizeof(CELULA));
  if(q != NULL){
    q->info = x;
    q->next = NULL;
    if(*lista == NULL){
      *lista = q;
    }
    else{
      aux = *lista;
      while(aux->next != NULL){
        aux = aux->next;
      }
      aux->next = q;
    }
  }
  else{
    return;
  }
}

//Função: Remover elemento do início da sequência
void remover_inicio(CELULA **lista){
	CELULA *aux;
  
  	aux = *lista;
  	*lista = aux->next;
  	free(aux);
}

//Função: Mostrar sequência de LEDs
void mostrarSequencia(CELULA **lista){
  CELULA *aux;
  int x = random(0,4);
  inserir_final(&(*lista), x);  //Insere um novo elemento no final da sequência

  aux = *lista;
  while(aux != NULL){ //Enquanto aux for != NULL: pisca o LED na posição aux->info e depois aux recebe a próxima célula
     delay(300);
     digitalWrite(leds[aux->info], HIGH);
     tone(13, frequencias[aux->info]);   //Emite um som correspondente ao LED
     delay(400);
     noTone(13);
     delay(300);
     digitalWrite(leds[aux->info], LOW);
     aux = aux->next;
  }  
}

//Função: Ler a sequência de LEDs
void lerSequencia(CELULA *lista){
  CELULA *aux;
  int posicaoPressionada;     //Recebe o índice do botão pressionado pelo usuário
  boolean cometeuErro = false;  //Valor booleano para verificar se o usuário errou ou não

  aux = lista; 
  while( aux != NULL & cometeuErro == false){
    posicaoPressionada = esperarBotao(2000); //0, 1, 2, ou 3
    tone(13, frequencias[posicaoPressionada]); //Emite um som correspondente ao LED
    delay(400);
    noTone(13);
    //Verificar se o usuário não pressionou o botão a tempo ou pressionou o botão errado, nesse caso: GAMEOVER
    if(posicaoPressionada == -1 | posicaoPressionada != aux->info){
        cometeuErro = true; //Sai do loop
        estadoJogo = GAMEOVER;
        tone(13, 42); //Emite um som de GAMEOVER
        delay(1500);
        noTone(13);
    }
    aux = aux->next;
  }
}

//Retorna a posição do botão pressionado (0, 1, 2 ou 3) ou -1 se o botão não tiver sido pressionado a tempo
int esperarBotao(int delay){
  int botaoPressionado = -1;
  int input;
  boolean botaoBackUp = false;
  
  milisegAtual = millis();     //Número de ms desde que o programa iniciou
  milisegAnterior = milisegAtual;   //Recebe o ponto no tempo onde o loop iniciou
  
  //Continua rodando o loop até que 'delay' milisegundos tenham se passado
  while(milisegAtual - milisegAnterior < delay & botaoBackUp == false){
    //Lê o botão e grava quando ele foi pressionado
    for(int pino = 0; pino < 4 & botaoBackUp == false; pino++){
      if(digitalRead(botoes[pino]) == HIGH){
        botaoPressionado = pino;
        
        //Acende o LED do botão correspondente
        digitalWrite(leds[pino], HIGH);
              
        //É possível que o botão esteja sendo segurado pelo usuário
        //Esse loop é iterado até que o botão seja solto
        while(milisegAtual - milisegAnterior < delay & botaoBackUp == false){
          input = digitalRead(botoes[pino]);
          if(input == LOW){
            botaoBackUp = true;
          }
          milisegAtual = millis();
        }
        
        //Desliga o LED do botão correspondente
        digitalWrite(leds[pino], LOW);
        
        //Verifica se o usuário segurou o botão por tempo demais
        if(milisegAtual - milisegAnterior > delay){
          botaoPressionado = -1; //O usuário demorou demais para parar de apertar o botão
        }
      }
    }
    milisegAtual = millis();
  }
  return botaoPressionado;
}

void esperarIniciar(){
  int botaoPressionado = -1;
  apagarTodos();

  //Espera o usuário pressionar um botão
  for(int pino = 0; pino < 4; pino++){
    if(botaoPressionado == -1){
      digitalWrite(leds[pino], HIGH);
      botaoPressionado = esperarBotao(800);
      digitalWrite(leds[pino], LOW);
    }
  }
  
  if(botaoPressionado != -1){
    //Um botão foi pressionado, então espera 2 segundos para o jogo iniciar
    delay(2000);
    if(ptrlista != NULL){	//Se o jogo já tiver sido jogado, ptrlista será != NULL, logo, é necessário dar um free para reiniciar a sequência
      while(ptrlista != NULL){
      	remover_inicio(&ptrlista);
      }
    }
    estadoJogo = PLAY; 
  }
}

//Apaga todos os LEDs
void apagarTodos(){
  for(int pino = 0; pino < 4; pino++){
    digitalWrite(leds[pino], LOW);
  }
}

//Acende todos os LEDs
void acenderTodos(){
  for(int pino = 0; pino < 4; pino++){
    digitalWrite(leds[pino], HIGH);
  }
}

//Itera um loop até que 'delay' milisegundos passem
//Enquanto o loop é iterado, observamos o valor do botão para ver se ele está pressionado ou não
boolean lerQualquerBotao(int delay){
  boolean botaoDown = false;    //Verifica se o botão foi pressionado e retornou à posição original
  
  milisegAtual = millis();     //Recebe o número de milisegundos desde o início do programa
  milisegAnterior = milisegAtual;   //Recebe o ponto onde foi iniciada a iteração do loop
  
  //Continua a iterar o loop até 'delay' segundos terem se passado
  while (milisegAtual - milisegAnterior < delay & botaoDown == false){
    //Lê o botão e grava quando ele foi pressionado
    for(int pino = 0; pino < 4; pino++){
      if(digitalRead(botoes[pino]) == HIGH){
        botaoDown = true;
      }
    }
    milisegAtual = millis();
  }
  return botaoDown;
}

//Piscar todos os LEDs
void piscarTodos(int n){
  for(int cont = 0; cont < n; cont++){
    acenderTodos();
    delay(300);
    apagarTodos();
    delay(300);
  } 
}

//Piscar o LED Vermelho 'n' vezes
void piscarVermelho(int n){
  for(int cont = 0; cont < n; cont++){
    digitalWrite(leds[0], HIGH);
    delay(300);
    digitalWrite(leds[0], LOW);
    delay(300);
  } 
}

//Piscar o LED Verde 'n' vezes
void piscarVerde(int n)
{
  for(int cont = 0; cont < n; cont++){
    digitalWrite(leds[1], HIGH);
    delay(300);
    digitalWrite(leds[1], LOW);
    delay(300);
  } 
}

//Piscar o LED Amarelo 'n' vezes
void piscarAmarelo(int n){
  for(int cont = 0; cont < n; cont++){
    digitalWrite(leds[2], HIGH);
    delay(300);
    digitalWrite(leds[2], LOW);
    delay(300);
  } 
}

//Piscar o LED Azul 'n' vezes
void piscarAzul(int n){
  for(int cont = 0; cont < n; cont++){
    digitalWrite(leds[3], HIGH);
    delay(300);
    digitalWrite(leds[3], LOW);
    delay(300);
  } 
}

//Inicialização dos componentes
void setup() { 
  
  for(int pino = 0; pino<4; pino++){  //Inicialização dos LEDs no pino 'i'
    pinMode(leds[pino], OUTPUT);
  }
  
  for(int pino = 0; pino<4; pino++){  //Inicialização dos botões no pino 'i'
    pinMode(botoes[pino], INPUT);
  }

  estadoJogo = START; //O estado do jogo é de esperar o usuário inserir um valor
  randomSeed(analogRead(40)); //Inicializa um gerador de números pseudo-aleatórios

  pinMode(13, OUTPUT); //Inicializa o Piezo Buzzer
  init(&ptrlista); //Inicializa o ponteiro externo com NULL
}

//Principal loop do jogo
void loop() {
  if(estadoJogo == START){  //Se o estado de jogo for START, espera o usuário iniciar o jogo
    esperarIniciar();
  }
  else{
    if(estadoJogo == PLAY){ //Se o estado de jogo for PLAY, o sistema mostrará uma sequência de LEDs e lerá as respostas do usuário
      mostrarSequencia(&ptrlista);
      lerSequencia(ptrlista);
    }
    else{
      if(estadoJogo == GAMEOVER){ //Se o estado de jogo for GAMEOVER, todos os LEDs piscaram 5 vezes e o jogo passará para o estado START
        piscarTodos(5);
        estadoJogo = START;
      }
    }
  }
}