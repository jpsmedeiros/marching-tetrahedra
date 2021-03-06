#include <cassert>
#include <iostream>
#include <cmath>
#include "Decimate.h"
#include "Array3D.h"
#include "LinkedList.h"

using namespace std;

LinkedList* resultsList = NULL;

static inline void drawVert(const Isosurface& surface, const Point3D& p1, const Point3D& p2, float isolevel)
{

    float v1 = p1.value;
    float v2 = p2.value;

    float x, y, z;

    if (v2 == v1) {
        x = (p1.x + p2.x) / 2.0f;
        y = (p1.y + p2.y) / 2.0f;
        z = (p1.z + p2.z) / 2.0f;
    } else {

        /*

         <----+-----+---+----->
              v1    |   v2
                 isolevel


         <----+-----+---+----->
              0     |   1
                  interp

         */

        // interp == 0: vert should be at p1
        // interp == 1: vert should be at p2
        float interp = (isolevel - v1) / (v2 - v1);
        float oneMinusInterp = 1 - interp;

        x = p1.x * oneMinusInterp + p2.x * interp;
        y = p1.y * oneMinusInterp + p2.y * interp;
        z = p1.z * oneMinusInterp + p2.z * interp;
    }

    Vector3D normal = surface.gradientAt(x, y, z);

    if (resultsList != NULL) {
        resultsList->add(normal.x, normal.y, normal.z, 1);
        resultsList->add(x, y, z, -1);
    }else {
        //cout << "n," << normal.x << ',' << normal.y << ',' << normal.z << ",1.0" << '\n';
        //cout << "s," << x << ',' << y << ',' << z << ",1.0" << '\n';
    };


}

static void drawTetrahedron(const Isosurface& surface, const Point3D p[4], float isolevel)
{

    /*

     Tetrahedron layout:

           0
           *
          /|\
         / | \
      3 *-----* 1
         \ | /
          \|/
           *
           2
     */

    unsigned char index = 0;
    for (int i = 0; i < 4; ++i)
        if (p[i].value <= isolevel)
            index |= (1 << i);

    switch (index) {

        // we don't do anything if everyone is inside or outside
        case 0x00:
        case 0x0F:
            break;

        // only vert 0 is inside
        case 0x01:
            drawVert(surface, p[0], p[1], isolevel);
            drawVert(surface, p[0], p[3], isolevel);
            drawVert(surface, p[0], p[2], isolevel);
            break;

        // only vert 1 is inside
        case 0x02:
            drawVert(surface, p[1], p[0], isolevel);
            drawVert(surface, p[1], p[2], isolevel);
            drawVert(surface, p[1], p[3], isolevel);
            break;

        // only vert 2 is inside
        case 0x04:
            drawVert(surface, p[2], p[0], isolevel);
            drawVert(surface, p[2], p[3], isolevel);
            drawVert(surface, p[2], p[1], isolevel);
            break;

        // only vert 3 is inside
        case 0x08:
            drawVert(surface, p[3], p[1], isolevel);
            drawVert(surface, p[3], p[2], isolevel);
            drawVert(surface, p[3], p[0], isolevel);
            break;

        // verts 0, 1 are inside
        case 0x03:
            drawVert(surface, p[3], p[0], isolevel);
            drawVert(surface, p[2], p[0], isolevel);
            drawVert(surface, p[1], p[3], isolevel);

            drawVert(surface, p[2], p[0], isolevel);
            drawVert(surface, p[2], p[1], isolevel);
            drawVert(surface, p[1], p[3], isolevel);
            break;

        // verts 0, 2 are inside
        case 0x05:
            drawVert(surface, p[3], p[0], isolevel);
            drawVert(surface, p[1], p[2], isolevel);
            drawVert(surface, p[1], p[0], isolevel);

            drawVert(surface, p[1], p[2], isolevel);
            drawVert(surface, p[3], p[0], isolevel);
            drawVert(surface, p[2], p[3], isolevel);
            break;

        // verts 0, 3 are inside
        case 0x09:
            drawVert(surface, p[0], p[1], isolevel);
            drawVert(surface, p[1], p[3], isolevel);
            drawVert(surface, p[0], p[2], isolevel);

            drawVert(surface, p[1], p[3], isolevel);
            drawVert(surface, p[3], p[2], isolevel);
            drawVert(surface, p[0], p[2], isolevel);
            break;

        // verts 1, 2 are inside
        case 0x06:
            drawVert(surface, p[0], p[1], isolevel);
            drawVert(surface, p[0], p[2], isolevel);
            drawVert(surface, p[1], p[3], isolevel);

            drawVert(surface, p[1], p[3], isolevel);
            drawVert(surface, p[0], p[2], isolevel);
            drawVert(surface, p[3], p[2], isolevel);
            break;

        // verts 2, 3 are inside
        case 0x0C:
            drawVert(surface, p[1], p[3], isolevel);
            drawVert(surface, p[2], p[0], isolevel);
            drawVert(surface, p[3], p[0], isolevel);

            drawVert(surface, p[2], p[0], isolevel);
            drawVert(surface, p[1], p[3], isolevel);
            drawVert(surface, p[2], p[1], isolevel);
            break;

        // verts 1, 3 are inside
        case 0x0A:
            drawVert(surface, p[3], p[0], isolevel);
            drawVert(surface, p[1], p[0], isolevel);
            drawVert(surface, p[1], p[2], isolevel);

            drawVert(surface, p[1], p[2], isolevel);
            drawVert(surface, p[2], p[3], isolevel);
            drawVert(surface, p[3], p[0], isolevel);
            break;

        // verts 0, 1, 2 are inside
        case 0x07:
            drawVert(surface, p[3], p[0], isolevel);
            drawVert(surface, p[3], p[2], isolevel);
            drawVert(surface, p[3], p[1], isolevel);
            break;

        // verts 0, 1, 3 are inside
        case 0x0B:
            drawVert(surface, p[2], p[1], isolevel);
            drawVert(surface, p[2], p[3], isolevel);
            drawVert(surface, p[2], p[0], isolevel);
            break;

        // verts 0, 2, 3 are inside
        case 0x0D:
            drawVert(surface, p[1], p[0], isolevel);
            drawVert(surface, p[1], p[3], isolevel);
            drawVert(surface, p[1], p[2], isolevel);
            break;

        // verts 1, 2, 3 are inside
        case 0x0E:
            drawVert(surface, p[0], p[1], isolevel);
            drawVert(surface, p[0], p[2], isolevel);
            drawVert(surface, p[0], p[3], isolevel);
            break;

        // shouldn't be going here
        default:
            assert(false);
    }

}

void decimate(const Isosurface& surface, float xMin, float xMax, float isolevel,
              size_t resolution, LinkedList* list, int process_id, int n_processes)
{

    /*
     * Lista encadeada pra ajudar a enviar pro nó mestre na ordem certa
     */
    resultsList = list;

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
     *  Sempre desenhamos o espaço em formato de cubo, portanto, basta usar o xMin pra calcular
     *  zMin e yMin, que é o ponto inicial das coordenadas y e z
     */
    float yMin = xMin;
    float zMin = xMin;

    /*
     * Cada range vai ser usado pra calcular o tamanho dos cubos.
     * Quanto maior o range, maior o cubo na direção do range em questão.
     * Isso não deve mudar o valor total calculado, só serve para exibição.
     *
     * O xMax, que vai definir o xRange de cada processo, fica definido em main.cpp na chamada deste método
     */
    float xRange = xMax - xMin;
    float yRange = -yMin - yMin;
    float zRange = -zMin - zMin;

    // cout << "\nxRange: " << xRange << " Offset_res_x: " << offset_res_x << " process: " << process_id - 1 << " res_h: " << res_h << "\n";

    /*
     * p1 é o ponto atual, que vai usar o p2 da interação passada e o valor inicial
     * da primeira posição do cubo pra cada processo
     */
    float x1 = (float) (offset_res_x) / res_h * xRange + xMin;
    float y1, z1;

    /*
     * p2 é o ponto seguinte, que é calculado em toda iteração.
     */
    float x2, y2, z2;

    Point3D last_top_surface[4];

    for (size_t i = offset_res_x; i < res_h + offset_res_x; ++i) {
        x2 = (float) (i+1) / res_h * xRange + xMin;

        for (size_t j = 0; j < resolution; ++j) {
            if (j == 0) y1 = yMin;
            y2 = (float) (j+1) / resolution * yRange + yMin;

            for (size_t k = 0; k < resolution; ++k) {
                if (k == 0) z1 = zMin;
                z2 = (float) (k+1) / resolution * zRange + zMin;

                /*
                 * Calcula o fundo do cubo se a iteração estiver começando em Z
                 *
                 * Pontos do topo do cubo serão reaproveitados
                 * ao invés de recalculados toda iteração.
                 * Para mais detalhes, checar o uso na declaração const Point3D v[8].
                 */
                if (k == 0) {
                    last_top_surface[0] = (Point3D) {x1, y1, z1, surface.valueAt(x1, y1, z1)}; // 0
                    last_top_surface[1] = (Point3D) {x2, y1, z1, surface.valueAt(x2, y1, z1)}; // 1
                    last_top_surface[2] = (Point3D) {x2, y2, z1, surface.valueAt(x2, y2, z1)}; // 2
                    last_top_surface[3] = (Point3D) {x1, y2, z1, surface.valueAt(x1, y2, z1)}; // 3
                }

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
                */

                /*
                 * Monta o Point3D representando o CUBO que vai ser usado com os valores relativos da função.
                 * OBS do método 3: cada ponto é recalculado conforme o necessário, oa invés de usar
                 * um grid como no método 2, isso poupa memória pois não precisaremos armazenar cada valor
                 * e depois consultá-los.
                 *
                 * O fundo do cubo, ou seja, pontos 0, 1, 2 e 3, já foi utilizado na
                 * iteração passada, portanto, podemos reaproveitar ao invés de recalcular,
                 * já que o topo do cubo da iteração anterior é o fundo do cubo atual sempre.
                 */
                const Point3D v[8] = {
                    last_top_surface[0], // 0
                    last_top_surface[1], // 1
                    last_top_surface[2], // 2
                    last_top_surface[3], // 3
                    {x1, y1, z2, surface.valueAt(x1, y1, z2)}, // 4
                    {x2, y1, z2, surface.valueAt(x2, y1, z2)}, // 5
                    {x2, y2, z2, surface.valueAt(x2, y2, z2)}, // 6
                    {x1, y2, z2, surface.valueAt(x1, y2, z2)}  // 7
                };

                /*
                 * Monta uma matriz de 6x4 Point3D representando CADA TETRAEDRO DO CUBO, sendo 6 no total.
                 * (Ou seja, 6 tetraedos de 4 lados, matriz 6x4 de Point3Ds)
                 *
                 *   Tetrahedrons are:
                 *    0, 7, 3, 2
                 *    0, 7, 2, 6
                 *    0, 4, 6, 7
                 *    0, 6, 1, 2
                 *    0, 6, 1, 4
                 *    5, 6, 1, 4
                 */
                const Point3D tetrahedra[6][4] = {
                    { v[0], v[7], v[3], v[2] },
                    { v[0], v[7], v[2], v[6] },
                    { v[0], v[4], v[7], v[6] },
                    { v[0], v[1], v[6], v[2] },
                    { v[0], v[4], v[6], v[1] },
                    { v[5], v[1], v[6], v[4] }
                };

                /*
                 * Chama o método de desenhar de fato para cada tetraedro, que vai adicionar
                 * na lista encadeada seus retornos, caso seja desenhado alguma coisa.
                 */
                for (int t = 0; t < 6; ++t) {
                    drawTetrahedron(surface, tetrahedra[t], isolevel);
                }

                /*
                 * "Anda" o fundo do cubo pra cima pois vamos para próxima iteração.
                 */
                last_top_surface[0] = v[4];
                last_top_surface[1] = v[5];
                last_top_surface[2] = v[6];
                last_top_surface[3] = v[7];

                /*
                 * Atualiza z1 pra próxima iteração utlizar o z2 já previamente calculado,
                 * evitando de calcularmos o mesmo ponto 2 vezes
                 */
                z1 = z2;
            }
            y1 = y2;
        }
        x1 = x2;
    }

//    if (process_id == n_processes - 1)
//        printf ("\nlast x2: %f\n", x2);

}
