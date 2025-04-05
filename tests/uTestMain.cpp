#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestPlugin.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTestExt/MockSupportPlugin.h" 

int main( int ac, char** av )
{    
    return RUN_ALL_TESTS( ac, av );
}