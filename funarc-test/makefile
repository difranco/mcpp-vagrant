# Executable name
EXE = funarc

PATH_MC = /vagrant/MC++_1.0
LIB_MC = -L$(PATH_MC)/lib
INC_MC = -I$(PATH_MC)/include
OBJS = funarc.o

# Compilation options
include $(PATH_MC)/src/makeoptions.mk

#####

$(EXE) : $(OBJS)
	$(LINK) $(PROF) $(FLAGS_LINK) -o $(EXE) $(OBJS) $(LIB_MC) $(LIB_PROFIL) $(LIB_FILIB) -llapack
       
funarc.o: funarc.cpp
	$(CPP) -c $(PROF) $(FLAGS_CPP) $(INC_MC) $(INC_PROFIL) $(INC_FILIB) -o funarc.o funarc.cpp

#####

MC :
	(cd $(PATH_MC)/src; make; make install)

#####

clean :
	rm -f $(EXE) $(OBJS) *.out *.eps
