package main

import (
	"fmt"

	"github.com/RSSU-Shellcode/Gleam-RT/runtime"
)

func main() {
	fmt.Println("host process will be exited")

	gleamrt.ExitProcess(0)
}
