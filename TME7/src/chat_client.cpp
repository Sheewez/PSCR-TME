#include <chat_common.h>

int monExit = 0;
struct myshm* shm_client, * shm_pere;

void* reader(void* arg){
	while(!monExit){
		sem_wait(&(shm_client->sem));

		if(shm_client->read != shm_client->write || shm_client->nb == MAX_MESS){

			struct message message = shm_client->messages[shm_client->read];
			shm_client->read = (shm_client->read + 1) % MAX_MESS;
			printf("%s", message.content);
			shm_client->nb--;
		}
		sem_post(&(shm_client->sem));
	}
	pthread_exit(NULL);
	return NULL;
}

void* writer(void*arg){
	while(!monExit){
		struct message msg;
		msg.type = 1;
		fgets(msg.content, TAILLE_MESS, stdin);

		if(monExit)break;
		sem_wait(&(shm_pere->sem));
		while(shm_pere->nb == MAX_MESS){
			sem_post(&(shm_pere->sem));
			sleep(1);
			sem_wait(&(shm_pere->sem));
		}
		shm_pere->messages[shm_pere->write] = msg;
		shm_pere->write = (shm_pere->write + 1) % MAX_MESS;
		shm_pere->nb++;
		sem_post(&(shm_pere->sem));
	}
	pthread_exit(NULL);
	return NULL;
}

void exithandler(int sig){
	exit = sig;
}

int main(int argc,char*argv[]){
	char*shm_client_name,*shm_pere_name;
	int shm_client_id, shm_pere_id;
	pthread_t tids[2];
	struct message msg;
	struct sigaction action;
	if(argc <= 2) {
		fprintf(stderr, "Usage: %s id_client id_server\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	printf("Connexion à %s sous l’identité %s\n", argv[2], argv[1]);

	action.sa_handler = exithandler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT, &action, 0);
	shm_client_name = (argv[1]);
	shm_pere_name = (argv[2]);

	if((shm_client_id = shm_open(shm_client_name, O_RDWR | O_CREAT, 0666)) == -1) {
		perror("shm_open shm_client error");
		exit(errno);
	}
	if((shm_pere_id = shm_open(shm_pere_name, O_RDWR | O_CREAT, 0666)) == -1) {
		perror("shm_open shm_pere error");
		exit(errno);
	}

	if(ftruncate(shm_client_id, sizeof(struct myshm)) == -1) {
		perror("ftruncate shm_client error");
		exit(errno);
	}

	if((shm_pere = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, shm_pere_id, 0)) == MAP_FAILED){
		perror("mmap shm_pere error");
		exit(errno);
	}
	if((shm_client = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, shm_client_id, 0)) == MAP_FAILED){
		perror("mmap shm_pere error");
		exit(errno);
	}


	shm_client->read = 0;
	shm_client->write = 0;
	shm_client->nb = 0;

	if(sem_init(&(shm_client->sem), 1, 1) == -1){
		perror("sem_init shm_client");
		exit(errno);
	}

	msg.type = 0;

	strcpy(msg.content, argv[1]);
	sem_wait(&(shm_pere->sem));
	shm_pere->messages[shm_pere->write] = msg;
	shm_pere->write = (shm_pere->write + 1) % MAX_MESS;
	shm_pere->nb++;
	sem_post(&(shm_pere->sem));

	pthread_create(&(tids[0]), NULL, reader, NULL);
	pthread_create(&(tids[1]), NULL, writer, NULL);
	pthread_join(tids[0], NULL);
	pthread_join(tids[1], NULL);

	printf("Déconnexion...\n");

	msg.type = 2;

	sem_wait(&(shm_pere->sem));
	shm_pere->messages[shm_pere->write] = msg;
	shm_pere->write = (shm_pere->write + 1) % MAX_MESS;
	shm_pere->nb++;
	sem_post(&(shm_pere->sem));
	sem_close(&(shm_client->sem));

	munmap(shm_client, sizeof(struct myshm));
	shm_unlink(shm_client_name);

	return EXIT_SUCCESS;
}
