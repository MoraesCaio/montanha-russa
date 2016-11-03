#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N_PASSAGEIROS 6
#define M_CARROS 2
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

//ticket embarque
int turnoEmb[M_CARROS];
int ticketEmb = 1;
int proxEmb = 1;
//ticket carro
int turnoDes[M_CARROS];
int ticketDes = 1;
int proxDes = 1;
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
	P(&filaEmbarcar);
	printf("%d - passageiro embarcando no carro %d\n", p, proxEmb);
	P(&s1);
		if(passageirosABordo == CAPACIDADE - 1){
			filaEmbarcar = 0;
			printf("%d - passageiro foi o último a entrar no carro %d\n", p, proxEmb);
			V(&encheuCarro);
			passageirosABordo = 0;
		}else{
			passageirosABordo += 1;
			V(&filaEmbarcar);
		}
	V(&s1);

	P(&filaDesembarcar);
	//desembarca
	P(&s2);
		printf("%d - passageiro desembarcando do carro %d\n", p, proxDes);
		if(passageirosDesembarcados == CAPACIDADE - 1){
			filaDesembarcar = 0;
			printf("%d - passageiro foi o último a descer do carro %d\n", p, proxDes);
			V(&esvaziouCarro);
			passageirosDesembarcados = 0;
		}else{
			passageirosDesembarcados += 1;
			V(&filaDesembarcar);
		}
	V(&s2);
	//FIM DO TICKET
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
	turnoEmb[c] = __sync_fetch_and_add(&ticketEmb,1);
	while(turnoEmb[c] != proxEmb);
	printf("%d - carro esperando passageiros embarcarem\n", c);
	V(&filaEmbarcar);
	P(&encheuCarro);
	proxEmb++;

	printf("%d - carro dando volta\n", c);

	//descarrega
	turnoDes[c] = __sync_fetch_and_add(&ticketDes,1);
	while(turnoDes[c] != proxDes);
	printf("%d - carro esperando passageiros desembarcarem\n", c);
	V(&filaDesembarcar);
	P(&esvaziouCarro);
	printf("%d - carro terminou sua vez\n", c);
	proxDes++;
	//FIM DO TICKET
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