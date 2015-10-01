#include <iostream>
#include <fstream> // E / S de arquivos
#include <iomanip> // Para ler em caracter a caracter
#include <graphics.h>

// Bibliotecas criados pela equipe de desenvolvimento
#include "..\..\header\td_functions.h"
#include "..\..\header\td_defines.h"
#include "..\..\header\sprite.h"
#include "..\..\header\campojogo.h"
#include "..\..\header\grade.h"
#include "..\..\header\soldado.h"
#include "..\..\header\pagina.h"

using namespace std;


int main(){
	
	// Declara��o de vari�veis
	CampoJogo meuCampo;
	Pagina minhaPg;
	bool gameLoop = true;
	Soldado meuSold; 
	int x, y; // Op��o do jogador para movimenta��o
	
	// Inicialize a janela gr�fica
	initwindow(TELA_W,TELA_H);
	
	// Inicializa a estrutura p�gina
	minhaPg.Init();
	
	// Troca a p�gina atual
	minhaPg.Troca();

	// Trabalha com a p�gina nos "bastidores"
	minhaPg.Ativa();
	
	// Inicializa��o do campo de jogo a partir de um arquivo de coordenadas
	meuCampo.Init("mapa01.txt");
	
	// Mostra campo de jogo
	meuCampo.Mostrar();
	
	// Inicializa o soldado
	meuSold.Init("Chara");
	
	// Mostra soldado
	meuSold.Show();	
	
	// Deixa a p�gina visual
	minhaPg.Visual();
	

	
	//Loop do jogo
	while(gameLoop == true){
		
		// Troca e ativa uma nova p�gina para modifca��es nos gr�ficos
		minhaPg.Troca();
		minhaPg.Ativa();
		
		// Limpa a tela
		cleardevice();
		
		// Mostra campo de jogo
		meuCampo.Mostrar();
	
		/// Teste - Move-se at� o destino especificado
		cout << endl << "x = " << meuSold.x << " | y = " << meuSold.y;
	if (meuSold.MovUntil(meuSold.X + (TILE_W * 1) , meuSold.Y - (TILE_H * 2))){
	/*	if(meuSold.MovUntil(120,800)){*/
		
			meuSold.MovUntil(meuSold.x, 64 );
		}
		
		// Mostra soldado
		meuSold.Show();
		
		//Deixa a p�gina visual
		minhaPg.Visual();
				
		// Delay de FPS
		delay(FPS);
		
	}
	closegraph();
	return 0;	
}