SUBDIRS += ../../rexjson/build/unix
SUBDIRS += ../../tests/dom/build/unix
SUBDIRS += ../../tests/parser/build/unix
SUBDIRS += ../../tests/value/build/unix
SUBDIRS += ../../tests/rpcserver/build/unix
 	
all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
