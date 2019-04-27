#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;

struct Compromisso {
	string descricao;
	int hora, minutos;
	Compromisso *prox;	
};

struct Dia {
	int num_comp;
	Compromisso *lista_compromissos;
};

struct Mes {
	string nome;
	int total_dias;
	Mes *prox;
	Dia *lista_dias;
};

struct Agenda{
	Mes *lista_mes;
};

//Funções da Agenda
Dia inicia_dia(); // Inicia o elemento dia
Mes* inicia_mes(string, int); // Inicia o elemento mês
Compromisso* inicia_compromisso(int, int, string); // Inicia o elemento compromisso

int menu_abrir_agenda(); // Função que retorna o mes escolhido pelo usuário
int abrir_dia_mes(int num_mes); // Função que retorna o dia escolhido pelo usuário

void inicia_agenda(Agenda*); // Função que inicia a estrutura da agenda
void carrega_agenda(Agenda*); // Função que irá ler o arquivo e salvar as informaçõoes nele
void realiza_operacoes_agenda(Mes**, int); // Função que irá pegar a opção digitada pelo usuário e chamar as funções para realiza-la
void salva_agenda_arquivo(Agenda *calendario); // Função que irá imprimir toda a agenda no arquivo de saída

//Funções de Lista
void busca_mes(Mes**, Mes**, string); // Busca um mês na lista de meses
void deleta_lista_comp(Compromisso **c, int hora, int min); // Deleta um compromisso na lista de compromissos
void fecha_agenda(Agenda *calendario); // Função que irá dar free em todas as estruturas da agenda no arquivo de saída
void insere_lista_comp(Compromisso **c, int hora, int min, string desc); // Insere um compromisso na lista de compromissos
void insere_lista_mes(Mes **m, int indice); // Insere um mes na lista de meses
void lista_comps(Compromisso *c); // Imprime todos os compromissos daquele dia.
