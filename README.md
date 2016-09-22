Design Decisions
-----
1. Programming Language

    C\++14. Chosen because data structure classes are usually taught in C++, and chosen because
presumably the point of this problem was to get a code sample and see if you knew how to use implement
a hash map using low-level structures.

2. Data structures

    Since the hash map is a fixed size and does not have to be resized, an array with open addressing
may seem like the ideal choice, but has poor performance due to the likelihood of hash collisions, and the overhead
that results from probing. Buckets of linked-lists offer near O(1) time performance at the cost of memory overhead
for pointers.

3. Template takes type T, but stores and returns values as type T*

    Though slightly awkward, this nicely enforces set()'s requirement of taking pointers as the value. Traditionally,
C++ collections stores values as copies, but as get() and delete() are allowed to return null, this is difficult to
handle in C++ without some kludging (but simple in Java!).

5. delete(key) was renamed to remove(key)

    delete is a reserved keyword in C++

Running
-----
FixedHashMap has no dependencies outside of the C++14 standard library, but tests for it
were written on top of Google Test. Sources for GTest release 1.8.0 has been included so
compilation should succeed if the compilation environment does not have it installed.

Unit tests can be run with:

    make && ./FixedHashMap_unittest

Timing tests, not dependent on GTest were also written and run with the following after `make`:

    ./ComplexityTest

Timing tests also double as a large-scale test, which should segfault if expected values differ from
actual values returned by set()/get()/remove()

Should compilation with make fail for whatever reason, it is possible to compile just the timing tests with g++:

    cd src
    g++ -std=c++14 FixedHashMap.hpp ComplexityTest.cpp -o ComplexityTest
    ./ComplexityTest

Testing was done on ArchLinux running kernel 4.7.2-1-ARCH and gcc version 6.1.1 20160802


Complexity
-----
Space complexity is O(N). Time complexity of this implementation should be O(1) on average, and O(N) in the worst case.
Testing shows that time complexity is indeed O(1), and worst case is not realized as load is not allowed to go past 1.
Interestingly, operations appear to be faster with more buckets.

    [hashmap]$ ./ComplexityTest
    Running with map size = {80,160,320,640}x1000

    Add operations until load = 1:
     80000:     55415941 ns; t/n =  692.699 ns; t/n/load = 692.699 ns
    160000:    120874888 ns; t/n =  755.468 ns; t/n/load = 755.468 ns
    320000:    216255663 ns; t/n =  675.799 ns; t/n/load = 675.799 ns
    640000:    498523091 ns; t/n =  778.942 ns; t/n/load = 778.942 ns
    Get operations with load = 1:
     80000:     31983587 ns; t/n =  399.795 ns; t/n/load = 399.795 ns
    160000:     67214875 ns; t/n =  420.093 ns; t/n/load = 420.093 ns
    320000:    146372024 ns; t/n =  457.413 ns; t/n/load = 457.413 ns
    640000:    309798869 ns; t/n =  484.061 ns; t/n/load = 484.061 ns
    Delete operations with load = 1:
     80000:     49787693 ns; t/n =  622.346 ns; t/n/load = 622.346 ns
    160000:    100631323 ns; t/n =  628.946 ns; t/n/load = 628.946 ns
    320000:    203732182 ns; t/n =  636.663 ns; t/n/load = 636.663 ns
    640000:    384847262 ns; t/n =  601.324 ns; t/n/load = 601.324 ns

    Add operations until load = 0.5:
     80000:     29000956 ns; t/n =  362.512 ns; t/n/load = 725.024 ns
    160000:     68100804 ns; t/n =   425.63 ns; t/n/load = 851.26 ns
    320000:    129516385 ns; t/n =  404.739 ns; t/n/load = 809.477 ns
    640000:    394375679 ns; t/n =  616.212 ns; t/n/load = 1232.42 ns
    Get operations with load = 0.5:
     80000:     26623141 ns; t/n =  332.789 ns; t/n/load = 665.579 ns
    160000:     52533961 ns; t/n =  328.337 ns; t/n/load = 656.675 ns
    320000:     97758301 ns; t/n =  305.495 ns; t/n/load = 610.989 ns
    640000:    185135118 ns; t/n =  289.274 ns; t/n/load = 578.547 ns
    Delete operations with load = 0.5:
     80000:     28112492 ns; t/n =  351.406 ns; t/n/load = 702.812 ns
    160000:     54355390 ns; t/n =  339.721 ns; t/n/load = 679.442 ns
    320000:    116553088 ns; t/n =  364.228 ns; t/n/load = 728.457 ns
    640000:    239857299 ns; t/n =  374.777 ns; t/n/load = 749.554 ns

    Add operations until load = 0.1:
     80000:      7259795 ns; t/n =  90.7474 ns; t/n/load = 907.474 ns
    160000:     14220457 ns; t/n =  88.8779 ns; t/n/load = 888.779 ns
    320000:     30269018 ns; t/n =  94.5907 ns; t/n/load = 945.907 ns
    640000:     61642134 ns; t/n =  96.3158 ns; t/n/load = 963.158 ns
    Get operations with load = 0.1:
     80000:      5417816 ns; t/n =  67.7227 ns; t/n/load = 677.227 ns
    160000:     10741522 ns; t/n =  67.1345 ns; t/n/load = 671.345 ns
    320000:     21308346 ns; t/n =  66.5886 ns; t/n/load = 665.886 ns
    640000:     43087017 ns; t/n =  67.3235 ns; t/n/load = 673.235 ns
    Delete operations with load = 0.1:
     80000:      7748251 ns; t/n =  96.8531 ns; t/n/load = 968.531 ns
    160000:     15115692 ns; t/n =  94.4731 ns; t/n/load = 944.731 ns
    320000:     29189919 ns; t/n =  91.2185 ns; t/n/load = 912.185 ns
    640000:     58998031 ns; t/n =  92.1844 ns; t/n/load = 921.844 ns







Memory leaks
-----
Memory leaks were checked with valgrind using the command:
    `valgrind --leak-check=full ./FixedHashMap_unittest`.


Note that running valgrind on the following program also reports 72,704 bytes still reachable:

    int main() { return 0; }

and thus obviously does not originate from my own code.


Transcript has been provided below for convenience.


    [hashmap]$ valgrind --leak-check=full ./FixedHashMap_unittest    
    ==11035== Memcheck, a memory error detector
    ==11035== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
    ==11035== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
    ==11035== Command: ./FixedHashMap_unittest
    ==11035==
    [==========] Running 7 tests from 1 test case.
    [----------] Global test environment set-up.
    [----------] 7 tests from FixedHashMapTest
    [ RUN      ] FixedHashMapTest.construct
    [       OK ] FixedHashMapTest.construct (43 ms)
    [ RUN      ] FixedHashMapTest.fixedSize
    [       OK ] FixedHashMapTest.fixedSize (20 ms)
    [ RUN      ] FixedHashMapTest.canAddToNewList
    [       OK ] FixedHashMapTest.canAddToNewList (9 ms)
    [ RUN      ] FixedHashMapTest.canGetandRemove
    [       OK ] FixedHashMapTest.canGetandRemove (21 ms)
    [ RUN      ] FixedHashMapTest.doesntLoseNodesOnDelete
    [       OK ] FixedHashMapTest.doesntLoseNodesOnDelete (16 ms)
    [ RUN      ] FixedHashMapTest.canCopyConstruct
    [       OK ] FixedHashMapTest.canCopyConstruct (24 ms)
    [ RUN      ] FixedHashMapTest.canCopyAssign
    [       OK ] FixedHashMapTest.canCopyAssign (24 ms)
    [----------] 7 tests from FixedHashMapTest (172 ms total)

    [----------] Global test environment tear-down
    [==========] 7 tests from 1 test case ran. (229 ms total)
    [  PASSED  ] 7 tests.
    ==11035==
    ==11035== HEAP SUMMARY:
    ==11035==     in use at exit: 72,704 bytes in 1 blocks
    ==11035==   total heap usage: 672 allocs, 671 frees, 144,278 bytes allocated
    ==11035==
    ==11035== LEAK SUMMARY:
    ==11035==    definitely lost: 0 bytes in 0 blocks
    ==11035==    indirectly lost: 0 bytes in 0 blocks
    ==11035==      possibly lost: 0 bytes in 0 blocks
    ==11035==    still reachable: 72,704 bytes in 1 blocks
    ==11035==         suppressed: 0 bytes in 0 blocks
    ==11035== Reachable blocks (those to which a pointer was found) are not shown.
    ==11035== To see them, rerun with: --leak-check=full --show-leak-kinds=all
    ==11035==
    ==11035== For counts of detected and suppressed errors, rerun with: -v
    ==11035== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
