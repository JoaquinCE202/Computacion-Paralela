#include <iostream>
#include <chrono>  

using namespace std;

int m=2;
int n=2;
int o=2;


void crear(int **&A, int dimension1, int dimension2)
{
  A=new int* [dimension1];
  for(int x=0;x<dimension1;x++)
    {
      A[x]=new int [dimension2];
    }
}

void borrar(int **&A, int dimension1, int dimension2)
{
  for (int x = 0; x < dimension2; x++) {
            delete[] A[x];
        }
        delete[] A;
}

void llenar(int **A, int count, int dimension1, int dimension2)
{

  for(int x=0; x<dimension1; x++)
  {
    for(int y=0; y<dimension2; y++)
    { 
      count++;
      A[x][y]=count;
    }
  }
}

void imprimir(int **A,int dimension1, int dimension2)
{
  for(int x=0; x<dimension1; x++)
  {
    for(int y=0; y<dimension2; y++)
    { 
      cout<<A[x][y]<<" ";
    }
    cout<<endl;
  }
  cout<<endl;
}

void multiplicacion_matrices(int **A,int **B,int **C)
{
  for(int x=0; x<m; x++)
  {
    for(int y=0; y<o; y++)
    { 
      C[x][y]=0;
    }
  }
  
  for(int x=0; x<m; x++)
  {
    for(int y=0; y<o; y++)
    { 
      for(int z=0; z<n; z++)
      {
        C[x][y]= C[x][y]+A[x][z]*B[z][y];
 
      }
    }
  }
 
}

void multiplicar_un_bloque(int ** A, int ** B, int **C, int blockSize,int i,int j,int k) 
{
  for (int ii = i; ii < min(m, i + blockSize); ++ii) 
  {
      for (int jj = j; jj < min(o, j + blockSize); ++jj) 
      {
          for (int kk = k; kk < min(n, k + blockSize); ++kk) 
          {
              C[ii][jj] += A[ii][kk] * B[kk][jj];
          }
      }
  }
}


void multiplicar_bloques(int ** A, int ** B, int **C, int bloque) {

  for(int x=0; x<m; x++)
  {
    for(int y=0; y<o; y++)
    { 
      C[x][y]=0;
    }
  }

    for (int i = 0; i < m; i += bloque) 
    {
      if(m < i )
              {
                break;
              }
        for (int j = 0; j < o; j += bloque) 
        {
          if(o < j )
              {
                break;
              }
            for (int k = 0; k < n; k += bloque) 
            {
                // Multiplicación de bloques
              if(n < k )
              {
                break;
              }
            multiplicar_un_bloque(A,B,C,bloque,i,j,k);
            }
        }
    }
}

void prueba(int** A, int** B, int** C, int salto) {
    int cont=salto;
    for (int x = 0; x < 50; x++) {

        n = m = o = cont;

        crear(A, m, n);
        crear(B, n, o);
        crear(C, m, o);

        llenar(A, 0, m, n);
        llenar(B, 9, n, o);

        auto start = std::chrono::system_clock::now();
        multiplicacion_matrices(A, B, C);
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<float, std::milli> duration = end - start;
        std::cout << duration.count() << "  ";
      
        // segunda prueba
        start = std::chrono::system_clock::now();
        multiplicar_bloques(A, B, C, cont/2);
        end = std::chrono::system_clock::now();

        duration = end - start;
        std::cout << duration.count() << std::endl;

         // Liberar la memoria de las matrices al final de cada iteración
        borrar(A, m, n);
        borrar(B, n, o);
        borrar(C, m, o);

        cont = cont + salto;
    }
}


int main() {

  int count=0;
  
  
  int **A;
  int **B;
  int **C;
  
  prueba(A,B,C,20);


}
