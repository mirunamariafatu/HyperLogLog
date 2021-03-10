// "Copyright 2020 Fatu Miruna Maria"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

unsigned int hash_function_int(void *a) {
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

void MInit(unsigned int *M){
  // se initializeaza vectorul M
    int i;
    for (i = 0; i < 2049; i++){
        memset(&M[i], '\0', sizeof(unsigned int));
    }
}

void MCreate(unsigned int *M, unsigned int *nr){
  unsigned int hash = hash_function_int(nr);

  // se obtine indexul din primii 11 biti ai hash-ului
  unsigned int index = hash & ((1<<11) - 1);

  // se identifica cati biti 0 initiali exista
  int i;
  unsigned int x = 1;
  for (i = 11; i < 32; i++){
    if ((hash & (1 << i)) == 0){
        x++;
    } else {
      break;
    }
  }
  // actualizam numarul de biti 0 initiali
  if (x > M[index]){
    M[index] = x;
  }
}

double ZCreate(unsigned int *M){
  double S = 0;
  int i;
  for (i = 0; i< 2049; i++){
    if (M[i] > 0){
    S += 1.0 / (1 << M[i]);
    }
  }
  double Z = 1 / S;
  return Z;
}

unsigned int ECreate(unsigned int *M){
  int i;
  // se identifica numarul de bucket-uri folosite
  int m = 0;
  for (i = 0; i < 2049; i++){
    if (M[i] > 0){
      m++;
    }
  }

  double alfa_m = 0;
  alfa_m = 0.7213 / (1 + (1.079 / m));

  double Z = ZCreate(M);
  unsigned int E = 0;
  E = alfa_m * m * m * Z;
  return E;
}

int main(int argc , char *argv[]){
    if (argc != 2) {
		printf("Incorrect number of arguments!\n");
		return -1;
	}
    char input_filename[100];
    int lg1 = strlen(argv[1]);
    snprintf(input_filename , lg1+1 , "%s" , argv[1]);

    // se deschide fisierul din care se vor citi datele
    FILE *in;
    if ((in = fopen(input_filename, "rb")) == NULL) {
        fprintf(stderr , "Can't open %s" , input_filename);
        return -1;
    }

    int m = (1 << 11) + 1;
    unsigned int M[m];
    MInit(M);

    // se citesc numerele din fisier
    unsigned int nr;
    while (fscanf(in, "%d", &nr) != EOF){
      MCreate(M, &nr);
      }

    unsigned int E = ECreate(M);
    printf("%d", E);

    fclose(in);
    return 0;
}
