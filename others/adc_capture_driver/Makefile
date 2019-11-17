obj-m += axi_adc_dma.o

-include ../../config.mk

PWD ?= $(shell pwd)

.PHONY: all clean

all:
	make -C ${KBUILD_DIR} M=${PWD} modules
	mkdir -p ../../${OUTPUT_DIR}
	cp *.ko ../../${OUTPUT_DIR}

clean:
	rm -rf .tmp_versions .*.*.cmd *.ko *.mod.c *.o *.symvers *.order
