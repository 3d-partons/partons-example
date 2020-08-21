#include <partons/Partons.h>

#include "../include/examples.h"

/*
 * Main function.
 */
int main(int argc, char** argv)
{
  // Init PARTONS application
  PARTONS::Partons* pPartons = PARTONS::Partons::getInstance();
  pPartons->init(argc, argv);

  // My code
  //computeRunningAlphaS();
  MyGPDEvolution();
  //makeUseOfGPDEvolution();

  // Report results and close PARTONS application properly
  pPartons->close();

  return 0;
}
