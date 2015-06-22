#include "tree/balanced_tree.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


#define SIZE_LARGE_TEST     10000
int32_t aDataFst[SIZE_LARGE_TEST];
int32_t aDataSnd[SIZE_LARGE_TEST];


/*===========================================================================*
 *              Test suite to manipulate primitive type input                *
 *===========================================================================*/
void TestPrimInsertBase();
void TestPrimInsertLarge();
void TestPrimDeleteLarge();
void TestPrimSearchAndBoundary();
int32_t CompareItemPrimitive(const void*, const void*);


int32_t SuitePrimitive()
{
    CU_pSuite pSuite = CU_add_suite("Primitive Type Input", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Simple item insertion and structure verification",
                     TestPrimInsertBase);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Bulk item insertion and structure verification",
            TestPrimInsertLarge);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Bulk item deletion and structure verification",
            TestPrimDeleteLarge);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item search and boundary case handling",
            TestPrimSearchAndBoundary);
    if (!pTest)
        return ERR_NOMEM;

    return SUCC;
}


int32_t main()
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();
    assert(CU_get_registry() != NULL);
    assert(!CU_is_test_running());

    /* Prepare the large dataset. */
    srand(time(NULL));
    int32_t idxFst = 0, idxSnd;
    while (idxFst < SIZE_LARGE_TEST) {
        int32_t iRand;
        bool bDup = true;
        do {
            iRand = rand() % SIZE_LARGE_TEST;
            for (idxSnd = 0 ; idxSnd < idxFst ; idxSnd++) {
                if (iRand == aDataFst[idxSnd])
                    break;
            }
            if (idxSnd == idxFst)
                bDup = false;
        } while (bDup);
        aDataFst[idxFst] = iRand;
        aDataSnd[idxFst] = iRand;
        idxFst++;
    }

    /* Prepare the test suite for primitive input. */
    if (SuitePrimitive() != SUCC) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Launch all the tests. */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return SUCC;
}


void TestPrimInsertBase()
{
    BalancedTree *pTree;
    CU_ASSERT(BalTreeInit(&pTree) == SUCC);

    /**
     * The vision of the test tree after maintaining the balanced tree property.
     *          9
     *        /   \
     *      4      15
     *     / \     / \
     *    1   6   10  22
     *         \      / \
     *          7   20   25
     */
    CU_ASSERT(pTree->insert(pTree, (Item)10, true) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)15, true) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)20, true) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)25, true) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)22, true) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)9, true) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)6, true) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)1, true) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)4, true) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)7, true) == SUCC);

    /* Check structure correctness. */
    Item item;
    CU_ASSERT(pTree->predecessor(pTree, (Item)4, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pTree->successor(pTree, (Item)4, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);

    CU_ASSERT(pTree->predecessor(pTree, (Item)6, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pTree->successor(pTree, (Item)6, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)7);

    CU_ASSERT(pTree->predecessor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);
    CU_ASSERT(pTree->successor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)9);

    CU_ASSERT(pTree->predecessor(pTree, (Item)9, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)7);
    CU_ASSERT(pTree->successor(pTree, (Item)9, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)10);

    CU_ASSERT(pTree->predecessor(pTree, (Item)10, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)9);
    CU_ASSERT(pTree->successor(pTree, (Item)10, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)15);

    CU_ASSERT(pTree->predecessor(pTree, (Item)15, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)10);
    CU_ASSERT(pTree->successor(pTree, (Item)15, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)20);

    CU_ASSERT(pTree->predecessor(pTree, (Item)20, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)15);
    CU_ASSERT(pTree->successor(pTree, (Item)20, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)22);

    CU_ASSERT(pTree->predecessor(pTree, (Item)22, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)20);
    CU_ASSERT(pTree->successor(pTree, (Item)22, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)25);

    /* Check the minimum and maximum item. */
    CU_ASSERT(pTree->minimum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pTree->maximum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)25);

    /* Check the container size. */
    CU_ASSERT_EQUAL(pTree->size(pTree), 10);

    BalTreeDeinit(&pTree, true);
}


void TestPrimInsertLarge()
{
    BalancedTree *pTree;
    CU_ASSERT(BalTreeInit(&pTree) == SUCC);

    /* Bulk data insertion. */
    int32_t idx;
    for (idx = 0 ; idx < SIZE_LARGE_TEST ; idx++)
        #if __x86_64__
        CU_ASSERT(pTree->insert(pTree, (Item)(int64_t)aDataFst[idx], true) == SUCC);
        #else
        CU_ASSERT(pTree->insert(pTree, (Item)aDataFst[idx], true) == SUCC);
        #endif

    /* Check structure correctness. */
    qsort(aDataFst, SIZE_LARGE_TEST, sizeof(int32_t), CompareItemPrimitive);
    for (idx = 1 ; idx < SIZE_LARGE_TEST - 1 ; idx++) {
        Item itemIn, itemOut;
        #if __x86_64__
        itemIn = (Item)(int64_t)aDataFst[idx];
        CU_ASSERT(pTree->predecessor(pTree, itemIn, &itemOut) == SUCC);
        CU_ASSERT_EQUAL(itemOut, (Item)(int64_t)aDataFst[idx - 1]);
        CU_ASSERT(pTree->successor(pTree, itemIn, &itemOut) == SUCC);
        CU_ASSERT_EQUAL(itemOut, (Item)(int64_t)aDataFst[idx + 1]);
        #else
        itemIn = (Item)aDataFst[idx];
        CU_ASSERT(pTree->predecessor(pTree, itemIn, &itemOut) == SUCC);
        CU_ASSERT_EQUAL(itemOut, (Item)aDataFst[idx - 1]);
        CU_ASSERT(pTree->successor(pTree, itemIn, &itemOut) == SUCC);
        CU_ASSERT_EQUAL(itemOut, (Item)aDataFst[idx + 1]);
        #endif
    }

    BalTreeDeinit(&pTree, true);
}


void TestPrimDeleteLarge()
{
    BalancedTree *pTree;
    CU_ASSERT(BalTreeInit(&pTree) == SUCC);

    /* Bulk data insertion. */
    int32_t idx;
    for (idx = 0 ; idx < SIZE_LARGE_TEST ; idx++)
        #if __x86_64__
        CU_ASSERT(pTree->insert(pTree, (Item)(int64_t)aDataSnd[idx], true) == SUCC);
        #else
        CU_ASSERT(pTree->insert(pTree, (Item)aDataSnd[idx]), true) == SUCC);
        #endif

    /* Bulk data deletion. */
    for (idx = SIZE_LARGE_TEST/2 ; idx < SIZE_LARGE_TEST ; idx++)
        #if __x86_64__
        CU_ASSERT(pTree->delete(pTree, (Item)(int64_t)aDataSnd[idx], true) == SUCC);
        #else
        CU_ASSERT(pTree->delete(pTree, (Item)aDataSnd[idx]), true) == SUCC);
        #endif

    /* Check structure correctness. */
    qsort(aDataSnd, SIZE_LARGE_TEST/2, sizeof(int32_t), CompareItemPrimitive);
    for (idx = 1 ; idx < SIZE_LARGE_TEST/2 - 1 ; idx++) {
        Item itemIn, itemOut;
        #if __x86_64__
        itemIn = (Item)(int64_t)aDataSnd[idx];
        CU_ASSERT(pTree->predecessor(pTree, itemIn, &itemOut) == SUCC);
        CU_ASSERT_EQUAL(itemOut, (Item)(int64_t)aDataSnd[idx - 1]);
        CU_ASSERT(pTree->successor(pTree, itemIn, &itemOut) == SUCC);
        CU_ASSERT_EQUAL(itemOut, (Item)(int64_t)aDataSnd[idx + 1]);
        #else
        itemIn = (Item)aDataSnd[idx];
        CU_ASSERT(pTree->predecessor(pTree, itemIn, &itemOut) == SUCC);
        CU_ASSERT_EQUAL(itemOut, (Item)aDataSnd[idx - 1]);
        CU_ASSERT(pTree->successor(pTree, itemIn, &itemOut) == SUCC);
        CU_ASSERT_EQUAL(itemOut, (Item)aDataSnd[idx + 1]);
        #endif
    }

    BalTreeDeinit(&pTree, true);
}


void TestPrimSearchAndBoundary()
{
    BalancedTree *pTree;
    CU_ASSERT(BalTreeInit(&pTree) == SUCC);

    /* Search for the empty tree. */
    Item item;
    CU_ASSERT(pTree->search(pTree, (Item)0, &item) == ERR_NODATA);
    CU_ASSERT_EQUAL(item, NULL);

    /* Search for the real data. */
    CU_ASSERT(pTree->insert(pTree, (Item)1, true) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)0, true) == SUCC);
    CU_ASSERT(pTree->search(pTree, (Item)0, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);

    CU_ASSERT(pTree->delete(pTree, (Item)0, true) == SUCC);
    CU_ASSERT(pTree->search(pTree, (Item)0, &item) == ERR_NODATA);
    CU_ASSERT_EQUAL(item, NULL);

    /* Test boundary cases. */
    CU_ASSERT(pTree->predecessor(pTree, (Item)0, &item) == ERR_NODATA);
    CU_ASSERT(pTree->successor(pTree, (Item)0, &item) == ERR_NODATA);
    CU_ASSERT(pTree->predecessor(pTree, (Item)1, &item) == ERR_NODATA);
    CU_ASSERT(pTree->successor(pTree, (Item)1, &item) == ERR_NODATA);

    CU_ASSERT(pTree->delete(pTree, (Item)1, true) == SUCC);
    CU_ASSERT(pTree->maximum(pTree, &item) == ERR_IDX);
    CU_ASSERT(pTree->minimum(pTree, &item) == ERR_IDX);
    CU_ASSERT(pTree->delete(pTree, (Item)1, true) == ERR_NODATA);

    BalTreeDeinit(&pTree, true);
}


int32_t CompareItemPrimitive(const void *p_Src, const void *p_Tge)
{
    int32_t iSrc = *(int32_t*)p_Src;
    int32_t iTge = *(int32_t*)p_Tge;

    if (iSrc == iTge)
        return 0;
    return (iSrc > iTge)? 1 : -1;
}
