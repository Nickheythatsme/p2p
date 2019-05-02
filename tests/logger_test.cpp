// 
// Nicholas Grout 4/30
//
#include "gtest/gtest.h"
#include "../src/util/logger.hpp"
#include <chrono>

using namespace p2p;
using namespace std;

void do_matrix_mult(int **matrix, size_t x, size_t y)
{
    for (int i=0; i<x; ++i)
    {
        for (int j=0; j<y; ++j)
            matrix[j][i] *= 100;
    }
}

void use_logger(int **matrix)
{
    Logger logger;
    for (int i=0; i<100; ++i)
    {
        logger.debug(std::to_string(i) + " logger");
    }
    do_matrix_mult(matrix, 100, 100);
}

void use_cout(int **matrix)
{
    for (int i=0; i<100; ++i)
    {
        cout << i << " cout" << std::endl;
    }
    do_matrix_mult(matrix, 100, 100);
}

int **make_matrix(size_t size)
{
    int **matrix = new int*[size];
    for (int i=0; i<size; ++i)
    {
        matrix[i] = new int[size];
        for (int j=0; j<size; ++j)
        {
            matrix[i][j] = j*i;
        }
    }
    return matrix;
}

TEST(Logger, Speed) {
    Logger logger;
    int **matrix1 = make_matrix(1000);
    int **matrix2 = make_matrix(1000);

    // cout
    auto start_cout = std::chrono::steady_clock::now();
    use_cout(matrix1);
    auto end_cout = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> len_cout = end_cout-start_cout;

    // logger
    auto start_logger = std::chrono::steady_clock::now();
    use_logger(matrix2);
    auto end_logger = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> len_logger = end_logger-start_logger;

    double len_cout_d = len_cout.count();
    double len_logger_d = len_logger.count();
    usleep(100000);
    printf("cout: %f us\n", len_cout_d);
    printf("logger: %f us\n", len_logger_d);
}

