/*
 * Block.h
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include <tsgl.h>

using namespace tsgl;

enum block_name
{
  A,
  B,
  C
};

static const int MAT_SIZE = 480; // total width of a single matrix

//static const int BLOCK_SIZE = 120;
//static const int SQUARE_WIDTH = 100;
// 120*4 = 480

/*!
 * \class Block
 * \brief Used as part of matrix for visualization
 * \details Draws a block of Cannon's Algorithm onto the Canvas.
 * \details The Block objects are created in a parallel block, and each thread gets one Block.
 * \details Then, the thread draws the Block onto the Canvas.
 * \details Each SeaUrchin object has its own color and plot data based off of the thread's id number.
 */
class Block {
public:

  /*!
   * \brief Explicitly construct a Block object.
   * \details Explicit constructor for a Block object.
   * \param can Reference to the Canvas to draw to.
   * \param threadId The id of the thread that is currently creating a Block object.
   * \param grid_size The size of the matrix grid.
   * \param blk The type of the current block object.
   * \return The constructed Block object.
   */
  Block(Canvas& can, int threadId, int grid_size, block_name blk);  //Default constructor

  /*!
   * \brief Draw the Block.
   * \details Actually draws the Block object onto the Canvas.
   * \param can Reference to the Canvas object to draw to.
   */
  void move(Canvas& can);

  /*!
   * \brief Destroy a Block.
   * \details Destructor for a Block object.
   * \note This function does absolutely nothing.
   */
  virtual ~Block();

private:
  int getX();
  int getY();
  int BLOCK_SIZE;
  int block_size, square_width;
  int margin, marginX, gridSize;
  int myX, myY;
  int myRow, myCol;
  int tid;
  ColorFloat myColor;
  Square* square;
  ProgressBar* pb; // only for result matrix
  block_name block;
  Canvas* c;
};

#endif /* BLOCK_H_ */
