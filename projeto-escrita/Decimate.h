
#ifndef DECIMATOR_H
#define DECIMATOR_H

#include <cstdlib>
#include "Isosurface.h"
#include "LinkedList.h"

//
// decimate
//
// Draws a bunch of GL_TRIANGLES (with correct normals and vertex order)
// in order to render the given surface at the given isolevel.
//

void decimate(const Isosurface& surface,
              float xMin, float xMax,
              float isolevel,
              size_t resolution, LinkedList* list, int process_id, int n_processes);

#endif
