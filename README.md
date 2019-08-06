# Criador de Cronogramas Escolares.

Cria uma agenda para professores de ensino fundamental a médio


## O Algorítmo

O algorítmo será exremamente difícil de criar, pois
se baseia num problema NP-Complete.

### Variáveis

- P - A lista de professores.
- H - A lista de horários.
- T - A lista de turmas.
- PH - Lista de tuplas (p,h) com p &in; P e h &in; H. Representa os períodos onde cada professsor está disponível
- TH - Lista de tuplas (t,h) com t &in; T e h &in; T. Representa os períodos onde t &in; T está disponível
- PT - Lista de tuplas (t,p,n) com t &in; T, p &in; P e n &in; &naturals; Os períodos onde T P está disponível

#### Returno:

- PHT - Lista de tuplas (p,h,t) com t &in; T, p &in; P,  

#### Constraints no Retorno:

- Não há um professor em duas turmas ao mesmo tempo
- Nenhuma turma estará vazia a qualquer momento.
- Nenhuma turma terá dois professores a qualquer momento.
- Todo professor, em relação a cada turma, terá exatamente o número designado de períodos.

### Ideia 1: Bruteforce Insano

    bool check();

    bruteforce(P,H,T,PH,TH,PT){
		// Talvez em H e P seja mais justo
		// Já que todos os horários de T tem que estar preenchidos
		// ou algo parecido.
		returnVal[len(H)][len(T)];
		int ctr = 0;
		for(prof in P){
			bool found = false;
			if(prof.isAvalible(time)){
				for(class in C){
					if(class.isAvalible(time)
						&& hasClassesRemaining(class, returnVal, prof)){
						change(returnVal[h][ctr] = prof);
					}
					found = true;
					break;
				}
			}
			if(!found){
				//backtrack?
				undo LastChange();

			}
		}
	}

### Ideia 2: Tentar todos as combinações em relação a cada período

	// se é válido até tal período
	bool is_valid();
	// retorna a i-ésima ordenação de n elementos.
	int* ith_combination(int i, int n);

	smarter_bruteforce(P,H,T,PH,TH,PT){
		returnVal[len(H)];
		for(i in H){
			bool is_solvable = false;
			for(j in len(P)!)
				returnVal[i] = j
				if(is_valid(ith_combination(returnVal))){
					is_solvable = true;
					break;
				}
			if(!is_solvable){
				i--;
				returnVal[i]++;
			}
		}
	}

---

### Ideia 3: Backtrack

---
