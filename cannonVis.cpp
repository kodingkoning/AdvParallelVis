/*
 * cannon.cpp
 *
 * Usage: ./cannonVis <gridSize>
 * gridSize*gridSize is the number of threads
 * gridSize is the number of rows and columns in the matrices
 */

#include "tsgl.h"
#include <omp.h>

#include "Block.h"

using namespace tsgl;

// TODO: create matrix A and B, and have one move by rows and one move by columns

// simulate the use of multiple processes with multiple threads (as multiple processes cannot write to the same TSGL canvas)

/*
 * TODO: write description
 */
void cannonFunction(Canvas& can, int threads, int gridSize) {
  // steps:
  //    setup
  //    loop:
  //        multiply
  //        move data to next process
  #pragma omp parallel num_threads(threads*3)
  {
    if(omp_get_thread_num() < threads) {
      Block bA(can, omp_get_thread_num(), gridSize, A);
      while(can.isOpen()) {
          can.sleepFor(2);
          bA.move(can);
      }
    } else if(omp_get_thread_num() < threads*2) {
        Block bB(can, omp_get_thread_num()-threads, gridSize, B);
        while(can.isOpen()) {
          can.sleepFor(2);
          bB.move(can);
        }
    } else {
        Block bC(can, omp_get_thread_num()-threads*2, gridSize, C);
        while(can.isOpen()) {
          can.sleepFor(3);
          bC.move(can);
        }
    }
  }
}


int main(int argc, char * argv[]) {
  int gridSize = (argc > 1) ? atoi(argv[1]) : 4;
  clamp(gridSize,1,8);
  int nthreads = gridSize*gridSize; // Number of threads. Max is 64
  int canHeight = Canvas::getDisplayHeight()-100;
  int canWidth = Canvas::getDisplayWidth()-100;
  Canvas c(-1, -1, canWidth, canHeight, "Cannon's Algorithm", WHITE, nullptr, FRAME * 2);
  std::wstring text_x = L"x";
  Text* x = new Text(canWidth/3-canWidth/2, 0, 0, L"x", FONT, 40, 0, 0, 0, ColorFloat(0, 0, 0, 1));
  c.add(x);
  Text* eq = new Text(2*canWidth/3-canWidth/2, 0, 0, L"=", FONT, 40, 0, 0, 0, ColorFloat(0, 0, 0, 1));
  c.add(eq);
  c.run(cannonFunction, nthreads, gridSize);
}

