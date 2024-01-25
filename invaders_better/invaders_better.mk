##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=invaders_better
ConfigurationName      :=Debug
WorkspaceConfiguration :=Debug
WorkspacePath          :=D:/projetos/c/invaders
ProjectPath            :=D:/projetos/c/invaders/invaders_better
IntermediateDirectory  :=../build-$(WorkspaceConfiguration)/invaders_better
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Web Master
Date                   :=1/25/2024
CodeLitePath           :="C:/Program Files/CodeLite"
MakeDirCommand         :=mkdir
LinkerName             :=C:/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/msys64/mingw64/bin/g++.exe -shared -fPIC
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
OutputDirectory        :=D:/projetos/c/invaders/build-$(WorkspaceConfiguration)/bin
OutputFile             :=..\build-$(WorkspaceConfiguration)\bin\$(ProjectName).exe
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            := -m64 -static 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)D:\projetos\libs\liballegro-5.2.9-git-release-static\include $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)allegro_monolith-static $(LibrarySwitch)winmm $(LibrarySwitch)shlwapi $(LibrarySwitch)ole32 $(LibrarySwitch)shell32 $(LibrarySwitch)user32 $(LibrarySwitch)stdc++ $(LibrarySwitch)opengl32 $(LibrarySwitch)gdi32 $(LibrarySwitch)gdiplus $(LibrarySwitch)webp $(LibrarySwitch)dsound $(LibrarySwitch)flac $(LibrarySwitch)psapi $(LibrarySwitch)dumb $(LibrarySwitch)freetype $(LibrarySwitch)png16 $(LibrarySwitch)zlib $(LibrarySwitch)comdlg32 $(LibrarySwitch)vorbis $(LibrarySwitch)ogg $(LibrarySwitch)opus $(LibrarySwitch)sharpyuv $(LibrarySwitch)vorbis 
ArLibs                 :=  "allegro_monolith-static" "winmm" "shlwapi" "ole32" "shell32" "user32" "stdc++" "opengl32" "gdi32" "gdiplus" "webp" "dsound" "flac" "psapi" "dumb" "freetype" "png16" "zlib" "comdlg32" "vorbis" "ogg" "opus" "sharpyuv" "vorbis" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)D:\projetos\libs\liballegro-5.2.9-git-release-static\lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := C:/msys64/mingw64/bin/ar.exe -r
CXX      := C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS :=  -gdwarf-2 -O0 -Wall $(Preprocessors)
CFLAGS   := -Wextra -Wall -Werror -Wno-unused-function -Wno-switch -Wuninitialized -Wstrict-prototypes -gdwarf-2 -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


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
	$(CC) $(SourceSwitch) "D:/projetos/c/invaders/invaders_better/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) main.c

##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


