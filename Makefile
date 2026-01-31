build:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build

test:
	cmake -S . -B build -DENABLE_SANITIZERS=ON
	cmake --build build
	ctest --test-dir build --output-on-failure

run: build
	./build/banking_app

clean:
	rm -rf build
