export GOARCH=386
go test -v -count=1 ./loader/...
export GOARCH=amd64
go test -v -count=1 ./loader/...