DEMO_SRC_PATH := ../../../src/

API_RES_PATH := ../../../../resources
API_INC_PATH := ../../../../include
API_LIB_PATH := ../../../../lib/Linux/x86

FLAGS := -g -ansi -m32
CC    := gcc

all: MRTDDemo RawOCRDemo TemplateAPIDemo


MRTDDemo:
	$(CC) $(FLAGS) $(DEMO_SRC_PATH)/MRTD-demo/demo.c $(DEMO_SRC_PATH)/utils/Linux/RecognizerImageWrapper.c -o mrtd-demo -I $(API_INC_PATH) -I $(DEMO_SRC_PATH)/utils/Linux -L $(API_LIB_PATH) -lRecognizerApi -ljpeg

RawOCRDemo:
	$(CC) $(FLAGS) $(DEMO_SRC_PATH)/raw-OCR-demo/raw-ocr-demo.c $(DEMO_SRC_PATH)/utils/Linux/RecognizerImageWrapper.c -o raw-ocr-demo -I $(API_INC_PATH) -I $(DEMO_SRC_PATH)/utils/Linux -L $(API_LIB_PATH) -lRecognizerApi -ljpeg

TemplateAPIDemo:
	$(CC) $(FLAGS) $(DEMO_SRC_PATH)/Templating-demo/TemplateAPICroIDBackDemo.c $(DEMO_SRC_PATH)/Templating-demo/TemplateAPICroIDFrontDemo.c $(DEMO_SRC_PATH)/Templating-demo/TemplateAPIDemo.c $(DEMO_SRC_PATH)/Templating-demo/TemplateAPIUtils.c $(DEMO_SRC_PATH)/utils/Linux/RecognizerImageWrapper.c -o templating-demo -I $(API_INC_PATH) -I $(DEMO_SRC_PATH)/utils/Linux -L $(API_LIB_PATH) -lRecognizerApi -ljpeg

run-MRTDDemo: MRTDDemo
	LD_LIBRARY_PATH=$(API_LIB_PATH) ./mrtd-demo $(API_RES_PATH) $(DEMO_SRC_PATH)/MRTD-demo/croID.jpg $(DEMO_SRC_PATH)/MRTD-demo/deID.jpg

run-RawOCRDemo: RawOCRDemo
	LD_LIBRARY_PATH=$(API_LIB_PATH) ./raw-ocr-demo $(API_RES_PATH) $(DEMO_SRC_PATH)/raw-OCR-demo/lipsum.jpg

run-TemplateAPIDemo: TemplateAPIDemo
	LD_LIBRARY_PATH=$(API_LIB_PATH) ./templating-demo $(API_RES_PATH) $(DEMO_SRC_PATH)/Templating-demo/oldBack.jpg $(DEMO_SRC_PATH)/Templating-demo/newBack.jpg $(DEMO_SRC_PATH)/Templating-demo/oldFront.jpg $(DEMO_SRC_PATH)/Templating-demo/newFront.jpg

run: run-MRTDDemo run-RawOCRDemo run-TemplateAPIDemo

clean:
	rm mrtd-demo || true
	rm raw-ocr-demo || true
	rm templating-demo || true
