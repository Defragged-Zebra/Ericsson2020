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
//#define PRODTEST
//#define SEED_FIXED

//values
#define SEED 1318360533
#define FILENAME "../testfiles/round2.1/log26188.txt"
//which tick switch from dijkstra to flood
#define SWITCH_TICK 5








//------------------------------------------------------------------------
//macro logic goes here if we want to have some

//I'm hiding this here, bc we shouldn't do logging levels with macros, but it this works for *some* area of the code
//debug level [INFO | WARN | NOTHING]:
#define WARN
#ifdef INFO
#define WARN
#endif
//------------------------------------------------------------------------
#endif //VIRUS_CONFIG_H
