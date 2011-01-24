
all: ./build

./build:
	node-waf configure build

clean:
	rm -rf ./build
