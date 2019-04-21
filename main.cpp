#include "agenda.hpp"


int main() {
	int num_mes, num_dia;
	string meses[12] = {"Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};
	
  	Mes *mes_atual;
	Agenda *calendario = (Agenda*)calloc(1, sizeof(Agenda));
	
	inicia_agenda(&calendario);
	carrega_agenda(&calendario);
  	
  	cout << "---------- SEJA BEM VINDO(A) A SUA AGENDA ----------" << endl;	
	num_mes = menu_abrir_agenda(); //Irá informar qual o mes a ser aberto
  	while(num_mes > 0){
		num_dia = abrir_dia_mes(num_mes); //irá informar qual o dia a ser considerado
		while (num_dia > 0) {
			busca_mes(&calendario->lista_mes->prox, &mes_atual, meses[num_mes-1]); //Pega o mes em questao na lista de meses
			realiza_operacoes_agenda(&mes_atual, num_dia - 1); //Faz as operações de agenda naquele dia até que seja solicitado a troca de dia

			num_dia = abrir_dia_mes(num_mes); //irá informar qual o dia a ser considerado
		}
		num_mes = menu_abrir_agenda(); //Irá informar qual o mes a ser aberto
  	}

  	salva_agenda_arquivo(calendario);
  	fecha_agenda(calendario);
}
