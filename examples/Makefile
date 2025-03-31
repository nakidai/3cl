TESTS += fac
TESTS += fib
TESTS += gcd
TESTS += mlc
TESTS += num

all: ${TESTS}

.PHONY: ${TESTS}
${TESTS}:
	@printf "Testing %s..." $@
	@echo " OK"
	@test "`../3cl -d $@.ccl < in/$@`" = "`cat out/$@`"
