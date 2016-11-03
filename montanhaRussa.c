#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N_PASSAGEIROS 3
#define M_CARROS 1
#define CAPACIDADE 3
#define MAX_VOLTAS 1
void P(int* s) {
	while(!__sync_lock_test_and_set(s,0));
}
void V(int* s) {
	__sync_fetch_and_add(s,1);
}

//ALUSOES
//funcoes a respeito do passageiro
void *passageiro(void *tid);
void esperaVez(int p);
void entraNoCarro(int p);
void esperaVoltaAcabar(int p);
void saiDoCarro(int p);
void passeiaPeloParque(int p); //tempo aleatorio
//funcoes a respeito do carro
void *carro(void *tid);
void esperaEncher(int c);
void esperaMontanhaRussa(int c);
void daUmaVolta(int c); //tempo aleatorio
void esperaEsvaziar(int c);

//ticket passageiro
int turno[N_PASSAGEIROS];
int ticket = 1;
int prox = 1;
//ticket carro
int turnoC[M_CARROS];
int ticketC = 1;
int proxC = 1;
//semaforo
int s1 = 1;
int s2 = 1;
int filaEmbarcar = 0;
int encheuCarro = 0;
int filaDesembarcar = 0;
int esvaziouCarro = 0;
int passageirosABordo = 0;
int passageirosDesembarcados = 0;
void *passageiro(void *tid){
	int p = (int) tid;
	printf("%d - passageiro iniciou\n", p);
	//INICIO DO TICKET
	// turno[p] = __sync_fetch_and_add(&ticket,1);
	// while(turno[p] != prox);
	//P(&filaEmbarcar);
	while(!filaEmbarcar);
	//embarca
	printf("%d - passageiro embarcando\n", p);
	P(&s1);
		if(passageirosABordo == CAPACIDADE - 1){
			filaEmbarcar = 0;
			printf("%d - passageiro foi o último a entrar\n", p);
			V(&encheuCarro);
			passageirosABordo = 0;
		}else{
			passageirosABordo += 1;
		}
	V(&s1);
	while(!filaDesembarcar);
	//desembarca
	P(&s2);
		printf("%d - passageiro desembarcando\n", p);
		if(passageirosDesembarcados == CAPACIDADE - 1){
			filaDesembarcar = 0;
			printf("%d - passageiro foi o último a descer\n", p);
			V(&esvaziouCarro);
			passageirosDesembarcados = 0;
		}else{
			passageirosDesembarcados += 1;
		}
	V(&s2);
	//FIM DO TICKET
	// prox++;
	/*while(voltas < MAX_VOLTAS){
		turno[p] = __sync_fetch_and_add(&numero,1);
		esperaVez(p);
		entraNoCarro(p);
		esperaVoltaAcabar(p);
		saiDoCarro(p);
		passeiaPeloParque(p);
	}*/
	return (void *) p;
}
void *carro(void *tid){
	int c = (int) tid;
	//INICIO DO TICKET
	// turnoC[c] = __sync_fetch_and_add(&ticketC,1);
	// while(turnoC[c] != proxC);
	//carregando passageiros
	printf("%d - carro esperando passageiros embarcarem\n", c);
	V(&filaEmbarcar);
	P(&encheuCarro);
	printf("%d - carro dando volta\n", c);
	//descarrega
	printf("%d - carro esperando passageiros desembarcarem\n", c);
	V(&filaDesembarcar);
	P(&esvaziouCarro);
	//FIM DO TICKET
	// proxC++;
	/*while(passageiroEsperando > 0){
		esperaEncher(c);
		esperaMontanhaRussa(c);
		daUmaVolta(c);
		esperaEsvaziar(c);
		voltas++;
	}*/
	return (void *) c;
}
int main(int argc, char *argv[]){
	pthread_t passageiros[N_PASSAGEIROS];
	pthread_t carros[M_CARROS];
	int erro, i;
	long pid, cid;
	srand(time(NULL));
	printf("Comecou\n");
	printf("N_PASSAGEIROS: %d\n", N_PASSAGEIROS);
	printf("M_CARROS: %d\n", M_CARROS);
	printf("CAPACIDADE: %d\n", CAPACIDADE);
	printf("MAX_VOLTAS: %d\n", MAX_VOLTAS);
	for(i = 0; i < M_CARROS; ++i){
		pthread_create(&carros[i],NULL,carro,(void *)i);
	}
	for(i = 0; i < N_PASSAGEIROS; ++i){
		pthread_create(&passageiros[i],NULL,passageiro,(void *)i);
	}
	for(i = 0; i < M_CARROS; ++i){
		pthread_join(carros[i],NULL);
	}
	for(i = 0; i < N_PASSAGEIROS; ++i){
		pthread_join(passageiros[i],NULL);
	}
	printf("N_PASSAGEIROS: %d\n", N_PASSAGEIROS);
	printf("M_CARROS: %d\n", M_CARROS);
	printf("CAPACIDADE: %d\n", CAPACIDADE);
	printf("MAX_VOLTAS: %d\n", MAX_VOLTAS);
	printf("Terminou.\n");
}