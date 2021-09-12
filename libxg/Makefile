ROOT=/opt/irix/root/usr
PREFIX=$(ROOT)/xg

install:
	@[ -f /opt/irix/sgug/bin/mips-sgi-irix6.5-gcc ] || (echo No /opt/irix/sgug/bin/mips-irix6.5-gcc && exit 1)
	@$(MAKE) -C src
	@mkdir -p $(PREFIX)/include $(PREFIX)/lib32
	@cp -r include/* $(PREFIX)/include
	@cp src/libxg.a $(PREFIX)/lib32
	@cp src/libxg.so $(PREFIX)/lib32
	@echo Installed into $(PREFIX)

