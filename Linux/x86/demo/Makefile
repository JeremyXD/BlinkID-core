all:
	gcc -m32 -ansi demo.c -o demo -I ../libRecognizerApi/inc -L ../libRecognizerApi/lib/ -lRecognizerApi

run: all
	LD_LIBRARY_PATH=../libRecognizerApi/lib ./demo croID.jpg
	LD_LIBRARY_PATH=../libRecognizerApi/lib ./demo deID.jpg

clean:
	rm demo
