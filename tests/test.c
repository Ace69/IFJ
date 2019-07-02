/*
 * File:   newcunittest.c
 * Author: dbulawa
 *
 * Created on 13.11.2018, 0:03:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "../src/header/scanner.h"
#include <stdbool.h>
#include "../src/header/parser.h"
/*
 * CUnit Test Suite
 */

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

void test_muj_intput2(){
    FILE *fp;
    fp = fopen("tests/muj_input2.rb", "rw+");
    delegateSourceFile(fp);
    CU_ASSERT_EQUAL(false,parser());
    fclose(fp);
}

int main() {
    
    CU_pSuite pSuite = NULL;
    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("Token test", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if (
        (NULL == CU_add_test(pSuite, "PARSER muj_input2", test_muj_intput2))
            
           ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
