/*
 * Project Minerva.
 *   testing.h - utility definitions for testing.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 */
#ifndef TESTING_H
#define TESTING_H

#define LMH_TEST(x) do{if(!(x)){lmh_result=false;printf("Test failed at %s %d\n", __FILE__, __LINE__);}}while(0)
#define LMH_TEST_EQ(x,y) do{if((x) != (y)){lmh_result=false;printf("Test failed at %s %d\n", __FILE__, __LINE__);}}while(0)



#endif /* TESTING_H */
