BUILD_DIR = build
INSTALL_DIR = bin

.PHONY: all install uninstall clean dvi dist tests coverage

all: install tests

build:
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR) --config Release

install: build
	cmake --install $(BUILD_DIR) --prefix $(INSTALL_DIR)
	@echo "Installed to $(INSTALL_DIR)/"

uninstall:
	rm -rf $(INSTALL_DIR)
	@echo "Uninstalled from $(INSTALL_DIR)/"

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(INSTALL_DIR)
	rm -rf dist

dvi:
	@if command -v open >/dev/null 2>&1; then \
		open README.md; \
	elif command -v xdg-open >/dev/null 2>&1; then \
		xdg-open README.md; \
	else \
		echo "Please open README.md manually"; \
	fi

dist: clean
	mkdir -p dist
	tar -czf dist/s21_maze_$(shell date +%Y%m%d).tar.gz \
		--exclude='.git' \
		--exclude='dist' \
		--exclude='build' \
		--exclude='bin' \
		.
	@echo "Created dist/s21_maze_$(shell date +%Y%m%d).tar.gz"

tests:
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
	cmake --build $(BUILD_DIR)
	cd $(BUILD_DIR) && ctest --output-on-failure

coverage:
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DBUILD_COVERAGE=ON
	cmake --build $(BUILD_DIR)
	cd $(BUILD_DIR) && $(MAKE) coverage
