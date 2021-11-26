##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=invaders
ConfigurationName      :=Release
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=D:/projetos/c/invaders
ProjectPath            :=D:/projetos/c/invaders/invaders
IntermediateDirectory  :=../build-$(ConfigurationName)/invaders
OutDir                 :=../build-$(ConfigurationName)/invaders
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Web Master
Date                   :=26/11/2021
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/MinGW/bin/g++.exe
SharedObjectLinkerName :=C:/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=..\build-$(ConfigurationName)\bin\$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)src $(IncludeSwitch)D:\projetos\libs\allegro-5.2.7\include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)allegro_monolith.dll 
ArLibs                 :=  "allegro_monolith.dll.a" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)D:\projetos\libs\allegro-5.2.7\lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=  -O4 -Wall -s $(Preprocessors)
CFLAGS   :=  -O4 -Wall -s $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=../build-$(ConfigurationName)/invaders/main.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ../build-$(ConfigurationName)/invaders/.d $(Objects) 
	@if not exist "..\build-$(ConfigurationName)\invaders" mkdir "..\build-$(ConfigurationName)\invaders"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@if not exist "..\build-$(ConfigurationName)\invaders" mkdir "..\build-$(ConfigurationName)\invaders"
	@if not exist ""..\build-$(ConfigurationName)\bin"" mkdir ""..\build-$(ConfigurationName)\bin""

../build-$(ConfigurationName)/invaders/.d:
	@if not exist "..\build-$(ConfigurationName)\invaders" mkdir "..\build-$(ConfigurationName)\invaders"

PreBuild:


##
## Objects
##
../build-$(ConfigurationName)/invaders/main.c$(ObjectSuffix): main.c ../build-$(ConfigurationName)/invaders/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/projetos/c/invaders/invaders/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/invaders/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/invaders/main.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/invaders/main.c$(DependSuffix) -MM main.c

../build-$(ConfigurationName)/invaders/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/invaders/main.c$(PreprocessSuffix) main.c


-include ../build-$(ConfigurationName)/invaders//*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


