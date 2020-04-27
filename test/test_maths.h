/*
 * Project Minerva.
 *   test_maths.h - tests for the mathematics utility module.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 */
#ifndef TEST_MATHS_H
#define TEST_MATHS_H


#include "testing.h"
#include "util.h"
#include "maths.h"

bool test_factorial_division(){
	uint64_t p[] =   {0, 1, 2, 3, 4, 5};
	uint64_t t[] =   {0, 1, 0, 2, 3, 5};
	uint64_t ans[] = {1, 1, 2, 3, 4, 1};
	bool lmh_result = true;

	for(int i = 0; i < 6; i++){
		LMH_TEST_EQ(ans[i], factorial_division(p[i], t[i]));
	}
	return lmh_result;
}

bool test_factorial(){
	uint64_t values[] = {0,1,2,3,10};
	uint64_t answers[] =  {1,1,2,6,3628800};

	bool lmh_result = true;

	for(int i = 0; i < 5; ++i){
		LMH_TEST_EQ(answers[i], factorial(values[i]));
	}
	return lmh_result;
}

bool test_order_elements_desc(){
	int i,j;
	int * out;
	bool lmh_result = true;
	int ini_lists[][10] = {
		{-1},
		{0,1,2,3, -1},
		{3,2,1,0, -1},
		{3,1,2,4, 0, -1},
		{8,7,0,5,1,2,4,3,6, -1},
	};

	int ordered[][10] = {
		{-1},
		{3,2,1,0, -1},
		{3,2,1,0, -1},
		{4,3,2,1,0, -1},
		{8,7,6,5,4,3,2,1,0, -1}
	};

	for(i = 0; i < 5; i++){
		out = order_elements_desc(ini_lists[i]);
		for(j = 0; ini_lists[i][j] >= 0; j++){
			LMH_TEST_EQ(ordered[i][j], out[j]);
		}
		LMH_TEST_EQ(ordered[i][j], out[j]);

		free(out);
	}

	return lmh_result;
}

bool test_order_elements_asc(){
	int i,j;
	int * out;
	bool lmh_result = true;
	int ini_lists[][10] = {
		{-1},
		{0,1,2,3, -1},
		{3,2,1,0, -1},
		{3,1,2,4, 0, -1},
		{8,7,0,5,1,2,4,3,6, -1},
	};

	int ordered[][10] = {
		{-1},
		{0,1,2,3, -1},
		{0,1,2,3, -1},
		{0,1,2,3,4, -1},
		{0,1,2,3,4,5,6,7,8, -1}
	};

	for(i = 0; i < 5; i++){
		out = order_elements_asc(ini_lists[i]);
		for(j = 0; ini_lists[i][j] >= 0; j++){
			LMH_TEST_EQ(ordered[i][j], out[j]);
		}
		LMH_TEST_EQ(ordered[i][j], out[j]);

		free(out);
	}

	return lmh_result;
}

bool test_str_list_len(){
	int i = 0;
	const char * const test_cases[][8] = {
		{ "Lorem", "Ipsum", "Dolor", "Sit", "Amet", NULL },
		{ "Lorem", NULL },
		{ NULL }
	};
	int answers[] = {5, 1, 0};
	bool lmh_result = true;

	for(i = 0; i < 3; i++){
		LMH_TEST_EQ(answers[i], str_list_len(test_cases[i]));
	}

	return lmh_result;
}

bool test_int_list_len(){
	int i = 0;
	bool lmh_result = true;
	int lists[][15] = {
		{0,1,2,3,4,5,-1},
		{1,2,5,2,1,51,2,31,-1,2,3,4,5,-2},
		{-3},
		{0,5,-1}
	};
	int answers[] = {6,8,0,2};
	for(i = 0; i < 4; i++){
		LMH_TEST_EQ(answers[i], int_list_len(lists[i]));
	}
	return lmh_result;
}

bool test_non_zero_int_list_count(){
	int i = 0;
	bool lmh_result = true;
	int lists[][20] = {
		{0,1,2,3,0,1,2,3,-1,1,2,3,0,1,2,-1},
		{-1},
		{0,1,-1},
		{0,0,-1},
		{0,1,-2}
	};
	int answers[] = {6,0,1,0,1};
	for(i = 0; i < 5; i++){
		LMH_TEST_EQ(answers[i], non_zero_int_list_count(lists[i]));
	}
	return lmh_result;
}

bool test_are_int_lists_equal(){
	int i = 0;
	bool lmh_result = true;
	int lists1[][15] = {
		{-1},
		{0,1,-1},
		{5,2,-2, 123, 0, -1},
		{5,2,-2, 123, 0, -1}
	};
	int lists2[][15] = {
		{-1},
		{0,1,-1},
		{5,2,-3,99,0,-1},
		{5,2,-3,99,0,-1}
	};
	int lists3[][15] = {
		{1,2,3,-1},
		{2,3,-5},
		{5,1,-3},
		{5,2,3,-3}
	};

	for(i = 0; i < 4; i++){
		LMH_TEST(are_int_lists_equal(lists1[i], lists1[i]));
		LMH_TEST(are_int_lists_equal(lists2[i], lists2[i]));
		LMH_TEST(are_int_lists_equal(lists3[i], lists3[i]));

		LMH_TEST(are_int_lists_equal(lists1[i], lists2[i]));

		LMH_TEST(! are_int_lists_equal(lists1[i], lists3[i]));
		LMH_TEST(! are_int_lists_equal(lists2[i], lists3[i]));
	}
	return lmh_result;
}

bool test_find_first_positive(){
	int i;
	bool lmh_result = true;

	int lists[][15]= {
		{-1},
		{1,0,0, -1},
		{0,1,2, -1},
		{0,0,0,0,0,0,-1},
		{-2, 5, -1},
		{0,0,0,0,0,0,5, -1}
	};
	int answers[] = {-1, 0, 1, -1, -1,6};

	for(i = 0; i < 6; i++){
		LMH_TEST_EQ(answers[i], find_first_positive(lists[i]));
	}
	return lmh_result;

}

bool test_find_last_positive(){
	int i;
	bool lmh_result = true;

	int lists[][15]= {
		{-1},
		{1,0,0, -1},
		{0,1,2, -1},
		{0,0,0,0,0,0,-1},
		{-2, 5, -1},
		{0,0,0,0,0,0,5, -1},
		{0,0,0,4,0,0,5, -1},
	};
	int answers[] = {-1, 0, 2, -1, -1,6,6};

	for(i = 0; i < 7; i++){
		LMH_TEST_EQ(answers[i], find_last_positive(lists[i]));
	}
	return lmh_result;
}

bool test_find_max_int(){
	int i;
	bool lmh_result = true;

	int lists[][15]= {
		{-1},
		{1,0,0, -1},
		{0,1,2, -1},
		{0,0,0,0,0,0,-1},
		{-2, 5, -1},
		{0,0,0,0,0,0,5, -1},
		{0,0,0,4,9,0,5, -1},
	};
	int answers[] = {-1, 0, 2, 0, -1,6,4};

	for(i = 0; i < 7; i++){
		LMH_TEST_EQ(answers[i], find_max_int(lists[i]));
	}
	return lmh_result;

}

bool test_int_list_both_positive_ctr(){
	bool lmh_result = true;
	int i = 0;

	int a[][12] = {
		{1,2,3,4,5,6,7,8, -1},
		{-3,5,2,-1},
		{-9},
		{5,1,1,1,1,3,-1}
	};
	int b[][12] = {
		{8,7,6,5,4,3,2,1, -1},
		{-3, 5,2,-1},
		{-9},
		{5,0,0,0,0,3,-1}
	};
	int answers[] = {8, 0, 0, 2};

	for(i = 0; i < 4; ++i){
		LMH_TEST_EQ(answers[i], int_list_both_positive_ctr(a[i], b[i]));
	}

	return lmh_result;
}

void test_maths_all(){
	bool result = true;
	result = !test_factorial_division() ? false :
			 !test_factorial() ? false :
			 !test_order_elements_desc() ? false :
			 !test_order_elements_asc() ? false :
			 !test_str_list_len()? false:
			 !test_int_list_len()? false:
			 !test_non_zero_int_list_count()? false:
			 !test_are_int_lists_equal()? false:
			 !test_find_first_positive()? false:
			 !test_find_last_positive()? false:
			 !test_find_max_int()? false:
			 !test_int_list_both_positive_ctr()? false:
			 true;

	if(result == false){
		printf("Maths Test suite failed\n");
	} else {
		printf("Maths Test suite passed\n");
	}
}

#endif /* TEST_MATHS_H */
