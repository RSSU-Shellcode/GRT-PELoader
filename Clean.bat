rd  /S /Q ".vs\GRT-PELoader\v17\ipch"
del /S /Q ".vs\GRT-PELoader\v17\Browse.VC.db"
del /S /Q ".vs\GRT-PELoader\v17\Solution.VC.db"

rd /S /Q "Debug"
rd /S /Q "Release"
rd /S /Q "x64"
rd /S /Q "x86"

rd /S /Q "builder\Debug"
rd /S /Q "builder\Release"
rd /S /Q "builder\x64"
rd /S /Q "builder\x86"

rd /S /Q "cutter\Debug"
rd /S /Q "cutter\Release"
rd /S /Q "cutter\x64"
rd /S /Q "cutter\x86"

rd /S /Q "test\Debug"
rd /S /Q "test\Release"
rd /S /Q "test\x64"
rd /S /Q "test\x86"

rd /S /Q "tool\sc_loader\Debug"
rd /S /Q "tool\sc_loader\Release"
rd /S /Q "tool\sc_loader\x64"
rd /S /Q "tool\sc_loader\x86"