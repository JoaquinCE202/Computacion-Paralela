#include <iostream>
#include <chrono>  

int MAX=1000;

using namespace std;

int main() {
  
  
  double A[MAX][MAX], x[MAX], y[MAX];

  for (int i = 0; i < MAX; i++)
    {
      for (int j = 0; j < MAX; j++)
        {
          A[i][j]=1;
        }
    }

  
  auto start = std::chrono::system_clock::now();

  
  for (int i = 0; i < MAX; i++)
    {
      for (int j = 0; j < MAX; j++)
        {
          y[i] += A[i][j] * x[j];
        }
    }
   auto end = std::chrono::system_clock::now();

  std::chrono::duration<float,std::milli> duration = end - start;

  std::cout << duration.count() <<endl;


  start = std::chrono::system_clock::now();
  for (int j = 0; j < MAX; j++)
    {
      for (int i = 0; i < MAX; i++)
        {
           y[i] += A[i][j] * x[j];
        }
    }
  end = std::chrono::system_clock::now();

  duration = end - start;

  std::cout << duration.count() <<endl;
  
 
}
