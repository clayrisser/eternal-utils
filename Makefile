CWD := $(shell pwd)
NAME := $(shell pwd | grep -Po "[^\/]*$$")
CMD := cat configure.ac | grep -Po "(?<=AC_INIT\(\[$(NAME)],\s\[).*(?=],)"
VERSION := $(shell $(CMD))

.PHONY: all
all: clean build

.PHONY: eternal-export
eternal-export: make break
	@dist/bin/eternal-export HELLO=WORLD

.PHONY: eternal-alias
eternal-alias: make break
	@dist/bin/eternal-alias hello="echo world"

.PHONY: break
break:
	@echo --------

.PHONY: make
make: clean dist
	@cd dist && ./configure
	@cd dist && make

.PHONY: clean
clean:
	-@rm -r $(NAME) \
		*.cache \
		*.in \
		*.log \
		*.tar.gz \
		aclocal.m4 \
		bin \
		compile \
		config.status \
		configure \
		dist \
		depcomp \
		install-sh \
		missing \
		src/*.in \
		src/*.o \
		src/.deps \
		src/Makefile 2>/dev/null || true

.PHONY: autoreconf
autoreconf: dist

dist:
	@rsync -rv --exclude=Makefile --exclude=.git --exclude=dist ./ dist
	@cd dist && autoreconf --install
	@rm -r dist/*.cache

.PHONY: build
build: $(NAME)_$(VERSION).tar.gz

$(NAME)_$(VERSION).tar.gz: dist
	@mv dist $(NAME)
	@tar -czvf $(NAME)_$(VERSION).tar.gz $(NAME)
	@mv $(NAME) dist
