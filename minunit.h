/*
 * MinUnit - a minimal unit testing framework for C
 * Credits go to John Brewer: http://www.jera.com/techinfo/jtns/jtn002.html
 */

#ifndef _MINUNIT_H
#define	_MINUNIT_H

#define mu_assert(message, test) do { if (!(test)) { tests_failed++; return message; } } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
int tests_run;
int tests_failed;


#endif	/* _MINUNIT_H */

