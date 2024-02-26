##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=invaders_better
ConfigurationName      :=Debug
WorkspaceConfiguration :=Linux_x64
WorkspacePath          :=/home/archdark/projetos/c/yasic
ProjectPath            :=/home/archdark/projetos/c/yasic/invaders_better
IntermediateDirectory  :=../build-$(WorkspaceConfiguration)/invaders_better
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Nicholas Oliveira
Date                   :=26/02/24
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
OutputFile             :=../build-$(WorkspaceConfiguration)/bin/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)DEBUG $(PreprocessorSwitch)DEBUG_PLAYER_UPDATE $(PreprocessorSwitch)PHYSFS_DATAFILES_OLD 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)D:\libs\allegro5.2.9.1-x64-static\include $(IncludeSwitch)D:\projetos\libs\liballegro-5.2.9-git-release-static/include $(IncludeSwitch)/usr/local/lib 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)allegro $(LibrarySwitch)allegro_font $(LibrarySwitch)allegro_ttf $(LibrarySwitch)allegro_audio $(LibrarySwitch)allegro_acodec $(LibrarySwitch)allegro_dialog $(LibrarySwitch)allegro_primitives $(LibrarySwitch)allegro_image 
ArLibs                 :=  "allegro" "allegro_font" "allegro_ttf" "allegro_audio" "allegro_acodec" "allegro_dialog" "allegro_primitives" "allegro_image" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)..\vendor\allegro5.2.9.1-x64-static\lib $(LibraryPathSwitch)..\vendor\liballegro-5.2.9-git-release-static/lib $(LibraryPathSwitch)/usr/local/include 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++-13
CC       := /usr/bin/gcc-13
CXXFLAGS :=   $(Preprocessors)
CFLAGS   := -Wextra -Werror -Wno-unused-function -Wno-switch -Wuninitialized -Wstrict-prototypes -O0 -std=c11 -Wall -gdwarf-2 -Wno-type-limits $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IntermediateDirectory)/miniz.c$(ObjectSuffix) 



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
$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/archdark/projetos/c/yasic/invaders_better/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM main.c

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) main.c

$(IntermediateDirectory)/miniz.c$(ObjectSuffix): miniz.c $(IntermediateDirectory)/miniz.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/archdark/projetos/c/yasic/invaders_better/miniz.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/miniz.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/miniz.c$(DependSuffix): miniz.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/miniz.c$(ObjectSuffix) -MF$(IntermediateDirectory)/miniz.c$(DependSuffix) -MM miniz.c

$(IntermediateDirectory)/miniz.c$(PreprocessSuffix): miniz.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/miniz.c$(PreprocessSuffix) miniz.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


