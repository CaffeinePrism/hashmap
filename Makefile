# Adapted from https://github.com/google/googletest/blob/master/googletest/make/Makefile

GTEST_DIR = ./gtest-1.8.0
USER_DIR = ./src

# CXX = clang++
CPPFLAGS += -isystem $(GTEST_DIR)/include
CXXFLAGS += -std=c++14 -g -Wall -Wextra -pthread

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

TARGETS = FixedHashMap_unittest ComplexityTest

all : $(TARGETS)

clean :
	rm -f $(TARGETS) gtest.a gtest_main.a *.o

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

# gtest_main.o : $(GTEST_SRCS_)
# 	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
#             $(GTEST_DIR)/src/gtest_main.cc
#
# gtest.a : gtest-all.o
# 	$(AR) $(ARFLAGS) $@ $^
# #
# gtest_main.a : gtest-all.o gtest_main.o
# 	$(AR) $(ARFLAGS) $@ $^

# Builds a sample test.  A test should link with either gtest.a or
# gtest_main.a, depending on whether it defines its own main()
# function.

FixedHashMap_unittest.o : $(USER_DIR)/FixedHashMap.hpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/FixedHashMap_unittest.cpp

FixedHashMap_unittest : FixedHashMap_unittest.o gtest-all.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

ComplexityTest.o : $(USER_DIR)/FixedHashMap.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/ComplexityTest.cpp

ComplexityTest : ComplexityTest.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
