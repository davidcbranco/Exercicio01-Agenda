#include "agenda.hpp"

int qtd_dias_meses[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
string nomes_meses[12] = {"Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

/*
	**************************************************
	******* Funções do funcionamento da agenda *******
	**************************************************
*/

void carrega_agenda(Agenda *calendario){
	int hora, min, indice;
	string descricao;
	Mes *mes_atual;
	ifstream entrada("historico.txt");
	string linha;
	if(entrada.is_open()){
		while ( getline (entrada,linha) ) {
			if(linha.front() == '-'){ //Char que identifica o mês
				linha.erase (linha.begin());
				busca_mes(&calendario->lista_mes->prox, &mes_atual, linha); //Pega o mes em questao na lista de meses
			
			}
			else {
				if(linha.front() == '#'){ //Char que identifica o dia
					linha.erase (linha.begin());
					indice = atoi(linha.c_str());
				}
				else{ // Se entrar aqui, significa que é um compromisso 
					hora = atoi(linha.substr(0,2).c_str());
					min = atoi(linha.substr(3,2).c_str());
					descricao = linha.substr(6);
	       			mes_atual->lista_dias[indice-1].num_comp++;
					insere_lista_comp(&mes_atual->lista_dias[indice-1].lista_compromissos, hora, min, descricao);
				} 		
			}
		}
		entrada.close();
	}
	else{ //Se o arquivo não existe, então não será feito nada.
		return;
	}
	entrada.close();
}

void salva_agenda_arquivo(Agenda *calendario){
	ofstream saida ("historico.txt");
	Mes *mes_aux = calendario->lista_mes;
	Compromisso *comp_aux;
	while(mes_aux->prox != NULL){ //Percorre todos os meses
		saida << "-" << mes_aux->prox->nome << endl;
		for (int i = 0; i < mes_aux->prox->total_dias; i++) { //Para cada dia do mes
			if(mes_aux->prox->lista_dias[i].num_comp > 0){ //Se o dia nao for vazio de compromissos, irá imprimir
				comp_aux = mes_aux->prox->lista_dias[i].lista_compromissos->prox;
				saida << "#" << i+1 << endl;
				while(comp_aux != NULL){ //percorre os compromissos
					saida << setfill('0') << setw(2) << comp_aux->hora << ":" << setw(2) << comp_aux->minutos << " " << comp_aux->descricao << endl;
					comp_aux = comp_aux->prox;
				}

			}
		}
		mes_aux = mes_aux->prox;
	}
	saida.close();
}

int menu_abrir_agenda(){ //Dado um mês, retorna o nó da lista de meses que o representa
	int num_mes;
	cout << "Digite o numero do mes para abrir a agenda ou digite 0 para sair" << endl;
	cin >> num_mes;
	
	while(num_mes < 0 || num_mes > 12) { //Se estiver fora do range dos meses, pede um valor valido
		cout << "Mes invalido, digite um numero entre 1 e 12" << endl;
		cin >> num_mes;
	}

	if (num_mes == 0) {
		return 0;
	}

	cout << "Mes selecionado: " << nomes_meses[num_mes-1] << endl;

	return num_mes;
}

int abrir_dia_mes(int num_mes){ //Função que verifica se o dia selecionado esta dentre do limite numerico dos dias daquele mês
	int num_dia;
	cout << "Digite o numero do dia para abrir a lista de compromissos ou digite 0 para fechar a agenda do mes" << endl;
	cin >> num_dia;
	
	while(num_dia < 0 || num_dia > qtd_dias_meses[num_mes-1]) { //Se estiver fora do range dos dias do mes, pede um valor valido
		cout << "Dia invalido, digite um numero entre 1 e "<< qtd_dias_meses[num_mes-1] << endl;
		cin >> num_dia;
	}

	if (num_dia == 0) {
		return 0;
	}

	cout << "Dia selecionado: " << num_dia << endl;

	return num_dia;
}

void realiza_operacoes_agenda(Mes **m, int indice){ //Função que irá coordenar as funções da agenda 
	int opcao, hora, min;
	char c;
	char descricao[200];
	do { // Este loop rodara infinitamente até que o usuário escolha a opção de sair deste dia.
		 // Sempre que escolher uma ação ela voltará para este menu mantendo o dia escolhido ou até selecionar a opção de troca-lo
		cout << "Escolha uma opcao" << endl;
	  	cout << "\t1 - Inserir Compromisso"<< endl;
	  	cout << "\t2 - Remover Compromisso"<< endl;
	  	cout << "\t3 - Listar Compromissos"<< endl;
	  	cout << "\t4 - Trocar o dia"<< endl;
	 
	 	cin >> opcao;

	 	switch ( opcao ) {
	        case 1:
	        	cout << "---Digite a hora do compromisso: ";
	        	cin >> hora >> c >> min;
	        	cout << "---Digite a descricao do compromisso: ";
	       		cin.ignore();
				cin.getline(descricao,200);
		 	  	insere_lista_comp(&(*m)->lista_dias[indice].lista_compromissos, hora, min, descricao);
	       		(*m)->lista_dias[indice].num_comp++;
	       		break;
	        case 2:
	        	cout << "---Digite a hora do compromisso: ";
	        	cin >> hora >> c >> min;
	        	deleta_lista_comp(&(*m)->lista_dias[indice].lista_compromissos, hora, min);
	       		(*m)->lista_dias[indice].num_comp--;
	       		break;
	        case 3:
	        	if ((*m)->lista_dias[indice].num_comp == 0) {
	        		cout << "Sem compromissos neste dia." << endl; 
	        	}
	        	else{
	        		lista_comps((*m)->lista_dias[indice].lista_compromissos->prox);
	       		}
	       		break;
	        case 4: // Caso o usuário queira trocar de dia
	        		// essa funcão irá retornar para a main onde irá pedir para o usuário selecionar um novo dia daquele mes 
	       		return;
	        default:
	        	cout << "Opcao invalida" << endl;
	        	break;
	    }
  	}while(true);
}

void inicia_agenda(Agenda *calendario){ //Função que inicializa a lista de meses
	
	calendario->lista_mes = (Mes*)calloc(1, sizeof(Mes));
	calendario->lista_mes->prox = NULL;

	for (int i = 0; i < 12; ++i) {
		insere_lista_mes(&calendario->lista_mes->prox, i);
	}
}

Mes* inicia_mes(string nome, int total_dias){ //Fuñção que aloca o vetor de dias e inicializa cada uma de suas posições
	Mes *m = (Mes*)calloc(1, sizeof(Mes));
	m->nome = nome;
	m->total_dias = total_dias;
	m->lista_dias = (Dia*)calloc(total_dias, sizeof(Dia));
	for (int i = 0; i < total_dias; i++) {
		m->lista_dias[i] = inicia_dia();
	}
	m->prox = NULL;

	return m;
}

Dia inicia_dia(){ //Função que inicializa o dia do mes
	Dia d;
	d.num_comp = 0;
	d.lista_compromissos = inicia_compromisso(0, 0, " "); //Lista com celula cabeça, ou seja, a primeira celula é vazia mas não nula.

	return d;
}

Compromisso* inicia_compromisso(int hora, int min, string desc){ // inicia um compromisso atribuindo as informaçoes de hora e descrição digitadas pelo usuário
	Compromisso *c = (Compromisso*)calloc(1, sizeof(Compromisso));
	c->hora = hora;
	c->minutos = min;
	c->descricao = desc;
	c->prox = NULL;

	return c;
}

/*
	*************************************************
	***** Funções a respeito da estrutura lista *****
	*************************************************
*/

void fecha_agenda(Agenda *calendario){
	Mes *mes_aux = calendario->lista_mes->prox; 
	Compromisso *comp_aux;
	while(mes_aux != NULL){
		for (int i = 0; i < mes_aux->total_dias; i++) {
			comp_aux = mes_aux->lista_dias[i].lista_compromissos->prox;
			while(comp_aux != NULL){
				comp_aux = comp_aux->prox;
				free(mes_aux->lista_dias[i].lista_compromissos->prox);
				mes_aux->lista_dias[i].lista_compromissos->prox = comp_aux;
			}
			
			free(mes_aux->lista_dias[i].lista_compromissos);
		}
		free(mes_aux->lista_dias);
		mes_aux = mes_aux->prox;
		free(calendario->lista_mes->prox);
		calendario->lista_mes->prox = mes_aux;	
	}
	free(calendario->lista_mes->lista_dias);
	free(calendario->lista_mes);
}

void insere_lista_mes(Mes **m, int indice){
	if (*m == NULL) { //caso a posição seja null, irá inicializar o mes
        (*m) = inicia_mes(nomes_meses[indice], qtd_dias_meses[indice]);
        return;
    }
    else { //se não for NULL, irá chamar recursivamente a função para avaliar o ponteiro prox
        insere_lista_mes(&(*m)->prox, indice);
    }
}

void busca_mes(Mes **m, Mes **novo, string nome){
	if ((*m)->nome == nome) { //caso tenha encontrado o mês, o armazena na variável passada
        (*novo) = *m;
        return;
    }
    else { //irá chamar recursivamente a função para avaliar o ponteiro prox até encontrar o mês
        busca_mes(&(*m)->prox, &(*novo), nome);
    }
}

void insere_lista_comp(Compromisso **c, int hora, int min, string desc){
	if ((*c)->prox == NULL) { //caso a posição seja null, irá inicializar o mes
        (*c)->prox = inicia_compromisso(hora, min, desc);
        
        return;
    }
    
    if ((*c)->prox->hora == hora && (*c)->prox->minutos == min) { //caso já exista um compromisso naquela hora, não irá inserir.
    	cout<< "Nao foi possível inserir pois ja existe um compromisso neste horario."<<endl;
    	return;
    }
    
    if ((*c)->prox->hora > hora) { //if para manter os compromissos ordenados crescentemente por hora
    	Compromisso *aux = inicia_compromisso(hora, min, desc);
	    aux->prox = (*c)->prox; 
	    (*c)->prox = aux;
    }
    else if((*c)->prox->hora == hora && (*c)->prox->minutos > min){ //if para manter os compromissos ordenados crescentemente por minutos em horas iguais
    	Compromisso *aux = inicia_compromisso(hora, min, desc);
	    aux->prox = (*c)->prox; 
	    (*c)->prox = aux;
    }
    else { //se não for NULL, irá chamar recursivamente a função para avaliar o ponteiro prox
        insere_lista_comp(&(*c)->prox, hora, min, desc);
    }
}

void deleta_lista_comp(Compromisso **c, int hora, int min){
	if ((*c)->prox == NULL) { //caso a posição seja null, nao existe compromisso no horario informado
        cout << "Compromisso nao encontrado" <<endl;
        return;
    }
	if ((*c)->prox->hora == hora && (*c)->prox->minutos == min) { //caso a ache o compromisso, irá deleta-lo
        Compromisso *aux = (*c)->prox;
        (*c)->prox = aux->prox;
        free(aux);
        return;
    }
    else { //se não encontrou, irá chamar recursivamente a função para avaliar o ponteiro prox
        deleta_lista_comp(&(*c)->prox, hora, min);
    }
}

void lista_comps(Compromisso *c){ //função que imprime a lista de compromissos
	if (c == NULL) {
        return;
    }
    cout << "Hora: " << setfill('0') << setw(2) << c->hora << ":" << setw(2) << c->minutos << " Descricao: " << c->descricao << endl;

    lista_comps(c->prox);
}
