default: check examples

check:
	$(MAKE) -C tests $@

examples:
	$(MAKE) -C examples

.PHONY: default check examples
