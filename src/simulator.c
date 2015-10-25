#include "fssim/simulator.h"


fs_simulator_t* fs_simulator_create(void)
{
  fs_simulator_t* sim = malloc(sizeof(*sim));
  PASSERT(sim, FS_ERR_MALLOC);

  sim->fs = NULL;

  return sim;
}

void fs_simulator_destroy(fs_simulator_t* sim)
{
  free(sim);
}
