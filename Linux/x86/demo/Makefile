all:
	gcc -m32 -ansi demo.c -o demo -I ../libRecognizerApi/inc -L ../libRecognizerApi/lib/ -lRecognizerApi

run: all
	LD_LIBRARY_PATH=../libRecognizerApi/lib ./demo barcode-image.png
	LD_LIBRARY_PATH=../libRecognizerApi/lib ./demo usdl-image.jpg

clean:
	rm demo
