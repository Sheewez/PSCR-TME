#include <chat_common.h>

struct user {
	char* name;
	struct myshm* shm;
} *users[MAX_USERS];

int exit = 0;

void exithandler(int sig){
	exit = sig;
}

int main(int argc, char* argv[]){
	char* shm_name;
	int shm_id, i;
	struct myshm* shm_pere;
	struct sigaction action;

	if(argc <= 1) {
		fprintf(stderr, "Usage: %s id_server\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	action.sa_handler = exithandler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT, &action, 0);

	shm_name = (argv[1]);

	if((shm_id = shm_open(shm_name, O_RDWR | O_CREAT, 0666)) == -1) {
		perror("shm_open shm_pere");
		exit(EXIT_FAILURE);
	}

	if(ftruncate(shm_id, sizeof(struct myshm)) == -1) {
		perror("ftruncate shm_pere");
		exit(EXIT_FAILURE);
	}

	if((shm_pere = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED,shm_id, 0)) == MAP_FAILED){
		perror("mmap shm_pere");
		exit(EXIT_FAILURE);
	}
	shm_pere->read = 0;
	shm_pere->write = 0;
	shm_pere->nb = 0;

	if(sem_init(&(shm_pere->sem), 1, 1) == -1){
		perror("sem_init shm_pere");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<MAX_USERS; i++){
		users[i] = NULL;
	}

	while(!exit){
		sem_wait(&(shm_pere->sem));

		if(shm_pere->read != shm_pere->write || shm_pere->nb == MAX_MESS){

			struct message message = shm_pere->messages[shm_pere->read];
			switch(message.type){
				case 0:
				{

				char* username;
				int shm_id_user;
				i=0;

				while(i<MAX_USERS && users[i] != NULL)
					i++;
				if(i == MAX_USERS){
					perror("impossible d’ajouter l’user");
					exit(EXIT_FAILURE);}
				users[i] = malloc(sizeof(struct user));
				if(users[i] == NULL){perror("impossible d’ajouter l’user");exit(EXIT_FAILURE);}

				users[i]->name = malloc((strlen(message.content) + 1)*sizeof(char));
				strcpy(users[i]->name, message.content);
				users[i]->name[strlen(message.content)] = ’\0’;
				printf("Connexion de %s\n", users[i]->name);

				username = message.content;
				if((shm_id_user = shm_open(username, O_RDWR | O_CREAT, 0666)) == -1) {
					perror("shm_open shm_fils");
					exit(EXIT_FAILURE);
				}
				if((users[i]->shm = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE,MAP_SHARED, shm_id_user, 0)) == MAP_FAILED){
					perror("mmap shm_fils");
					exit(EXIT_FAILURE);
				}
				break;
			}
				case 1:
				{

					int temp = 0;
					printf("Réception message serveur : %s\n", message.content);
					for(i=0; i<MAX_USERS; i++){
						if(users[i] != NULL){
							struct message msg;
							struct myshm*shm = users[i]->shm;
							temp++;

							msg.type = 1;
							strcpy(msg.content, message.content);
							sem_wait(&(shm->sem));
							while(shm->nb == MAX_MESS){
								sem_post(&(shm->sem));
								sleep(1);
								sem_wait(&(shm->sem));
							}
							// verifier si full

							printf("\t\tEnvoi à %s\n", users[i]->name);
							shm->messages[shm->write] = msg;
							shm->write = (shm->write + 1) % MAX_MESS;
							shm->nb++;
							sem_post(&(shm->sem));
						}
					}
					if(temp == 0){
						printf("Nobody found!\n");
					}
					break;
				}
					case 2:
					{
						printf("Déconnexion de %s\n", message.content);
						/*Déconnexion*/
						i=0;
						/*On récupère le user correspondant à l’user qui se déconnecte*/
						while(i<MAX_USERS && (users[i] == NULL || strcmp(users[i]->name, message.content)!= 0))
							i++;
						if(i == MAX_USERS){
							perror("Something went wrong! L’user n’existe pas!");
							exit(EXIT_FAILURE);
						}

						/*On libère l’espace mémoire*/
						free(users[i]->name);
						munmap(users[i]->shm, sizeof(struct myshm));
						free(users[i]);
						users[i] = NULL;
						break;
					}
			}
			shm_pere->read = (shm_pere->read + 1) % MAX_MESS;
			shm_pere->nb--;
		}
		sem_post(&(shm_pere->sem));
	}

	sem_close(&(shm_pere->sem));
	munmap(shm_pere, sizeof(struct myshm));
	shm_unlink(shm_name);

	return EXIT_SUCCESS;
}


