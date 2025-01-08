.PHONY: all utest utestConf clean cleanTests gcovr lizard

BUILD_DIR = build
TEST_BUILD_DIR := ${BUILD_DIR}/utest

all: gcovr lizard

${TEST_BUILD_DIR}/Makefile:
	cmake \
		-B${TEST_BUILD_DIR} \
		-G "Unix Makefiles" \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DCMAKE_RULE_MESSAGES:BOOL=ON

utestConf: ${TEST_BUILD_DIR}/Makefile

utest: utestConf
	$(MAKE) -C ${TEST_BUILD_DIR} --no-print-directory

gcovr: utestConf
	$(MAKE) -C ${TEST_BUILD_DIR} coverage --no-print-directory

lizard: utestConf
	$(MAKE) -C ${TEST_BUILD_DIR} complexity --no-print-directory

cleanTests:
	@rm -rf $(TEST_BUILD_DIR)
	@rm -rf $(wildcard *.gcov) || true

clean: cleanTests
	@rm -rf $(BUILD_DIR)/${BUILD_TYPE}