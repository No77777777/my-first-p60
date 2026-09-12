#include <srtest.h>

/**
 * IPA Test Engine UL aggregation test
 */
class ipa_test_engine_ul_aggr
{
public:
    ipa_test_engine_ul_aggr()
    {}
    
    void setup();

    void teardown();
    
    /**
     * test case 0001
     */
    void test0001();

    /**
     * test case 0002
     */
    void test0002();

    /**
     * test case 0003
     */
    void test0003();

    /**
     * test case 0004
     */
    void test0004();

    /**
     * test case 0005
     */
    void test0005();

    /**
     * test case 0006
     */
    void test0006();

    /**
     * test case 0007
     */
    void test0007();

    /**
     * test case 0008
     */
    void test0008();

    /**
     * test case 0009
     */
    void test0009();

    /**
     * test case 0010
     */
    void test0010();

    /**
     * test case 0011
     */
    void test0011();

    /**
     * test case 0012
     */
    void test0012();

    /**
     * test case 0013
     */
    void test0013();

    /**
     * test case 0014
     */
    void test0014();

    /**
     * test case 0015
     */
    void test0015();
};

/**
 * IPA Test Engine DL aggregation test
 */
class ipa_test_engine_dl_aggr
{
public:
    ipa_test_engine_dl_aggr()
    {}
    
    void setup();

    void teardown();
    
    /**
     * test case 0001
     */
    void test0001();

    /**
     * test case 0002
     */
    void test0002();

    /**
     * test case 0003
     */
    void test0003();

    /**
     * test case 0004
     */
    void test0004();

    /**
     * test case 0005
     */
    void test0005();

    /**
     * test case 0006
     */
    void test0006();

    /**
     * test case 0007
     */
    void test0007();

    /**
     * test case 0008
     */
    void test0008();

    /**
     * test case 0009
     */
    void test0009();

    /**
     * test case 0010
     */
    void test0010();

    /**
     * test case 0011
     */
    void test0011();

    /**
     * test case 0012
     */
    void test0012();

    /**
     * test case 0013
     */
    void test0013();

    /**
     * test case 0014
     */
    void test0014();

    /**
     * test case 0015
     */
    void test0015();
};

/**
 * IPA Test Engine Loopback aggregation test
 */
class ipa_test_engine_loopback_aggr
{
public:
    ipa_test_engine_loopback_aggr()
    {}
    
    void setup();

    void teardown();
    
    /**
     * test case 0001
     */
    void test0001();

    /**
     * test case 0002
     */
    void test0002();

    /**
     * test case 0003
     */
    void test0003();

    /**
     * test case 0004
     */
    void test0004();

    /**
     * test case 0005
     */
    void test0005();

    /**
     * test case 0006
     */
    void test0006();

    /**
     * test case 0007
     */
    void test0007();

    /**
     * test case 0008
     */
    void test0008();

    /**
     * test case 0009
     */
    void test0009();

    /**
     * test case 0010
     */
    void test0010();

    /**
     * test case 0011
     */
    void test0011();

    /**
     * test case 0012
     */
    void test0012();

    /**
     * test case 0013
     */
    void test0013();

    /**
     * test case 0014
     */
    void test0014();

    /**
     * test case 0015
     */
    void test0015();
};

#ifdef _SCL
  #pragma scl_test_class(ipa_test_engine_ul_aggr)
  #pragma scl_test_setup(ipa_test_engine_ul_aggr, setup)
  #pragma scl_test_teardown(ipa_test_engine_ul_aggr, teardown)

  #pragma scl_test_class(ipa_test_engine_dl_aggr)
  #pragma scl_test_setup(ipa_test_engine_dl_aggr, setup)
  #pragma scl_test_teardown(ipa_test_engine_dl_aggr, teardown)

  #pragma scl_test_class(ipa_test_engine_loopback_aggr)
  #pragma scl_test_setup(ipa_test_engine_loopback_aggr, setup)
  #pragma scl_test_teardown(ipa_test_engine_loopback_aggr, teardown)
#endif
