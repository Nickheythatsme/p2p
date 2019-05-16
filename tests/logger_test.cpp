#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Nicholas Grout 4/30
//
#include "gtest/gtest.h"
#include "../src/util/logger.hpp"
#include <regex>

using namespace p2p;
using namespace std;

using matrix_vector = std::vector<std::vector<int>>;
using output_test = void (*)(matrix_vector &);

#define LOG_ITERATIONS 100
#define MATRIX_SIZE 1000
#define PLACE_HOLDER_TEXT "TESTING"
#define PAUSE(T) std::this_thread::sleep_for(std::chrono::milliseconds(T)) // give logs time to output

void do_matrix_mult(matrix_vector &matrix)
{
    for (auto &column : matrix)
        for (auto &var : column)
            var *= 10;
}

void use_logger(matrix_vector &matrix)
{
    Logger logger;
    Logger::add_file("logger.out", false);

    for (int i = 0; i < LOG_ITERATIONS; ++i) {
        logger.info("logger");
    }
    do_matrix_mult(matrix);
}

void use_basic_ostream(matrix_vector &matrix)
{
    std::ofstream fout("basic_ostream.out");

    for (int i = 0; i < LOG_ITERATIONS; ++i) {
        fout << PLACE_HOLDER_TEXT << std::endl;
    }
    do_matrix_mult(matrix);
}

void use_fprintf(matrix_vector &matrix)
{
    FILE *fout = fopen("fprintf.out", "w");

    for (int i = 0; i < LOG_ITERATIONS; ++i) {
        fprintf(fout, "%s\n", PLACE_HOLDER_TEXT);
        fflush(fout);
    }
    do_matrix_mult(matrix);
    fclose(fout);
}

double run_tests(output_test test)
{
    matrix_vector matrix{MATRIX_SIZE};
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        matrix[i].reserve(MATRIX_SIZE);
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            matrix[i][j] = i * j;
        }
    }

    auto start = std::chrono::steady_clock::now();
    test(matrix);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return duration.count();
}

TEST(Logger, Speed)
{
    // basic_ostream
    cout << "Starting basic_ostream test" << endl;
    double basic_ostream_duration = run_tests(use_basic_ostream);

    // fprintf
    cout << "Starting fprintf test" << endl;
    double printf_duration = run_tests(use_fprintf);

    // logger
    cout << "Starting logger test" << endl;
    double logger_duration = run_tests(use_logger);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    printf("basic_ostream: %f us\n", basic_ostream_duration);
    printf("printf:        %f us\n", printf_duration);
    printf("logger:        %f us\n", logger_duration);
}

TEST(Logger, OutputFormat)
{
    Logger logger("output");
    Logger::add_file("output-test.out", false);
    Logger::log_level = Logger::INFO;

    logger.debug("debug");
    logger.info("info");
    logger.warn("warn");
    logger.crit("crit");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Check to make sure output was correct
    std::ifstream fin("output-test.out");
    char buff[1024];
    std::vector<std::string> lines;

    fin.get(buff, 1024, '\n');
    fin.ignore(1024, '\n');

    while (fin.good()) {
        lines.emplace_back(buff);
        cout << buff << endl;

        fin.get(buff, 1024, '\n');
        fin.ignore(1024, '\n');
    }

    if (lines.size() != 3) {
        FAIL() << "Number of outputted lines was incorrect (expected 3 but got " << lines.size() << ")";
    }

    const char *time_check = R"(^(\[\d\d:\d\d:\d\d\])(.*))";
    const char *message_check = "(.*)(\\S+)$";
    for (const auto &line : lines) {
        try{
            if (!std::regex_match(line, std::regex(time_check))) {
                FAIL() << "Time was missing from log line: \"" << line << "\"";
            }
            if (!std::regex_match(line, std::regex(message_check))) {
                FAIL() << "Message was missing from log line: \"" << line << "\"";
            }
        }catch(const std::regex_error &e) {
            cout << "regex error: " << e.what() << endl;
        }
    }
}

TEST(Logger, VariadicTest)
{
    Logger logger;
    Logger::use_console();
    Logger::log_level = Logger::DEBUG;
    PAUSE(500);

    logger.info("testing ", 4, " variadic ", 0x12, " functions");
    PAUSE(200);
}

#pragma clang diagnostic pop
