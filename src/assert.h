/*
 * Project Minerva.
 *   assert.h - a set of debugging functions.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains functions to assert preconditions
 * and to learn about bugs as fast as possible.
 */
#ifndef ASSERT_H
#define ASSERT_H

#define LMH_DEBUG


#ifdef  LMH_DEBUG
#	define LMH_RAISE( m ) do{ printf("Assertion Error (F: %s, L: %d, E: %s);\n", __FILE__, __LINE__, m); }while(0)
#	define LMH_ASSERT( c ) do{if(!(c)){LMH_RAISE("Assertion Failed");}}while(0)
#else /* LMH_DEBUG */
#	define LMH_RAISE( m ) do{}while(0)
#	define LMH_ASSERT( c ) do{}while(0)
#endif /* LMH_DEBUG */


#endif /* ASSERT_H*/
