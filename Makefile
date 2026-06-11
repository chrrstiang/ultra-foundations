JAVA_SRC_DIR = src/java/main
CLANG_TIDY = /opt/homebrew/opt/llvm/bin/clang-tidy
SYSROOT = $(shell xcrun --show-sdk-path)
JAVA_OUT_DIR = out
CPP_BUILD_DIR = build
CPP_EXECUTABLE = $(CPP_BUILD_DIR)/ultra-foundations
PGM_IMAGE = baboon.ascii

# C++ targets
build-cpp:
	cmake -B $(CPP_BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && cmake --build $(CPP_BUILD_DIR)

clean-cpp:
	rm -rf $(CPP_BUILD_DIR)

# Java targets
build-java:
	find $(JAVA_SRC_DIR) -name "*.java" | xargs javac -d $(JAVA_OUT_DIR)

clean-java:
	rm -rf $(JAVA_OUT_DIR)

# Test targets
test-cpp: build-cpp
	cd $(CPP_BUILD_DIR) && ctest --output-on-failure

test-java:
	mvn test

test-java-bridge: build-cpp
	mvn verify

# Lint targets
lint-cpp: build-cpp
	find src/cpp/main -name "*.cpp" | xargs $(CLANG_TIDY) -p $(CPP_BUILD_DIR) --extra-arg="-isysroot$(SYSROOT)" --extra-arg="-std=c++17"

lint-java:
	mvn spotbugs:check

lint: lint-cpp lint-java

# Run targets
run-scan: build-cpp build-java
	java -Djava.library.path=$(CPP_BUILD_DIR) -cp $(JAVA_OUT_DIR) manager.Main

run-bridge: build-cpp build-java
	java -Djava.library.path=$(CPP_BUILD_DIR) -cp $(JAVA_OUT_DIR) bridge.NativeFilterBridge

run-pipeline: build-cpp
	./$(CPP_EXECUTABLE) $(PGM_IMAGE).pgm out.pgm

.PHONY: build-cpp clean-cpp build-java clean-java test-cpp test-java test-java-bridge lint-cpp lint-java lint run-scan run-bridge run-pipeline
