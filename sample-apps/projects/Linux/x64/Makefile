DEMO_SRC_PATH := ../../../src/

API_RES_PATH := ../../../../resources
API_INC_PATH := ../../../../include
API_LIB_PATH := ../../../../lib/Linux/x64

FLAGS := -g -ansi -m64
CC    := gcc

MRTDDemo:
	$(CC) $(FLAGS) $(DEMO_SRC_PATH)/MRTD-demo/demo.c $(DEMO_SRC_PATH)/utils/Linux/RecognizerImageWrapper.c -o mrtd-demo -I $(API_INC_PATH) -I $(DEMO_SRC_PATH)/utils/Linux -L $(API_LIB_PATH) -lRecognizerApi -ljpeg

RawOCRDemo:
	$(CC) $(FLAGS) $(DEMO_SRC_PATH)/raw-OCR-demo/raw-ocr-demo.c $(DEMO_SRC_PATH)/utils/Linux/RecognizerImageWrapper.c -o raw-ocr-demo -I $(API_INC_PATH) -I $(DEMO_SRC_PATH)/utils/Linux -L $(API_LIB_PATH) -lRecognizerApi -ljpeg

TemplateAPIDemo:
	$(CC) $(FLAGS) $(DEMO_SRC_PATH)/Templating-demo/TemplateAPICroIDBackDemo.c $(DEMO_SRC_PATH)/Templating-demo/TemplateAPICroIDFrontDemo.c $(DEMO_SRC_PATH)/Templating-demo/TemplateAPIDemo.c $(DEMO_SRC_PATH)/Templating-demo/TemplateAPIUtils.c $(DEMO_SRC_PATH)/utils/Linux/RecognizerImageWrapper.c -o templating-demo -I $(API_INC_PATH) -I $(DEMO_SRC_PATH)/utils/Linux -L $(API_LIB_PATH) -lRecognizerApi -ljpeg

all: MRTDDemo RawOCRDemo TemplateAPIDemo

run: all
	LD_LIBRARY_PATH=$(API_LIB_PATH) ./mrtd-demo $(API_RES_PATH) $(DEMO_SRC_PATH)/MRTD-demo/croID.jpg $(DEMO_SRC_PATH)/MRTD-demo/deID.jpg
	LD_LIBRARY_PATH=$(API_LIB_PATH) ./raw-ocr-demo $(API_RES_PATH) $(DEMO_SRC_PATH)/raw-OCR-demo/lipsum.jpg
	LD_LIBRARY_PATH=$(API_LIB_PATH) ./templating-demo $(API_RES_PATH) $(DEMO_SRC_PATH)/Templating-demo/oldBack.jpg $(DEMO_SRC_PATH)/Templating-demo/newBack.jpg $(DEMO_SRC_PATH)/Templating-demo/oldFront.jpg $(DEMO_SRC_PATH)/Templating-demo/newFront.jpg

clean:
	rm mrtd-demo
	rm raw-ocr-demo
	rm templating-demo
