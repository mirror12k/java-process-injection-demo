


docker:
	docker build -t java-injector .
	docker run -v ${CURDIR}/bin:/work/bin -it java-injector make build

build:
	-mkdir bin
	javac -d bin -h inc -cp bin src/Demo.java
	x86_64-w64-mingw32-gcc -m64 -Wall -D_JNI_IMPLEMENTATION_ -D_IS64_ -Wl,--kill-at -shared -I inc -I inc/win32 src/Demo.c -o bin/injector.dll
	ls -la /work/bin

