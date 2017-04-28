all:
	gcc -g -m64 -ansi demo.c RecognizerImageWrapper.c -o demo -I ../libRecognizerApi/inc -L ../libRecognizerApi/lib/ -lRecognizerApi -ljpeg
	gcc -g -m64 -ansi raw-ocr-demo.c RecognizerImageWrapper.c -o raw-ocr-demo -I ../libRecognizerApi/inc -L ../libRecognizerApi/lib/ -lRecognizerApi -ljpeg

run: all
	LD_LIBRARY_PATH=../libRecognizerApi/lib ./demo croID.jpg
	LD_LIBRARY_PATH=../libRecognizerApi/lib ./demo deID.jpg
	LD_LIBRARY_PATH=../libRecognizerApi/lib ./raw-ocr-demo lipsum.jpg

clean:
	rm demo
	rm raw-ocr-demo
