#ifndef DECIMATOR_DEPRECATED_H
#define DECIMATOR_DEPRECATED_H

#include <cstdlib>
#include "Isosurface.h"
#include "LinkedList.h"

//
// decimate
//
// Draws a bunch of GL_TRIANGLES (with correct normals and vertex order)
// in order to render the given surface at the given isolevel.
// (METODO 2 antigo APENAS)
//

void decimate_deprecated(const Isosurface& surface,
              float xMin, float xMax,
              float yMin, float yMax,
              float zMin, float zMax,
              float isolevel,
              size_t resolution, LinkedList* list, int process_id, int n_processes);

#endif
