/*
 * Block.cpp
 */

#include "Block.h"

// TODO: make a matrix width (whatever it is when the gridSize is 4) and make the larger ones fit into that so we can have all the sizes fit on one screen

ColorFloat getColor(int tid, int gridSize, block_name blk) {
    if(blk == A) {
      int myRow = tid/gridSize;
      int myCol = tid%gridSize;
      ColorFloat color = Colors::highContrastColor(myCol);
      color.A = (myRow+0.5)/gridSize;
      return color;
    } else if(blk == B) {
      int myRow = tid/gridSize;
      int myCol = tid%gridSize;
      ColorFloat color = Colors::highContrastColor(myRow);
      color.A = (myCol+0.5)/gridSize;
      return color;
    }
    return ColorFloat(0, 0, 0, 1);
}

void glideTo(Canvas& can, Drawable* d, float x, float y, float time, int steps) {
    float stepX = (x - d->getCenterX())/steps;
    float stepY = (y - d->getCenterY())/steps;
    for(int i = 0; i < steps; i++) {
        d->changeXBy(stepX);
        d->changeYBy(stepY);
        can.sleepFor(time/steps);
    }
}

int Block::getX() {
    if(block == A)
        return -c->getWindowWidth()/2 + marginX + (myCol+0.5)*BLOCK_SIZE;
    if(block == B)
        return -c->getWindowWidth()/2 + marginX*2 + BLOCK_SIZE*gridSize + (myCol+0.5)*BLOCK_SIZE;
    else
        return -c->getWindowWidth()/2 + marginX*3 + BLOCK_SIZE*gridSize*2 + (myCol+0.5)*BLOCK_SIZE;
}

int Block::getY() {
    return -c->getWindowHeight()/2 + margin + (gridSize-myRow-0.5)*BLOCK_SIZE;
}


Block::Block(Canvas& can, int threadId, int grid_size, block_name blk) {
  c = &can;
  block = blk;
  gridSize = grid_size;
  BLOCK_SIZE = MAT_SIZE/gridSize;
  int squareWidth = BLOCK_SIZE*0.85;
  tid = threadId;
  margin = (can.getWindowHeight() - MAT_SIZE)/2;
  //margin = (can.getWindowHeight() - BLOCK_SIZE*gridSize)/2 + 60;
  marginX = (can.getWindowWidth() - MAT_SIZE*3)/4;
  myRow = threadId / gridSize;
  myCol = threadId % gridSize;
  if(block == A) {
    myX = getX();
    myY = getY();
  } else if(block == B) {
    myX = getX();
    myY = getY();
  } else if(block == C) {
    myX = getX();
    myY = getY();
  }
  myColor = getColor(threadId, grid_size, block);
  if(block == C) {
    pb = new ProgressBar(myX, myY, 0, squareWidth, squareWidth, 0, gridSize, gridSize, 0, 0, 0);
    for(int i=0; i<grid_size; i++) {
      pb->update(i+0.001,i);
    }
    can.add(pb);
  } else {
    square = new Square(myX, myY, 0, squareWidth, 0, 0, 0, myColor);
    can.add(square);
  }

  // stagger the blocks
  can.sleepFor(1);
  if(block == A) {
    myCol = (myCol-myRow+gridSize)%gridSize;
    myX = getX();
  } else if(block == B) {
    myRow = (myRow-myCol+gridSize)%gridSize;
    myY = getY();
  } else if(block == C) {
    myRow = (myCol+myRow+gridSize)%gridSize;
  }
  if(block != C)
    glideTo(can, square, myX, myY, 3, 40);
  else
    can.sleepFor(0);
}

void Block::move(Canvas& can) {
  if(block == A) {
    myCol++;
    myCol += gridSize;
    myCol %= gridSize;
    myX = getX();
  } else if(block == B) {
    myRow++;
    myRow %= gridSize;
    myY = getY();
  }

  if(block == C) {
    for(int i=1; i<41; i++) {
        pb->update(myRow+i/40.0,myRow);
        can.sleepFor(2.0/40.0);
    }
    myRow--;
    myRow += gridSize;
    myRow %= gridSize;
  } else
    glideTo(can, square, myX, myY, 3, 40);
}

Block::~Block() {
  delete square;
  delete pb;
}

