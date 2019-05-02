// 
// Nicholas Grout 4/30
//
#include "gtest/gtest.h"
#include "../src/util/logger.hpp"
#include <fstream>

using namespace p2p;
using namespace std;

using matrix_vector = std::vector<std::vector<int>>;
using output_test = void (*)(matrix_vector&);

#define LOG_ITERATIONS 100
#define MATRIX_SIZE 1000
#define PLACE_HOLDER_TEXT "TESTING"

void do_matrix_mult(matrix_vector& matrix)
{
    for (auto& column : matrix)
        for(auto& var : column)
            var *= 10;
}

void use_logger(matrix_vector& matrix)
{
    Logger logger;
    std::ofstream fout("logger.out");
    logger.change_stream(std::move(fout));

    for (int i=0; i<LOG_ITERATIONS; ++i)
    {
        logger.info(PLACE_HOLDER_TEXT);
    }
    do_matrix_mult(matrix);
}

void use_basic_ostream(matrix_vector& matrix)
{
    std::ofstream fout("basic_ostream.out");
    auto backup = cout.rdbuf(fout.rdbuf());

    for (int i=0; i<LOG_ITERATIONS; ++i)
    {
        cout << PLACE_HOLDER_TEXT << std::endl;
    }
    do_matrix_mult(matrix);
    cout.rdbuf(backup);
}

void use_fprintf(matrix_vector& matrix)
{
    FILE* fout = fopen("fprintf.out","w");

    for (int i=0; i<LOG_ITERATIONS; ++i)
    {
        fprintf(fout, "%s\n", PLACE_HOLDER_TEXT);
        fflush(fout);
    }
    do_matrix_mult(matrix);
    fclose(fout);
}

double run_tests(output_test test)
{
    matrix_vector matrix {MATRIX_SIZE};
    for (int i=0; i<MATRIX_SIZE; ++i)
    {
        matrix[i].reserve(MATRIX_SIZE);
        for (int j=0; j<MATRIX_SIZE; ++j)
        {
            matrix[i][j] = i*j;
        }
    }

    auto start = std::chrono::steady_clock::now();
    test(matrix);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return duration.count();
}

void normal_matrix_mult(matrix_vector& matrix)
{
    Logger logger;
    std::ofstream fout("logger_null.out");
    logger.change_stream(std::move(fout));

    for (int i=0; i<100; ++i)
    {
        do_matrix_mult(matrix);
        // logger.debug(PLACE_HOLDER_TEXT);
        do_matrix_mult(matrix);
    }
}
void matrix_mult_with_logger(matrix_vector& matrix)
{
    Logger logger;
    std::ofstream fout("logger2.out");
    logger.change_stream(std::move(fout));
    logger.change_stream(std::move(cout));

    for (int i=0; i<100; ++i)
    {
        do_matrix_mult(matrix);
        logger.debug(PLACE_HOLDER_TEXT);
        do_matrix_mult(matrix);
    }
}

TEST(Logger, Speed) {
    // basic_ostream
    cout << "Starting basic_ostream test" << endl;
    double basic_ostream_duration = run_tests(use_basic_ostream);

    // fprintf
    cout << "Starting fprintf test" << endl;
    double printf_duration = run_tests(use_fprintf);

    // logger
    cout << "Starting logger test" << endl;
    double logger_duration = run_tests(use_logger);

    cout << "Starting logger slow down test" << endl;
    double logger_slow_down_duration = run_tests(normal_matrix_mult) - run_tests(matrix_mult_with_logger);

    usleep(100000);
    printf("basic_ostream: %f us\n", basic_ostream_duration);
    printf("printf:        %f us\n", printf_duration);
    printf("logger:        %f us\n", logger_duration);
    printf("logger slow down:        %f us\n", logger_slow_down_duration);

    if (logger_duration > printf_duration)
    {
        FAIL() << "logger test duration was longer than printf";
    }
    else if (logger_duration > basic_ostream_duration)
    {
        FAIL() << "logger test duration was longer than basic_ostream";
    }
}
