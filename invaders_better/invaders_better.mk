##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=invaders_better
ConfigurationName      :=Debug
WorkspaceConfiguration :=Static
WorkspacePath          :=D:/programacao/C/yasic
ProjectPath            :=D:/programacao/C/yasic/invaders_better
IntermediateDirectory  :=../build-$(WorkspaceConfiguration)/invaders_better
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=nicho
Date                   :=27/01/2024
CodeLitePath           :="C:/Program Files/CodeLite"
MakeDirCommand         :=mkdir
LinkerName             :=D:/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=D:/msys64/mingw64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=D:/programacao/C/yasic/build-$(WorkspaceConfiguration)/bin
OutputFile             :=..\build-$(WorkspaceConfiguration)\bin\$(ProjectName).exe
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=D:/msys64/mingw64/bin/windres.exe
LinkOptions            := -m64 -m64 -static -static-libstdc++ -static-libgcc
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)D:\libs\allegro5.2.9.1-x64-static\include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)allegro_monolith-static $(LibrarySwitch)winmm $(LibrarySwitch)shlwapi $(LibrarySwitch)ole32 $(LibrarySwitch)user32 $(LibrarySwitch)opengl32 $(LibrarySwitch)gdi32 $(LibrarySwitch)gdiplus $(LibrarySwitch)webp $(LibrarySwitch)dsound $(LibrarySwitch)flac $(LibrarySwitch)psapi $(LibrarySwitch)dumb $(LibrarySwitch)freetype $(LibrarySwitch)png16 $(LibrarySwitch)zlib $(LibrarySwitch)comdlg32 $(LibrarySwitch)sharpyuv $(LibrarySwitch)ogg 
ArLibs                 :=  "allegro_monolith-static" "winmm" "shlwapi" "ole32" "user32" "opengl32" "gdi32" "gdiplus" "webp" "dsound" "flac" "psapi" "dumb" "freetype" "png16" "zlib" "comdlg32" "sharpyuv" "ogg" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)D:\libs\allegro5.2.9.1-x64-static\lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := D:/msys64/mingw64/bin/ar.exe -r
CXX      := D:/msys64/mingw64/bin/g++.exe
CC       := D:/msys64/mingw64/bin/gcc.exe
CXXFLAGS :=  -gdwarf-2 -O0 -Wall $(Preprocessors)
CFLAGS   := -Wextra -Werror -Wno-unused-function -Wno-switch -Wuninitialized -Wstrict-prototypes -gdwarf-2 -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := D:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/main.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@if not exist "$(OutputDirectory)" $(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c 
	$(CC) $(SourceSwitch) "D:/programacao/C/yasic/invaders_better/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) main.c

##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


