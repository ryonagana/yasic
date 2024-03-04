##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=invaders_linux
ConfigurationName      :=Debug
WorkspaceConfiguration :=Linux_x64
WorkspacePath          :=/home/archdark/projetos/c/yasic
ProjectPath            :=/home/archdark/projetos/c/yasic/invaders_linux
IntermediateDirectory  :=../build-$(WorkspaceConfiguration)/invaders_linux
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Nicholas Oliveira
Date                   :=03/03/24
CodeLitePath           :=/home/archdark/.codelite
MakeDirCommand         :=mkdir -p
LinkerName             :=/usr/bin/g++-13
SharedObjectLinkerName :=/usr/bin/g++-13 -shared -fPIC
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
OutputDirectory        :=/home/archdark/projetos/c/yasic/build-$(WorkspaceConfiguration)/bin
OutputFile             :=../build-$(WorkspaceConfiguration)/bin/$(ProjectName)_lnx
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
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
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++-13
CC       := /usr/bin/gcc-13
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -Wextra -Wextra -Werror -Wno-unused-function -Wno-switch -Wuninitialized -Wstrict-prototypes $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_invaders_better_main.c$(ObjectSuffix) $(IntermediateDirectory)/up_invaders_better_miniz.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@$(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_invaders_better_main.c$(ObjectSuffix): ../invaders_better/main.c $(IntermediateDirectory)/up_invaders_better_main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/archdark/projetos/c/yasic/invaders_better/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_invaders_better_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_invaders_better_main.c$(DependSuffix): ../invaders_better/main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_invaders_better_main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_invaders_better_main.c$(DependSuffix) -MM ../invaders_better/main.c

$(IntermediateDirectory)/up_invaders_better_main.c$(PreprocessSuffix): ../invaders_better/main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_invaders_better_main.c$(PreprocessSuffix) ../invaders_better/main.c

$(IntermediateDirectory)/up_invaders_better_miniz.c$(ObjectSuffix): ../invaders_better/miniz.c $(IntermediateDirectory)/up_invaders_better_miniz.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/archdark/projetos/c/yasic/invaders_better/miniz.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_invaders_better_miniz.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_invaders_better_miniz.c$(DependSuffix): ../invaders_better/miniz.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_invaders_better_miniz.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_invaders_better_miniz.c$(DependSuffix) -MM ../invaders_better/miniz.c

$(IntermediateDirectory)/up_invaders_better_miniz.c$(PreprocessSuffix): ../invaders_better/miniz.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_invaders_better_miniz.c$(PreprocessSuffix) ../invaders_better/miniz.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


