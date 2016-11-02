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

int semaphore = 1;
void *passageiro(void *tid){
	int p = (int) tid;

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
	turnoCarro[c] = __sync_fetch_and_add(&numeroCarro,1);
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
		pthread_join(&carros[i],NULL);
	}
	for(i = 0; i < N_PASSAGEIROS; ++i){
		pthread_join(&passageiros[i],NULL);
	}
	printf("N_PASSAGEIROS: %d\n", N_PASSAGEIROS);
	printf("M_CARROS: %d\n", M_CARROS);
	printf("CAPACIDADE: %d\n", CAPACIDADE);
	printf("MAX_VOLTAS: %d\n", MAX_VOLTAS);
	printf("Terminou.\n");
}