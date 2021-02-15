#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


#define N_LEITORES 7
#define N_ESCRITORES 1

sem_t mutex,mutex_e,escr,leit,fila;//todos iniciados em 1
pthread_mutex_t mutex,mutex_e;
int leitores = 0,fila_e = 0;




void* leitor(void* args)
{
	int id = *(int*) args;
	printf("Leitor %d iniciado\n",id);
	while(1){

	//bloqueio
	sem_wait(&fila);
	sem_post(&fila);
	
	sem_wait(&leit);
	sem_post(&leit);
	
	sem_wait(&mutex);
	leitores++;

	if(leitores == 1) sem_wait(&escr);
	sem_post(&mutex);
	
	
	printf("Leitor %d esta lendo\n",id);for(int i = 0; i< 10000;i++){int j = i - 3;}
	printf("Leitor %d terminou\n",id);
	sem_wait(&mutex);
	leitores--;
	if(leitores == 0) sem_post(&escr);
	sem_post(&mutex);
	}pthread_exit(NULL);
}

void* escritor(void* args)
{
	int id = *(int*) args;
	printf("Escritor %d iniciado\n",id);
	while(1)
	{
		sem_wait(&mutex_e);
		fila_e++;
		sem_post(&mutex_e);
		if(fila_e==1) sem_wait(&fila);

		//bloqueio
		sem_wait(&leit);
		sem_wait(&escr);
		
		printf("Escritor %d esta escrevendo\n",id);for(int i = 0; i < 10000; i++){int j = 2*i;}
		printf("Escritor %d teriminou\n",id);
		sem_wait(&mutex_e);
		fila_e--;
		sem_post(&mutex_e);
		if(fila_e == 0) sem_post(&fila);
		sem_post(&escr);
		sem_post(&leit);
	}
}


int main()
{
	int l_ids[N_LEITORES],e_ids[N_ESCRITORES];
	pthread_t ids[N_LEITORES + N_ESCRITORES];
	sem_init(&mutex,0,1);
	sem_init(&mutex_e,0,1);
	sem_init(&leit,0,1);
	sem_init(&escr,0,1);
	sem_init(&fila,0,1);
	for(int i = 0; i < N_LEITORES;i++)
	{
		l_ids[i] = i;
		pthread_create(&ids[i],NULL,leitor,(void*)&l_ids[i]);
	}
	for(int i = 0; i < N_ESCRITORES; i++)
	{
		e_ids[i] = i;
		pthread_create(&ids[N_LEITORES + i],NULL,escritor,(void*)&e_ids[i]);
	}
	for(int i = 0; i < N_ESCRITORES + N_LEITORES;i++) pthread_join(ids[i],NULL);
	return 0;
}





















