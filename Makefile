NANOPROTOC = $(NANOPB)/generator-bin/protoc --proto_path=api --nanopb_out=device
GOPROTOC = protoc --gofast_out=app

.PHONY: proto

all: proto

proto:
	$(NANOPROTOC) api/*.proto
	$(GOPROTOC) api/*.proto
