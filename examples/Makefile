TESTS += cnt
TESTS += fac
TESTS += fib
TESTS += gcd
TESTS += mlc
TESTS += num

export ASAN_OPTIONS=detect_leaks=0

all: ${TESTS}

.PHONY: ${TESTS}
${TESTS}:
	@printf "Testing %s..." $@
	@echo " OK"
	@test "`../3cl -d $@.ccl < in/$@`" = "`cat out/$@`"
