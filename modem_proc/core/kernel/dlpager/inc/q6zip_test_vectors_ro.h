#pragma once
#include "q6zip_test.h"

#define Q6ZIP_TEST_VECTORS_RO_MAX 13

/** @brief Number of Q6ZIP RO error vectors */
#define Q6ZIP_TEST_VECTORS_RO_ERROR_MAX 3

#define Q6ZIP_TEST_RO_DICT_SIZE 4096

extern unsigned char const Q6ZIP_RO_DICT[ Q6ZIP_TEST_RO_DICT_SIZE ];

/** @brief Table of Q6ZIP RO vectors */
extern q6zip_test_vectors_index_t const RO_VECTORS_INDEX[ Q6ZIP_TEST_VECTORS_RO_MAX ];

/** @brief Table of Q6ZIP RO error vectors */
extern q6zip_test_vectors_index_t const RO_ERROR_VECTORS[ Q6ZIP_TEST_VECTORS_RO_ERROR_MAX ];

