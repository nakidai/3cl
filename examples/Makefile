TESTS += fib
TESTS += gcd

all: ${TESTS}

.PHONY: ${TESTS}
${TESTS}:
	@echo "Test: $@"
	@test "`../3cl -d $@.ccl`" = "`cat $@.ccl.test`"
