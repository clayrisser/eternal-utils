NAME := eternal-utils
PATCH := 1
PPA := ppa:codejamninja/jam-os
VERSION := 0.2.0

.PHONY: all
all: clean

.PHONY: clean
clean:
	@git clean -fXd

.PHONY: clone
clone: build.tar.gz
build.tar.gz:
	@rsync -avz --exclude .git --exclude ubuntu .. build
	@cd .. && echo `git log -1 --pretty=%B` > ubuntu/message
	@tar -czvf build.tar.gz build
	@rm -rf build

setup: clone build_$(VERSION).orig.tar.gz
build_$(VERSION).orig.tar.gz:
	@(sleep 5; xdotool key s)&
	@(sleep 8; xdotool key y)&
	@bzr dh-make build $(VERSION) build.tar.gz
	@rm -rf build/debian
	@cp -r debian build/debian
	@cd build && bzr add .
	@cd build && bzr commit -m "`cat ../message`"
	@rm message

.PHONY: test
test: setup
	@cd build && bzr builddeb -- -us -uc
	@lesspipe *.deb
	-@lintian *.dsc
	-@lintian *.deb

.PHONY: build
build: test
	@cd build && bzr builddeb -- -nc -us -uc
	@cd build && bzr builddeb -S

.PHONY: publish
publish: build
	@dput $(PPA) $(NAME)_$(VERSION)-$(PATCH)_source.changes
