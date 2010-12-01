##########
## Define Suffixes
##########

.SUFFIXES:	.cpp
.cpp.o:
	${CXX} ${CXXFLAGS} -c -o $@ $<

#OPTCXXFLAGS = -O3 -fsched-spec-load -fPIC -arch x86_64
#LIBFLAGS = -dynamiclib -arch x86_64 -framework JavaVM

# Linking rule
LINK=${CXX} ${CXXFLAGS} -o $@


#######
## R
#######

R_HOME := 		$(shell R RHOME)

## include headers and libraries for R 
RCPPFLAGS := 		$(shell $(R_HOME)/bin/R CMD config --cppflags)
RLDFLAGS := 		$(shell $(R_HOME)/bin/R CMD config --ldflags)
RBLAS := 		$(shell $(R_HOME)/bin/R CMD config BLAS_LIBS)
RLAPACK := 		$(shell $(R_HOME)/bin/R CMD config LAPACK_LIBS)

## include headers and libraries for Rcpp interface classes
RCPPINCL := 		$(shell echo 'Rcpp:::CxxFlags()' | $(R_HOME)/bin/R --vanilla --slave)
RCPPLIBS := 		$(shell echo 'Rcpp:::LdFlags()'  | $(R_HOME)/bin/R --vanilla --slave)

## include headers and libraries for RInside embedding classes
RINSIDEINCL := 		$(shell echo 'RInside:::CxxFlags()' | $(R_HOME)/bin/R --vanilla --slave)
RINSIDELIBS := 		$(shell echo 'RInside:::LdFlags()'  | $(R_HOME)/bin/R --vanilla --slave)

## compiler etc settings used in default make rules
CPPFLAGS := 		-Wall $(shell $(R_HOME)/bin/R CMD config CPPFLAGS)
CXXFLAGS := 		$(RCPPFLAGS) $(RCPPINCL) $(RINSIDEINCL) $(shell $(R_HOME)/bin/R CMD config CXXFLAGS) $(OPTCXXFLAGS)
LDFLAGS	:= 		-s
LDLIBS := 		$(RLDFLAGS) $(RBLAS) $(RLAPACK) $(RCPPLIBS) $(RINSIDELIBS)




########################
## TrackMeNot Exploit ##
########################

tmn_exploit: PSMatrix.o rcluster.o main.o
	${LINK} PSMatrix.o rcluster.o main.o ${LDLIBS}

main.o: main.cpp PSMatrix.h PSMatrix.cpp rcluster.h rcluster.cpp


PSMatrix.o: PSMatrix.h PSMatrix.cpp

rcluster.o: rcluster.h rcluster.cpp
	${CXX} ${CXXFLAGS} -c -g rcluster.cpp

clean:
	rm -f *.o
