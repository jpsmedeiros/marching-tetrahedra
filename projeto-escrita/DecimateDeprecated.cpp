#include <cassert>
#include <iostream>
#include <cmath>
#include "Decimate.h"
#include "DecimateDeprecated.h"
#include "Array3D.h"
#include "LinkedList.h"

using namespace std;

LinkedList* resultsListDeprecated = NULL;

void decimate_deprecated(const Isosurface& surface,
              float xMin, float xMax,
              float yMin, float yMax,
              float zMin, float zMax,
              float isolevel,
              size_t resolution,
              LinkedList* list, int process_id, int n_processes)
{
    /*
     * Lista encadeada pra ajudar a enviar pro nó mestre na ordem certa
     */
    resultsListDeprecated = list;

    /*
     * Vai dizer quanto de resolução cada processo vai ter, pra ficar equilibrado.
     * Do contrário, a resolução multiplicaria junto com o nº de processos
     */
    int res_h = (resolution / (n_processes - 1));

    /*
     * Esse é o offset de onde cada processo deverá começar e desenhar.
     * O offset deverá ser logo após o fim do processo anterior, não pular pixels e nem interpolá-los.
     */
    int offset_res_x = res_h * (process_id - 1);

    /*
     * Cada range vai ser usado pra calcular o tamanho dos cubos.
     * Quanto maior o range, maior o cubo na direção do range em questão.
     * Isso não deve mudar o valor total calculado, só serve para exibição.
     *
     * O xMax, que vai definir o xrange de cada processo, fica definido em main.cpp na chamada deste método
     */
    float xrange = xMax - xMin;
    float yrange = yMax - yMin;
    float zrange = zMax - zMin;

    /*
     * Grid que vai conter o valor da função pra cada ponto
     */
    float grid[res_h + 1][resolution + 1][resolution + 1];

    /*
     * Posição do grid que inicia em zero pra todos os processos
     */
    int gridPositionX;

    /*
     * Variáveis auxiliares pra armazenar valores no grid
     */
    float x, y, z;

    for (int i = offset_res_x, gridPositionX = 0; i <= res_h + offset_res_x; ++i, ++gridPositionX) {
        x = (float) (i)/res_h * xrange + xMin;
        for (size_t j = 0; j <= resolution; ++j) {
            y = (float)j/resolution * yrange + yMin;
            for (size_t k = 0; k <= resolution; ++k) {
                z = (float)k/resolution * zrange + zMin;
                grid[gridPositionX][j][k] = surface.valueAt(x, y, z);
            }
        }
    }

    /*
     * p1 é o ponto atual
     * p2 é o ponto seguinte
     * TODO: guardar o ponto seguinte pra n precisar recalcular o ponto atual na próxima iteração
     */
    float x1, x2;
    float y1, y2;
    float z1, z2;

    for (size_t i = offset_res_x, gridPositionX = 0; i < res_h + offset_res_x; ++i, ++gridPositionX) {
        x1 = (float) i / res_h * xrange + xMin;
        x2 = (float) (i+1) / res_h * xrange + xMin;
        for (size_t j = 0; j < resolution; ++j) {
            y1 = (float) j / resolution * yrange + yMin;
            y2 = (float) (j+1) / resolution * yrange + yMin;
            for (size_t k = 0; k < resolution; ++k) {
                z1 = (float) k / resolution * zrange + zMin;
                z2 = (float) (k+1) / resolution * zrange + zMin;

                /*

                 Cube layout:

                    4 ----- 7
                   /|      /|
                  / |     / |
                 5 ----- 6  |
                 |  0 ---|--3
                 | /     | /
                 |/      |/
                 1 ----- 2


                 Tetrahedrons are:
                     0, 7, 3, 2
                     0, 7, 2, 6
                     0, 4, 6, 7
                     0, 6, 1, 2
                     0, 6, 1, 4
                     5, 6, 1, 4

                 */

                const Point3D v[8] = {
                    {x1, y1, z1, grid[gridPositionX][j][k]},
                    {x2, y1, z1, grid[gridPositionX+1][j][k]},
                    {x2, y2, z1, grid[gridPositionX+1][j+1][k]},
                    {x1, y2, z1, grid[gridPositionX][j+1][k]},
                    {x1, y1, z2, grid[gridPositionX][j][k+1]},
                    {x2, y1, z2, grid[gridPositionX+1][j][k+1]},
                    {x2, y2, z2, grid[gridPositionX+1][j+1][k+1]},
                    {x1, y2, z2, grid[gridPositionX][j+1][k+1]}
                };

                const Point3D tetrahedra[6][4] = {
                    { v[0], v[7], v[3], v[2] },
                    { v[0], v[7], v[2], v[6] },
                    { v[0], v[4], v[7], v[6] },
                    { v[0], v[1], v[6], v[2] },
                    { v[0], v[4], v[6], v[1] },
                    { v[5], v[1], v[6], v[4] }
                };

                for (int t = 0; t < 6; ++t){
                    drawTetrahedron(surface, tetrahedra[t], isolevel);
                }
            }
        }
    }
}
