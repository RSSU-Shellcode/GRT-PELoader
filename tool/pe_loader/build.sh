export GOOS=windows
go build -v -trimpath -ldflags "-s -w" -o GRT-PELoader.exe main.go