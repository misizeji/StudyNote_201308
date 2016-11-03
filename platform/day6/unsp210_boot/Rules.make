unexport O_TARGET
unexport obj-y
unexport subdirs
unexport SUBDIRS

ifdef O_TARGET
$(O_TARGET): $(obj-y)
	rm -f $@
	$(LD) -r -o $@ $^
endif # O_TARGET

%.o:%.c
	$(CC) -Wall -c -O2 -o $@ $< $(DIR_INCLUDE)

%.o:%.S
	$(CC) -Wall -c -O2 -o $@ $< $(DIR_INCLUDE)
	
#��Ŀ¼makefile�ĵݹ����:�����µ�Ŀ¼ʱҪ���������
SUBDIRS=arch init
.PHONY:subdirs $(SUBDIRS)
subdirs:$(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@		
