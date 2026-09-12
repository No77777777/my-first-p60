#pragma once
#include "q6zip_test_scenario_init.h"
#include "q6zip_test_scenario_unzip_ro.h"
#include "q6zip_test_scenario_unzip_rw.h"
#include "q6zip_test_scenario_zip_rw.h"
#include "q6zip_test_scenario_error_ro.h"
#include "q6zip_test_scenario_mixed.h"
#include "q6zip_test_scenario_copy.h"

static q6zip_test_scenario_t * q6zip_test_registry[] = {
    &q6zip_test_scenario_init,
    &q6zip_test_scenario_unzip_ro,
    &q6zip_test_scenario_unzip_rw,
    &q6zip_test_scenario_zip_rw,
    &q6zip_test_scenario_error_ro,
    &q6zip_test_scenario_mixed,
    &q6zip_test_scenario_copy
};
