#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

// Minimal test framework — no external dependencies

static int g_tests_run    = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST(name) \
    static void name(); \
    struct name##_reg { name##_reg() { \
        ++g_tests_run; \
        std::cout << "  [ RUN ] " #name << std::endl; \
        try { name(); ++g_tests_passed; std::cout << "  [PASS] " #name << std::endl; } \
        catch (const std::exception& e) { ++g_tests_failed; std::cout << "  [FAIL] " #name ": " << e.what() << std::endl; } \
    } } name##_instance; \
    static void name()

#define EXPECT_NEAR(a, b, tol) \
    do { \
        double _a = (a), _b = (b), _t = (tol); \
        if (std::abs(_a - _b) > _t) { \
            throw std::runtime_error( \
                std::string("EXPECT_NEAR failed: |") + std::to_string(_a) + " - " + std::to_string(_b) + \
                "| = " + std::to_string(std::abs(_a - _b)) + " > " + std::to_string(_t)); \
        } \
    } while(0)

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) throw std::runtime_error("EXPECT_TRUE failed: " #cond); \
    } while(0)

#define EXPECT_LT(a, b) \
    do { \
        if (!((a) < (b))) throw std::runtime_error( \
            std::string("EXPECT_LT failed: ") + std::to_string(a) + " >= " + std::to_string(b)); \
    } while(0)

#define EXPECT_GT(a, b) \
    do { \
        if (!((a) > (b))) throw std::runtime_error( \
            std::string("EXPECT_GT failed: ") + std::to_string(a) + " <= " + std::to_string(b)); \
    } while(0)

inline int test_summary() {
    std::cout << "\n============================\n";
    std::cout << "Tests run:    " << g_tests_run    << "\n";
    std::cout << "Tests passed: " << g_tests_passed << "\n";
    std::cout << "Tests failed: " << g_tests_failed << "\n";
    std::cout << "============================\n";
    return g_tests_failed > 0 ? 1 : 0;
}
