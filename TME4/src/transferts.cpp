#include<vector>
#include<Banque.h>
#include<Compte.h>
#include<thread>




void transfertJob (int index, Banque & banque) {

	for (int i= 0 ; i < 100000; i++) {
		int debite = rand() % banque.size();
		int credite = rand() % banque.size();
		int val = rand() % 100;

		banque.transfert(debite,credite,val);
	}
}

void transfertBis(size_t deb, size_t cred, unsigned int val) {
	Compte * debiteur = new Comptes[deb];
	Compte * crediteur = new Comptes[cred];

	debiteur.lock();
	crediteur.lock();

	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}

	debiteur.unlock();
	crediteur.unlock();
}
void transfertBisCorr(size_t deb, size_t cred, unsigned int val) {
	Compte * debiteur = new Comptes[deb];
	Compte * crediteur = new Comptes[cred];

	if (deb < cred) {
		debiteur.lock();
		crediteur.lock();
	}else{
		crediteur.lock();
		debiteur.lock();
	}

	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}

	debiteur.unlock();
	crediteur.unlock();
}


int main () {
	using namespace pr;
	const int N = 10;
	srand(time(nullptr));
	vector<thread> threads;
	threads.reserve(N);

	Banque b(200,100);

	for (int i=0; i < N ; i++) {
		threads.emplace_back(transfertJob, i, std::ref(b));
	}

	for (int i=0; i < N ; i++) {
		threads[i].join();
	}

	return 0;
}
