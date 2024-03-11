##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=invaders_linux
ConfigurationName      :=Debug
WorkspaceConfiguration :=Static_Mingw32_x64
WorkspacePath          :=D:/projetos/c/invaders
ProjectPath            :=D:/projetos/c/invaders/invaders_linux
IntermediateDirectory  :=../build-$(WorkspaceConfiguration)/invaders_linux
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Web Master
Date                   :=3/11/2024
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
OutputFile             :=..\build-$(WorkspaceConfiguration)\bin\$(ProjectName)_lnx
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)/usr/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)allegro $(LibrarySwitch)allegro_font $(LibrarySwitch)allegro_audio $(LibrarySwitch)allegro_acodec $(LibrarySwitch)allegro_ttf $(LibrarySwitch)allegro_dialog $(LibrarySwitch)allegro_primitives $(LibrarySwitch)allegro_physfs $(LibrarySwitch)allegro_image $(LibrarySwitch)physfs $(LibrarySwitch)m 
ArLibs                 :=  "allegro" "allegro_font" "allegro_audio" "allegro_acodec" "allegro_ttf" "allegro_dialog" "allegro_primitives" "allegro_physfs" "allegro_image" "physfs" "m" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/usr/lib64 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := C:/msys64/mingw64/bin/ar.exe -r
CXX      := C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -Wextra -Wextra -Werror -Wno-unused-function -Wno-switch -Wuninitialized -Wstrict-prototypes $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/up_invaders_better_miniz.c$(ObjectSuffix) $(IntermediateDirectory)/up_invaders_better_main.c$(ObjectSuffix) 



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
$(IntermediateDirectory)/up_invaders_better_miniz.c$(ObjectSuffix): ../invaders_better/miniz.c 
	$(CC) $(SourceSwitch) "D:/projetos/c/invaders/invaders_better/miniz.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_invaders_better_miniz.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_invaders_better_miniz.c$(PreprocessSuffix): ../invaders_better/miniz.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_invaders_better_miniz.c$(PreprocessSuffix) ../invaders_better/miniz.c

$(IntermediateDirectory)/up_invaders_better_main.c$(ObjectSuffix): ../invaders_better/main.c 
	$(CC) $(SourceSwitch) "D:/projetos/c/invaders/invaders_better/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_invaders_better_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_invaders_better_main.c$(PreprocessSuffix): ../invaders_better/main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_invaders_better_main.c$(PreprocessSuffix) ../invaders_better/main.c

##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


