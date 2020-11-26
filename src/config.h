//
// Created by lukac on 11/16/2020.
//
#ifndef VIRUS_CONFIG_H
#define VIRUS_CONFIG_H
//file for storing all macros, so they'll work everywhere
// This was our problem: "When the compiler compiles xxx.c it only includes xxx.h and the macro is not there. It does not include main.c.
// Each .c file is compiled separately."



//switches
//#define PROD
#define PRODTEST
//#define SEED_FIXED

//values
#define SEED 47923402
#define FILENAME "../testfiles/round3/log12%3A17%3A23.txt"
//which tick switch from dijkstra to flood
#define SWITCH_TICK 0








//------------------------------------------------------------------------
//macro logic goes here if we want to have some

//I'm hiding this here, bc we shouldn't do logging levels with macros, but it this works for *some* area of the code
//debug level [VERBOSE | INFO | WARN | NOTHING]:
#define INFO

#ifdef PROD
#undef PRODTEST
#endif

#ifdef VERBOSE
#define INFO
#endif
#ifdef INFO
#define WARN
#endif
//------------------------------------------------------------------------
#endif //VIRUS_CONFIG_H
