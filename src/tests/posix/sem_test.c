/**
 * @file
 * @brief Tests for POSIX semaphore.
 *
 * @date 20.03.02
 * @author Weixuan XIAO
 */

#include <errno.h>
#include <semaphore.h>
#include <embox/test.h>

EMBOX_TEST_SUITE("posix/semaphore test");

#define SEMAPHORE_INIT_VALUE 5

TEST_CASE("Test sem_init with shared=0, value=0") {
	sem_t semaphore;

	test_assert_equal(sem_init(&semaphore, 0, 0), ENOERR);
}

TEST_CASE("Test sem_init with shared=1, value=0") {
	sem_t semaphore;

	test_assert_equal(sem_init(&semaphore, 1, 0), ENOERR);
}

TEST_CASE("Test sem_init with shared=0, value=1") {
	sem_t semaphore;

	test_assert_equal(sem_init(&semaphore, 0, 1), ENOERR);
}

TEST_CASE("Test sem_init with shared=1, value=1") {
	sem_t semaphore;

	test_assert_equal(sem_init(&semaphore, 1, 1), ENOERR);
}

TEST_CASE("Test sem_destroy") {
	sem_t semaphore;

    // Up to now, sem_destroy does nothing but return ENOERR
	test_assert_equal(sem_destroy(&semaphore), ENOERR);
}

TEST_CASE("Test sem_getvalue") {
	sem_t semaphore;
    int value = -1;

	test_assert_equal(sem_init(&semaphore, 0, SEMAPHORE_INIT_VALUE), ENOERR);
    test_assert_equal(value, -1);                       // Test before getvalue
    test_assert_equal(sem_getvalue(&semaphore, &value), ENOERR);
    test_assert_equal(value, SEMAPHORE_INIT_VALUE); // Test after getvalue
}

TEST_CASE("Test sem_wait") {
	sem_t semaphore;
    int value = -1;

	test_assert_equal(sem_init(&semaphore, 0, SEMAPHORE_INIT_VALUE), ENOERR);
    test_assert_equal(value, -1);
    test_assert_equal(sem_getvalue(&semaphore, &value), ENOERR);
    test_assert_equal(value, SEMAPHORE_INIT_VALUE); 	// Test before sem_wait
    
    test_assert_equal(sem_wait(&semaphore), ENOERR);
    test_assert_equal(sem_getvalue(&semaphore, &value), ENOERR);
    test_assert_equal(value, SEMAPHORE_INIT_VALUE - 1); // Test after sem_wait
}

TEST_CASE("Test sem_post") {
	sem_t semaphore;
    int value = -1;

	test_assert_equal(sem_init(&semaphore, 0, SEMAPHORE_INIT_VALUE), ENOERR);
    test_assert_equal(value, -1);
    test_assert_equal(sem_getvalue(&semaphore, &value), ENOERR);
    test_assert_equal(value, SEMAPHORE_INIT_VALUE); 	// Test before sem_wait
    
    test_assert_equal(sem_wait(&semaphore), ENOERR);
    test_assert_equal(sem_getvalue(&semaphore, &value), ENOERR);
    test_assert_equal(value, SEMAPHORE_INIT_VALUE - 1); // Test after sem_wait

	test_assert_equal(sem_post(&semaphore), ENOERR);
    test_assert_equal(sem_getvalue(&semaphore, &value), ENOERR);
    test_assert_equal(value, SEMAPHORE_INIT_VALUE); 	// Test after sem_post
}

/*
extern int    sem_timedwait(sem_t *, const struct timespec *); //this
extern int    sem_trywait(sem_t *); //OK
*/

/*
TODO: Not yet implemented:
extern int    sem_close(sem_t *);
extern sem_t *sem_open(const char *, int, ...);
extern int    sem_unlink(const char *);
*/
