SUBDIRS += dom/build/linux
SUBDIRS += parser/build/linux
SUBDIRS += value/build/linux
SUBDIRS += rpcserver/build/linux
 	
all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
