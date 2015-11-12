#include <iostream>
#include <stdlib.h> // Para gerar n�meros aleat�rios
#include <fstream> // E / S de arquivos
#include <iomanip> // Para ler em caracter a caracter
#include <time.h> // Para trabalhar com o tempo
#include <math.h> // Para usar a fun��o potencia��o e raiz quadrada
#include <winsock2.h> // Para trabalhar com comunica��o em rede
#include <stdio.h>	  // Para trabalhar com funl��es da linguagem C
#include <windows.h> // Para trabalhar com fun��es de som do Windows
#include <graphics.h>

// Para usar a biblioteca Winsock (comunica��o em rede)
#pragma comment (lib, "Ws2_32.lib")

// Fun��es que N�O utilizam as estruturas dos arquivos header
void Carrega(void *arrayImg, void *arrayMask, char *rPath);
void GetImage(void** pImg, char path[], int width, int height);
void* GetImage(char path[], int width, int height);

// Bibliotecas criados pela equipe de desenvolvimento
#include "..\..\header\radiobutton.h"
#include "..\..\header\enumeracoes.h"
#include "..\..\header\td_defines.h"
#include "..\..\header\sprite.h"
#include "..\..\header\campojogo.h"
#include "..\..\header\tdelay.h"
#include "..\..\header\soldado.h"
#include "..\..\header\pagina.h"
#include "..\..\header\lider.h"
#include "..\..\header\torre.h"
#include "..\..\header\jogador.h"
#include "..\..\header\ondaeixo.h"
#include "..\..\header\grade.h"
#include "..\..\header\botao.h"
#include "..\..\header\barra_vida.h"
#include "..\..\header\rede.h"
using namespace std;



/*Fun��es que utilizam as fun��es dos arquivos header*/
void EnviaSold(Jogador *meuJog, Jogador *outroJog, CampoJogo meuCampo);
void Avisa(TDelay gameTime, Lider Hitler);
void MostraLideres(Lider *meuLider, Lider *outroLider);
void Aviso(int posX, int posY, char * msg, int color, Lider hitler);
void DefesaTorre(Jogador *meuJog, Jogador *outroJog, Jogador *eixoIA);
bool SemTorrePerto(Torre *torre0, int tileCimaX,int tileCimaY);



// Vari�veis globais
//======================================================
Jogador meuJog,  outroJog, eixoVsMeuJog,eixoVsOutroJog;
CampoJogo meuCampo;
char onda;			// Ceritifique-se que essas vari�veis
Pagina minhaPg;		// s�o inicializadas no come�o de todas
bool gameLoop;		// fun��es em que s�o utilzadas
TDelay gameTime;
Grade minhaGrd;
EscolhaEmMenu escolhaMenu;
Botao botaoUmJog, botaoDoisJog, botaoCredit, botaoJogar, botaoVoltar,
botaoCliente, botaoServidor, botaoConexao, botaoOpcaoServ;
char *ladoMeuJog,*ladoOutroJog;
int gameSpeed;
Radio radioSpeed, radioLider, radioModoIP;
Rede minhaRede;
//==========================================================

// Fun��es que usam vari�veis globais
void Gameplay(TipoGameplay tipoGameplay);
EscolhaEmMenu MenuUmJogador();
EscolhaEmMenu Menu();
EscolhaEmMenu MenuUmJog();
EscolhaEmMenu MenuDoisJog();
void BackgroundMenu();
EscolhaEmMenu MenuCliente();
EscolhaEmMenu MenuServidor();
void IAOutroJog();
void RecebePacoteJogo();
void SimulaOutroJog(TipoGameplay tipoGameplay,OndaEixo *ondaVsOutroJog);
void EnviaPacoteJogo();

int main(){
	
	// Inicializa biblioteca de conex�es em rede
	minhaRede.WinSockInit();
	
	int iResult;

	
	// Inicialize os bot�es que ser�o usados nos menus
	botaoUmJog.Init(BOTAO1_X,BOTAO1_Y,4,4);
	botaoDoisJog.Init(BOTAO2_X,BOTAO2_Y,5,4);
	botaoCredit.Init(BOTAO3_X,BOTAO3_Y,4,4);
	botaoJogar.Init(BOTAO_JOGAR_X,BOTAO_JOGAR_Y,3,1);
	botaoVoltar.Init(BOTAO_VOLTAR_X,BOTAO_VOLTAR_Y,3,1);
	botaoCliente.Init(BOTAO_CLIENTE_X, BOTAO_CLIENTE_Y, 5,4);
	botaoServidor.Init(BOTAO_SERV_X,BOTAO_SERV_Y ,5,4);
	botaoConexao.Init(TILE_W * 17,TILE_H * 12 + 8, 5,1);
	botaoOpcaoServ.Init(TILE_W * 11,TILE_H * 14,4,2);
	
	//Inicialize os radio buttons que ser�o usados nos menus
	radioSpeed.prox = NULL;
	radioLider.prox = NULL;
	radioModoIP.prox = NULL;
	radioSpeed.Insere(&radioSpeed,"4",false,TILE_W * 20 + 16, TILE_H * 10 + 16);
	radioSpeed.Insere(&radioSpeed,"8",true,TILE_W * 22 + 16, TILE_H * 10 + 16);	
	radioLider.Insere(&radioLider,"Stalin",true,TILE_W * 20 + 16, TILE_H * 12 + 16);
	radioLider.Insere(&radioLider,"Roosevelt",false,TILE_W * 23 + 16,TILE_H * 12 + 16);
	radioModoIP.Insere(&radioModoIP,"Autom�tico", true,TILE_W * 20 + 16, TILE_H * 10 + 16);
	radioModoIP.Insere(&radioModoIP,"Manual", false,TILE_W * 20 + 16, TILE_H * 11 + 16);
	
	// A velocidade do jogo ainda n�o foi definida
	gameSpeed = NULL;

	
	// Fornece uma seed para o gerador de n�meros pseudoaleat�rios
	srand(time(NULL));
	
	// Inicializa a estrutura p�gina
	minhaPg.Init();	
	
	// Inicializa a janela gr�fica
	initwindow(TELA_W,TELA_H, "Seek Of Peace: Cold WW2");
	
	// Processo de carregamento dos tiles do jogo
	minhaPg.Troca();
	minhaPg.Ativa();
	meuCampo.Init();	
	cleardevice();
	minhaPg.Visual();
	// ======= Fim do processamento ============
	
	escolhaMenu = MENU;
	
	// Loop do jogo
	while(escolhaMenu != SAIR ){
		
		switch(escolhaMenu){
			case MENU:
				escolhaMenu = Menu();
				break;
			case UM_JOGADOR:
				escolhaMenu = MENU;
				Gameplay(SINGLEPLAYER);
				break;
			case DOIS_JOGADORES:
				escolhaMenu = MENU;
				Gameplay(MULTIPLAYER);
				break;
			case MENU_UM_JOG:
				escolhaMenu = MenuUmJog();
				break;
			case MENU_DOIS_JOG:
				escolhaMenu = MenuDoisJog();
				break;
			case MENU_CLIENTE:
				escolhaMenu = MenuCliente();
				break;
			case MENU_SERVIDOR:
				escolhaMenu = MenuServidor();
				break;
		}	
	}
	
	minhaRede.EncerraWinSock();
	radioSpeed.LimpaNo(&radioSpeed);
	closegraph();
	return 0;	
}


// Menu de dois jogadores
EscolhaEmMenu MenuDoisJog(){
	
	EscolhaEmMenu escolha;

	minhaPg.Troca();
	minhaPg.Ativa();
	
	cleardevice();
	BackgroundMenu();
	

	botaoCliente.Show();
	botaoServidor.Show();	
	botaoVoltar.Show();
	outtextxy(botaoCliente.x + 32, botaoCliente.y + 72,"CLIENTE");
	outtextxy(botaoServidor.x + 24,botaoServidor.y + 72,"SERVIDOR");
	outtextxy(BOTAO_VOLTAR_X + 4,BOTAO_VOLTAR_Y + 24,"VOLTAR");

	minhaPg.Visual();
	
	escolha = SEM_ESCOLHA;
	while(escolha == SEM_ESCOLHA){
		
		if(botaoCliente.CheckClick() == true)
			escolha = MENU_CLIENTE;
			
		if(botaoServidor.CheckClick() == true)
			escolha = MENU_SERVIDOR;
			
		if(botaoVoltar.CheckClick() == true)
			escolha = MENU;
	}
		
	return escolha;
}



// Rotina de envio de soldados para outro jogador 
void EnviaSold(Jogador *meuJog, Jogador *outroJog, CampoJogo meuCampo){

	Soldado *novoIni;
	Soldado *pSold, *anterior;
	Soldado *soldado0;
	TDelay *tempoEspera;
	BarraVida meuHP;
	
	soldado0 = meuJog->soldado0;
	tempoEspera = &(outroJog->esperaIni);
	
	for(pSold = soldado0->prox; pSold != NULL; pSold = pSold->prox){
	
		if(pSold->vida > 0 && pSold->chegou != true){
			
			pSold->IA(meuCampo, tempoEspera);
			pSold->Show();
			meuHP.Show(pSold->x,pSold->y,pSold->vida,"soldado");
			
		} 
		
		else{
			
			anterior = pSold->Anterior(soldado0);
			
			if(pSold->chegou == true){
				
				pSold->Remove(anterior);
				outroJog->vida -= DANO_CHEGOU;
				outroJog->lider.Furia();
			}
			else{
				pSold->Remove(anterior);
				outroJog->dinheiro += BONUS_SOLD;
				
			}
			
			pSold = anterior;		
		}
	}
}

//=========================================================================



// Mostra uma mensagem conforme o tempo de jogo
void Avisa(TDelay gameTime, Lider Hitler){
	int gTimeInt;
	gTimeInt = gameTime.GameTime();
	
	setcolor(YELLOW);
	if(gTimeInt >= BEGIN && gTimeInt <= BEGIN + 2)
		Aviso(550,10,"INICIO DA BATALHA",YELLOW,Hitler);
		
	else if (gTimeInt >= ONDA1 && gTimeInt <= ONDA1 + 2)
		Aviso(480,30,"Cuidado! A� vem o primeiro soldado do Eixo", YELLOW,Hitler);
				
	else if(gTimeInt >= ONDA2 && gTimeInt <= ONDA2 + 2)
		Aviso(480,30,"Faltam 3 m para o ataque final do EIXO",YELLOW,Hitler);
	
	else if(gTimeInt >= ONDA5 && gTimeInt <= ONDA5 + 2)
		Aviso(480,30,"Faltam 2 m para o ataque final do EIXO",YELLOW,Hitler);

	else if(gTimeInt >= ONDA8 && gTimeInt <= ONDA8 + 2)
		Aviso(480,30,"Falta 1 m para o ataque final do EIXO",YELLOW,Hitler);

	else if(gTimeInt >= ONDA10 && gTimeInt <= ONDA10 + 2)
		Aviso(480,30,"� hora do ataque final do EIXO...",YELLOW,Hitler);

	else if(gTimeInt >= END && gTimeInt <= END + 2)
		Aviso(590,10,"Fim de Jogo.", YELLOW, Hitler);
}

/*Busca uma imagem com as informa��es passadas por par�metro*/
void GetImage(void** pImg, char path[], int width, int height){
	
	// L� e coloca na tela uma imagem
	readimagefile(path,0,0,width,height); 
	
	/// Calcula o tamanho da imagem com base na posi��o
	int size = imagesize(0,0,width,height);
	
	// Aloca mem�ria para a vari�vel que vai recebe-la
	*pImg = malloc(size);
	
	// Recebe a imagem
	getimage(0,0,width,height,*pImg); 
}


/*Mostra os l�ders/ avatares dos jogadores*/
void MostraLideres(Lider *meuLider, Lider *outroLider){
	
	BarraVida meuHP;

		
	meuLider->VerificaFuria();
	outroLider->VerificaFuria();
	
	meuLider->VerificaImg();
	outroLider->VerificaImg();
	
	meuLider->Show();
	outroLider->Show();
	
	if(meuLider->nome == "Stalin"){
		meuHP.Show(STALIN_X, STALIN_Y,*meuLider->vida,"lider");
		meuHP.Show(ROOSEVELT_X,ROOSEVELT_Y,*outroLider->vida,"lider");
	} else{
		meuHP.Show(ROOSEVELT_X,ROOSEVELT_Y,*meuLider->vida,"lider");
		meuHP.Show(STALIN_X, STALIN_Y,*outroLider->vida,"lider");
	}
}

/*Busca e retorna uma imagem com as informa��es passadas por par�metro*/
void* GetImage(char path[], int width, int height){
		
	void *pImg;
	readimagefile(path,0,0,width,height); 
	int size = imagesize(0,0,width,height);
	pImg = malloc(size);
	getimage(0,0,width,height,pImg); 
	return pImg;
}	


/*Texto e outros recursos gr�ficos mostrados pela fun��o Avisa*/
void Aviso(int posX, int posY, char * msg, int color, Lider hitler){
	settextjustify(LEFT_TEXT,TOP_TEXT);
	hitler.Show();
	setcolor(color);	
	outtextxy(posX,posY,msg);
}

/*Procedimento de defesa da torre*/
void DefesaTorre(Jogador *meuJog, Jogador *outroJog, Jogador *eixoIA, bool atira){
	Torre *torre0;
	Soldado *soldado0, *alvo;
	Torre *pTorre;
	
	torre0 = meuJog->torre0;
	
	for(pTorre = torre0->prox; pTorre != NULL;pTorre = pTorre->prox){
		
		alvo = NULL;
		pTorre->tipoAnimCanhao = 0;
		soldado0 = eixoIA->soldado0;
		
		alvo = pTorre->BuscaAlvo(soldado0);	
		
		if(alvo == NULL){
				
			soldado0 = outroJog->soldado0;
			
			alvo = pTorre->BuscaAlvo(soldado0);	
			if(alvo == NULL);
				pTorre->AnimacaoPatrulha();	
							
		}
			
		if(alvo != NULL){
			
			pTorre->AnimacaoMira(alvo);
			
			if(pTorre->reload.PassouDelay(TORRE_RELOAD)){
				pTorre->reload.Atualiza();
				pTorre->tipoAnimCanhao = 2;
				if(atira == true)
					pTorre->Atira(alvo);
			}	
		}
		
		pTorre->MostraTorre();	 	
	}
			
			
			
}
		




// Modo de um jogador
void Gameplay(TipoGameplay tipoGameplay){
	
	OndaEixo ondaVsMeuJog, ondaVsOutroJog;
	
	minhaPg.Troca();	// Troca a p�gina atual

	// Trabalha com a p�gina nos "bastidores"
	minhaPg.Ativa();
	
	cleardevice();
	
	// Atribui times aos jogadores
	meuJog.Init(ladoMeuJog,&gameSpeed);
	outroJog.Init(ladoOutroJog,&gameSpeed);
	eixoVsMeuJog.Init(LADO3,&gameSpeed);
	eixoVsOutroJog.Init(LADO3,&gameSpeed);
	
	// Inicializa gerenciador de ondas do eixo
	ondaVsMeuJog.Init(&eixoVsMeuJog,&gameSpeed,meuJog.lado);
	ondaVsOutroJog.Init(&eixoVsOutroJog,&gameSpeed,outroJog.lado);
	
	// Inicializa��o do campo de jogo a partir de um arquivo de coordenadas
	meuCampo.PosLoad("mapa05.txt");
	
	// Mostra campo de jogo
	meuCampo.Mostrar();
	
	// Deixa a p�gina visual
	minhaPg.Visual();
	
	// Come�a a contar o tempo de jogo
	gameTime.Atualiza();
	
	// M�sica do gameplay 
	PlaySound("../../Assets/Music/gameplay.wav",NULL,SND_LOOP | SND_ASYNC);	
		
	// Define o loop de jogo
	gameLoop = true;
			
	//Loop do jogo
	while(gameLoop == true){
	
		
		// Troca e ativa uma nova p�gina para modifica��es
		minhaPg.Troca();
		minhaPg.Ativa();
		
		// Limpa a tela
		cleardevice();
		
		// Mostra campo de jogo	
		meuCampo.Mostrar();	
		
		// Mostra gui do jogador
		meuJog.MostraGUI(); 
	
		// Verifica se � hora de enviar uma onda de soldados do Eixo
		onda = gameTime.SoldOnda();	
				
		// Verifica o tipo de envio de soldados do Eixo
		ondaVsMeuJog.Verifica(onda,meuCampo);

		// Avisa momentos importantes para o jogador
		Avisa(gameTime, eixoVsMeuJog.lider);
			
		// Verifica o input do usu�rio com a GUI
		meuJog.InputGUI ();
		
		// Executa procedimento de colocar torre
		meuJog.ArrastaTorre(meuCampo);
						
		// Rotina de defesa da torre 
		DefesaTorre(&meuJog,&outroJog,&eixoVsMeuJog,true);
		
		// Rotina de envio de soldados o jogador
		EnviaSold(&meuJog,&outroJog,meuCampo);
		
		// Rotina de envio de soldados do Eixo contra o jogador
		EnviaSold(&eixoVsMeuJog,&meuJog,meuCampo);
		
		// Mostra os lideres
		MostraLideres(&meuJog.lider,&outroJog.lider);
		
		// Simula a IA no Singleplayer 
		if(tipoGameplay == SINGLEPLAYER)
			IAOutroJog();
		
		//Ou recebe/envia dados no Multiplayer
		else{
			EnviaPacoteJogo();
			RecebePacoteJogo();
		}
			
		// Simula o comportamento do outro jogador	
		SimulaOutroJog(tipoGameplay,&ondaVsOutroJog);
		
		// Limpa campo de carregamento de imagens
		meuCampo.LimpaD();
							
		//Deixa a p�gina visual
		minhaPg.Visual();

	
		// Delay de FPS
		delay(FPS);	
	}
	// Libera a mem�ria
	meuCampo.LimpaMem();
	
	//Limpa mem�ria alocada para os soldados
	meuJog.soldado0->LimpaNo(meuJog.soldado0);
	outroJog.soldado0->LimpaNo(outroJog.soldado0);
	eixoVsMeuJog.soldado0->LimpaNo(eixoVsMeuJog.soldado0);
	eixoVsOutroJog.soldado0->LimpaNo(eixoVsOutroJog.soldado0);
	
	//Limpa mem�ria alocada para as torres
	meuJog.torre0->LimpaNo(meuJog.torre0);
	outroJog.torre0->LimpaNo(outroJog.torre0);
	eixoVsMeuJog.torre0->LimpaNo(eixoVsMeuJog.torre0);
	eixoVsOutroJog.torre0->LimpaNo(eixoVsOutroJog.torre0);
}

// Envia dados no modo multiplayer 
void EnviaPacoteJogo(){
	
	bool enviei;
	
	char pacote[30];
	strcpy(pacote,"");
	
	if(meuJog.qtdSoldEspera == 1){
		strcat(pacote,"NEW_SOLD|");
		meuJog.qtdSoldEspera = 0;
	}
	
	
	if(strcmp(pacote,"") == 0)
		strcpy(pacote,"_");
	
	if(minhaRede.clienteOuServidor == "cliente"){
	
		do{
			enviei = minhaRede.EnviaParaOServer(pacote);
		} while(enviei == false);

	}
		
	else if(minhaRede.clienteOuServidor == "servidor"){
		
		do{
			enviei = minhaRede.EnviaParaOClient(pacote);
		} while(enviei == false);
	}		
}


// Simula a IA no modo Singleplayer
void IAOutroJog(){
	
}

// Recebe dados no modo multiplayyer
void RecebePacoteJogo(){
	
	int i;
	char c;
	char temp[2];
	char buffer[10];
	TipoPacote tipoPacote;
	char pacote[30];
	
	
	bool recebi;
	
	if(minhaRede.clienteOuServidor == "cliente")
		recebi = minhaRede.RecebeDoServer();
		
	else if(minhaRede.clienteOuServidor == "servidor")
		recebi = minhaRede.RecebeDoClient();	
		
	if(recebi == true){
		
		c = minhaRede.pacote[0];
		i = 0;
		tipoPacote =  SEM_TIPO;
		strcpy(buffer,"");
		
		while(c != '\0'){
			
			if(c == '|' && tipoPacote == SEM_TIPO ){
				
				if(strcmp(buffer,"NEW_SOLD") == 0){
					outroJog.qtdSoldEspera = 1;
					cout << "NEW_SOLD\n"; // Teste

				}
					
				strcpy(buffer,"");
			
			} else if(c == '|' && tipoPacote != SEM_TIPO){
				
				strcpy(buffer,"");
				
			} else{
				temp[0] = c;
				temp[1] = '\0';
				strcat(buffer,temp);
			}
			
			i++;
			c = minhaRede.pacote[i];
		
		}
	}
}



// Simula o comportamento do outro jogador
void SimulaOutroJog(TipoGameplay tipoGameplay, OndaEixo *ondaVsOutroJog){
	
	if(outroJog.qtdSoldEspera > 0){
		
		if(outroJog.envioSold.PassouDelay(ESPERA_DELAY) == true){
			outroJog.envioSold.Atualiza();
			outroJog.soldado0->Insere(outroJog.soldado0,outroJog.lado,gameSpeed);
			outroJog.qtdSoldEspera--;
		}	
	}
	
	// Verifica a a onda atual para o envio de soldados do Eixo
	ondaVsOutroJog->Verifica(onda,meuCampo);
	
	// Envia soldados do jogador advers�rio contra o jogador atual
	EnviaSold(&outroJog,&meuJog,meuCampo);
	
	// Envia soldados nazistas contra o jogador advers�rio
	EnviaSold(&eixoVsOutroJog,&outroJog,meuCampo);

}





//=============================================================
// Menu de jogo
EscolhaEmMenu Menu(){

	
	EscolhaEmMenu escolha;
	int mouseX,mouseY;

	minhaPg.Troca();
	minhaPg.Ativa();
	
	BackgroundMenu();
	
	// Boto�es do menu
	botaoUmJog.Show();
	botaoDoisJog.Show(); 
	botaoCredit.Show();
	
	outtextxy(BOTAO1_X + 48  ,BOTAO1_Y + (TILE_H * 2), "UM"); 
	outtextxy(BOTAO1_X + 8 ,BOTAO1_Y + (TILE_W * 2) + 16, "JOGADOR"); 
	outtextxy(BOTAO2_X + 56  ,BOTAO2_Y + (TILE_H * 2), "DOIS"); 
	outtextxy(BOTAO2_X  + 16,BOTAO2_Y + (TILE_W * 2) + 16, "JOGADORES");
	outtextxy(BOTAO3_X + 8  ,BOTAO3_Y + 72 , "CR�DITOS"); 
			
	minhaPg.Visual(); 
	
	escolha = SEM_ESCOLHA;
	while(escolha == SEM_ESCOLHA){
		
		if( botaoUmJog.CheckClick() == true)  
			escolha = MENU_UM_JOG;
			
		if(botaoDoisJog.CheckClick() == true)
			escolha = MENU_DOIS_JOG;
	}
	
	return escolha;
}


// Menu de um jogador
EscolhaEmMenu MenuUmJog(){
	
	EscolhaEmMenu escolha;
	escolha = SEM_ESCOLHA;
	
	while(escolha == SEM_ESCOLHA){
	
		minhaPg.Troca();
		minhaPg.Ativa();
		
		cleardevice();
		
		// Desenha o background b�sico do menu
		BackgroundMenu();
				
		// Barras de contraste para os bot�es radio
		setfillstyle(1,BLACK);
		setcolor(BLACK);
		bar(TILE_W * 20, TILE_H * 10,TILE_W * 24,TILE_H * 11 );
		bar(TILE_W * 20, TILE_H * 12, TILE_W * 28, TILE_H * 13);
	
		// Mostra os bot�es 
		botaoJogar.Show();
		botaoVoltar.Show();
				
		// Mostra bot�es radio
		radioSpeed.MostraLista(&radioSpeed);
		radioLider.MostraLista(&radioLider);
		
		// Mostra o texto dos bot�es 
		outtextxy(TILE_W * 11, TILE_H * 12 + 16,"Lider:" );
		outtextxy(TILE_W * 11, TILE_H * 10 + 24, "Velocidade do jogo");
		outtextxy(BOTAO_JOGAR_X + 8,BOTAO_JOGAR_Y + 24,"JOGAR");
		outtextxy(BOTAO_VOLTAR_X + 4,BOTAO_VOLTAR_Y + 24,"VOLTAR");
		
		minhaPg.Visual();
		
		// Verifica clicks nos bot�es r�dio
		radioSpeed.VerificaClick(&radioSpeed);
		radioLider.VerificaClick(&radioLider);
		
		
		// --------------- Processamento do bot�o Jogar===================
		if(botaoJogar.CheckClick() == true){
			escolha = UM_JOGADOR;			
				
			if( radioLider.RadioChecked(&radioLider)->label == "Roosevelt"){
				ladoMeuJog = LADO1;
				ladoOutroJog = LADO2;
			} 
			else{
				ladoMeuJog = LADO2;
				ladoOutroJog = LADO1;
			}
				
				
			if(radioSpeed.RadioChecked(&radioSpeed)->label == "4")
				gameSpeed = 4;
			else
				gameSpeed = 8;
		}
		//========================== Fim do processamento==================	
	
		// Processamento do bot�o voltar
		if(botaoVoltar.CheckClick() == true){
			escolha = MENU;
		}
			
	}
	return escolha;
}

// Desenha o visual b�sico do menu de jogo
void BackgroundMenu(){
	
	
	settextjustify(LEFT_TEXT,CENTER_TEXT);
	// Modifica texto para o tamanho do logo
	settextstyle(BOLD_FONT,HORIZ_DIR,7);
	
	// Carrega o menu de jogo (imagem de fundo)
	meuCampo.PosLoad("menu.txt"); 
								
	meuCampo.Mostrar();		 
	 
	// Logo do jogo
	setcolor(GREEN);
	outtextxy(LOGO_X,LOGO_Y,"SEEK OF PEACE");
	setcolor(DARKGRAY);
	outtextxy(LOGO2_X,LOGO2_Y,"COLD WW2");
	setfillstyle(1,LIGHTGRAY);
	
	// Modifica texto para tamanho usual
	settextstyle(BOLD_FONT,HORIZ_DIR,1);
	setcolor(LIGHTGREEN);
	
}


// Menu do cliente
EscolhaEmMenu MenuCliente(){
	
	char nomeMeuLider[20];
	char liderDele[30];
	char velocidJogo[27];
	char buffer[15];
	TipoPacote tipoPacote;
	char c;
	int i;
	char temp[2];

	
	strcpy(nomeMeuLider, "Meu Lider: ");
	strcpy(liderDele, "Outro Lider: ");
	strcpy(velocidJogo, "Velocidade do Jogo: ");

	
	EscolhaEmMenu escolha;
	escolha = SEM_ESCOLHA;
	char ipEPorta[25];
	char charPorta[7];
	
	minhaRede.ClientInit(LOCALHOST,PORTA_PADRAO);
	
	while(escolha == SEM_ESCOLHA){
		
			
		minhaPg.Troca();
		minhaPg.Ativa();
		
		cleardevice();
		
		// Desenha o background b�sico do menu
		BackgroundMenu();
				
		// Barras de contraste para os bot�es radio
		setfillstyle(1,BLACK);
		setcolor(BLACK);
		bar(TILE_W * 20, TILE_H * 10,TILE_W * 27,TILE_H * 12 );
		
		
		// Informa��es do jogo
		if(minhaRede.clienteConectado == true){
			bar(TILE_W * 12, TILE_H * 14, TILE_W * 26, TILE_H * 17);
			setcolor(LIGHTBLUE);
			outtextxy(TILE_W * 12 + 8,TILE_H * 14 + 16,nomeMeuLider);
			setcolor(LIGHTRED);
			outtextxy(TILE_W * 12 + 8,TILE_H * 15 + 16 ,liderDele);
			setcolor(LIGHTGREEN);
			outtextxy(TILE_W * 12 + 8,TILE_H * 16 + 16 ,velocidJogo);	
		}

			

		
		botaoVoltar.Show();
		botaoJogar.Show();
		botaoConexao.Show();
		
		radioModoIP.MostraLista(&radioModoIP);
		
		strcpy(ipEPorta,LOCALHOST);
		strcat(ipEPorta,":");
		itoa(PORTA_PADRAO,charPorta,10);
		strcat(ipEPorta,charPorta);
		
		
		outtextxy(TILE_W * 11, TILE_H * 10 + 24, "IP / Porta do Servidor: ");
		outtextxy(TILE_W * 27 + 8, TILE_H * 10 + 24, ipEPorta);
		outtextxy(botaoJogar.x + 8,botaoJogar.y + 24,"JOGAR");
		outtextxy(botaoVoltar.x + 4,botaoVoltar.y + 24,"VOLTAR");
		
		if(minhaRede.clienteConectado == false)
			outtextxy(botaoConexao.x + 16,botaoConexao.y + 24,"CONECTAR");
		else
			outtextxy(botaoConexao.x + 16,botaoConexao.y + 24,"DESCONECTAR");
	
		//minhaGrd.Colocar();
		minhaPg.Visual();
		
		if(minhaRede.clienteConectado == false)
			radioModoIP.VerificaClick(&radioModoIP);
			
		if(botaoJogar.CheckClick() == true){
			if(minhaRede.clienteConectado == true){
				if(minhaRede.EnviaParaOServer("CLIENT_OK|true|") == true){
					if(minhaRede.RecebeDoServer() == true){
												
						c = minhaRede.pacote[0];
						i = 0;
						tipoPacote = SEM_TIPO;
						strcpy(buffer,"");	
						
						while(c != '\0'){
							
							if(c == '|' && tipoPacote == SEM_TIPO){
								
								if(strcmp(buffer,"SERVER_OK") == 0){
									tipoPacote = SERVER_OK;
								}
								
								strcpy(buffer,"");
								
							} else if(c == '|' && tipoPacote != SEM_TIPO){
								
								if(tipoPacote == SERVER_OK){
									if(strcmp(buffer,"true") == 0){
										
										minhaRede.servOk = true;
										escolha = DOIS_JOGADORES; 
										
									} 
									else if(strcmp(buffer,"false") == 0){
										minhaRede.servOk = false;
									}
								}
								strcpy(buffer,"");
								tipoPacote = SEM_TIPO;	
								
							} else{
								temp[0] = c;
								temp[1] = '\0';
								strcat(buffer,temp);
							}
							
							i++;
							c = minhaRede.pacote[i];	
						}
					}
			
				
				}		
			}
		}
		
		else if(botaoConexao.CheckClick() == true){
			
			if(minhaRede.clienteConectado == false){
				
				if(minhaRede.ConectaServer() == true){
					if(minhaRede.RecebeDoServer() == true){
					
						c = minhaRede.pacote[0];
						i = 0;
						tipoPacote = SEM_TIPO;
						strcpy(buffer,"");
						
						while(c != '\0'){
		
							if(c == '|' && tipoPacote == SEM_TIPO){
								
															
								if(strcmp(buffer,"LIDER") == 0){
									tipoPacote = LIDER_SERV;	
								}
								else if(strcmp(buffer,"GAMESPEED") == 0){
									tipoPacote = GAMESPEED;
								}
								
								strcpy(buffer,"");	

								
							} else if(c == '|' && tipoPacote != SEM_TIPO){
								
								if(tipoPacote == LIDER_SERV){
									strcat(liderDele,buffer);
									
									if(strcmp(buffer,"Stalin") == 0){
										strcat(nomeMeuLider,"Roosevelt");
										ladoMeuJog = LADO1;
										ladoOutroJog = LADO2;
									} else{
										strcat(nomeMeuLider,"Stalin");
										ladoMeuJog = LADO2;
										ladoOutroJog = LADO1;
									}
									
									
								} else if(tipoPacote == GAMESPEED){
									strcat(velocidJogo,buffer);
									if(strcmp(buffer,"4") == 0){
										gameSpeed = 4;
									} else{
										gameSpeed = 8;
									}
								}
								
								tipoPacote = SEM_TIPO;
								strcpy(buffer,"");		
							} 
							
							else{
								temp[0] = c;
								temp[1] = '\0';
								strcat(buffer,temp);	
							}
							
							i++;
							c = minhaRede.pacote[i];
						}	
						
					}
					
				}
				
			}
		
		}
		
		
		else if(botaoVoltar.CheckClick() == true){
			/// Talvez seja interessante enviar uma mensagem
			// para o servidor antes de fechar o socket
			escolha = MENU_DOIS_JOG;
			minhaRede.FechaSocketClient();
			delay(150);
		}
			
		
	}
	
	return escolha;

}

EscolhaEmMenu MenuServidor(){
	
	bool delayParaGUI = false;	
	char pacote[30], tempGmSpeed[2], *tempLider;
	char buffer[15];
	TipoPacote tipoPacote;
	char c;
	int i;
	char temp[2];
	
	// Inicializa as flags e o servidor
	minhaRede.FlagsInit();
	
	minhaRede.ServerInit(PORTA_PADRAO);
	
	EscolhaEmMenu escolha;
	escolha = SEM_ESCOLHA;
	
	
	while(escolha == SEM_ESCOLHA){
			
		minhaPg.Troca();
		minhaPg.Ativa();
		
		cleardevice();
		
		// Desenha o background b�sico do menu
		BackgroundMenu();
				
		// Barras de contraste para os bot�es radio
		setfillstyle(1,BLACK);
		setcolor(BLACK);
		bar(TILE_W * 20, TILE_H * 10,TILE_W * 24,TILE_H * 11 );
		bar(TILE_W * 20, TILE_H * 12, TILE_W * 28, TILE_H * 13);
		bar(TILE_W * 20, TILE_H * 14, TILE_W * 32, TILE_H * 17);
	
		// Mostra os bot�es 
		botaoJogar.Show();
		botaoVoltar.Show();
		botaoOpcaoServ.Show();
				
		// Mostra bot�es radio
		radioSpeed.MostraLista(&radioSpeed);
		radioLider.MostraLista(&radioLider);
		
		// Mostra o texto dos bot�es 
		outtextxy(TILE_W * 11, TILE_H * 12 + 16,"Lider:" );
		outtextxy(TILE_W * 11, TILE_H * 10 + 24, "Velocidade do jogo");
		outtextxy(BOTAO_JOGAR_X + 8,BOTAO_JOGAR_Y + 24,"JOGAR");
		outtextxy(BOTAO_VOLTAR_X + 4,BOTAO_VOLTAR_Y + 24,"VOLTAR");
		
		
		// Altera��o da op��o de conex�o
		if(minhaRede.clienteConectado == false){
			outtextxy(botaoOpcaoServ.x + 24,botaoOpcaoServ.y + 24, "ABRIR");
			outtextxy(botaoOpcaoServ.x + 8,botaoOpcaoServ.y + 40,"SERVIDOR");
		} else{
			outtextxy(botaoOpcaoServ.x + 24,botaoOpcaoServ.y + 24, "FECHAR");
			outtextxy(botaoOpcaoServ.x + 8,botaoOpcaoServ.y + 40,"SERVIDOR");
		}

		
		// Primeira mensagem do log
		if(minhaRede.servidorInit == true){
			setcolor(LIGHTGREEN);
			outtextxy(TILE_W * 20, TILE_H * 14 + 16, "- Click em \"Abrir Servidor\".");
		} else{
			setcolor(LIGHTRED);
			outtextxy(TILE_W * 20, TILE_H * 14 + 16, "- Falha na inicializa��o do servidor.");
		}
		
		
		// Segunda mensagem do log
		if(minhaRede.clienteConectado == true){
			setcolor(LIGHTBLUE);
			outtextxy(TILE_W * 20, TILE_H * 15 + 8, "- Cliente est� conectado!");
		} 
		
		minhaPg.Visual();
		
		
		// Verifica clicks nos bot�es r�dio (funcionam apenas antes do cliente se conectar)
		if(minhaRede.clienteConectado == false){
			radioSpeed.VerificaClick(&radioSpeed);
			radioLider.VerificaClick(&radioLider);
		}

		
		
		// --------------- Processamento do bot�o Jogar===================
		if(botaoJogar.CheckClick() == true){
			
			if(minhaRede.clienteOk == false){
				outtextxy(TILE_W * 20, TILE_H * 16 + 8, "- Aguardando resposta do cliente...");
				if(minhaRede.RecebeDoClient() == true){
						c = minhaRede.pacote[0];
						i = 0;
						tipoPacote = SEM_TIPO;
						strcpy(buffer,"");
						
						
						while( c != '\0' ){
							
							if( c == '|' && tipoPacote == SEM_TIPO){
								
								if(strcmp("CLIENT_OK",buffer) == 0){
									
									tipoPacote = CLIENT_OK;
								}
								
								strcpy(buffer,"");
								
							} else if(c == '|' && tipoPacote != SEM_TIPO){
								
								if(tipoPacote == CLIENT_OK){
									
									if(strcmp("true",buffer) == 0){
										minhaRede.clienteOk = true;
									} else if(strcmp("false",buffer) == 0){
										minhaRede.clienteOk = false;
									}
								}
								
								strcpy(buffer,"");
								tipoPacote = SEM_TIPO;
							}
							else{
								temp[0] = c;
								temp[1] = '\0';
								strcat(buffer,temp);
							}
							
							i++;
							c = minhaRede.pacote[i];
							
						}						
				}
				if(minhaRede.clienteOk == true){
					minhaRede.servOk = true;
					minhaRede.EnviaParaOClient("SERVER_OK|true|");
					escolha = DOIS_JOGADORES;
				}
			}
		}
	
		//===============Processamento do bot�o voltar=================
		if(botaoVoltar.CheckClick() == true){
			escolha = MENU_DOIS_JOG;
			minhaRede.FechaSocketServer();
			delay(100); // Delay para evitar duplo click
		}
		
		if(delayParaGUI == true){
			delay(125);
			delayParaGUI = false;
		}
		
		// ===============Processamento do bot�o Abrir servidor===============
		if(botaoOpcaoServ.CheckClick() == true){
			
			if(minhaRede.clienteConectado == true)
				minhaRede.FechaConexaoClient();	
		
			else if(minhaRede.clienteConectado == false){
			
				outtextxy(TILE_W * 20, TILE_H * 14 + 16, "_________Servidor aberto_________");
				setcolor(YELLOW);
				outtextxy(TILE_W * 20, TILE_H * 15 + 8, "- Aguardando cliente...");
			
				if(minhaRede.AceitaConexaoClient() == true){
					
					strcpy(tempGmSpeed ,radioSpeed.RadioChecked(&radioSpeed)->label);
					gameSpeed = atoi(tempGmSpeed);
					tempLider = radioLider.RadioChecked(&radioLider)->label; 
					
					if( tempLider == "Roosevelt"){
						ladoMeuJog = LADO1;
						ladoOutroJog = LADO2;
					} 
					else{
						ladoMeuJog = LADO2;
						ladoOutroJog = LADO1;
					}
					

					strcpy(pacote,"LIDER|");
					strcat(pacote,tempLider);
					strcat(pacote,"|GAMESPEED|");
					strcat(pacote,tempGmSpeed);
					strcat(pacote,"|");									
					minhaRede.EnviaParaOClient(pacote);
					delayParaGUI = true;
				}
			}

		}
	}
	return escolha;	
}


