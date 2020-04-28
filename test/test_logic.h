/*
 * Project Minerva.
 *   test_logic.h - tests for the logic module.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 */
#ifndef TEST_LOGIC_H
#define TEST_LOGIC_H

#include "types.h"
#include "logic.h"

bool test_detect_class_circular_subordination(){
	bool lmh_result = true;
	/* Dataset 1 */ {
		School sc;
		sc.n_classes = 0;
		LMH_TEST_EQ(false, detect_class_circular_subordination(&sc));
	}
	/* Dataset 2 */ {
		School sc;
		sc.n_classes = 2;
		int a_sub[3] = {0,1,-1};
		Class classes[2] = {
			{
				.name="A",
				.subordinates=a_sub
			}, {
				.name="B",
				.subordinates=NULL
			}
		};
		sc.classes = classes;
		LMH_TEST_EQ(false, detect_class_circular_subordination(&sc));
	}
	/* Dataset 3 */ {
		School sc;
		sc.n_classes = 2;
		int a_sub[3] = {1,0,-1};
		Class classes[2] = {
			{
				.name="A",
				.subordinates=a_sub
			}, {
				.name="B",
				.subordinates=NULL
			}
		};
		sc.classes = classes;

		LMH_TEST_EQ(true, detect_class_circular_subordination(&sc));
	}
	/* Dataset 4 */ {
		School sc;
		sc.n_classes = 2;
		int a_sub[3] = {0,1,-1};
		int b_sub[3] = {1,0,-1};
		Class classes[3] = {
			{
				.name="A",
				.subordinates=a_sub
			}, {
				.name="B",
				.subordinates=b_sub
			}
		};
		sc.classes = classes;
		LMH_TEST_EQ(true, detect_class_circular_subordination(&sc));
	}
	/* Dataset 5 */ {
		School sc;
		sc.n_classes = 3;
		int a_sub[4] = {0,1,0,-1};
		int b_sub[4] = {0,0,0,-1};
		int c_sub[4] = {0,1,0,-1};
		Class classes[3] = {
			{
				.name="A",
				.subordinates=a_sub
			}, {
				.name="B",
				.subordinates=b_sub
			}, {
				.name="C",
				.subordinates=c_sub
			}
		};
		sc.classes = classes;
		LMH_TEST_EQ(false, detect_class_circular_subordination(&sc));
	}
	/* Dataset 6 */ {
		School sc;
		sc.n_classes = 3;
		int a_sub[5] = {0,1,0,-1};
		int b_sub[5] = {0,0,0,-1};
		int c_sub[5] = {0,1,0,-1};
		Class classes[3] = {
			{
				.name="A",
				.subordinates=a_sub
			}, {
				.name="B",
				.subordinates=b_sub
			}, {
				.name="C",
				.subordinates=c_sub
			}
		};
		sc.classes = classes;

		LMH_TEST_EQ(false, detect_class_circular_subordination(&sc));
	}
	/* Dataset 7 */ {
		School sc;
		sc.n_classes = 3;
		int a_sub[5] = {0,1,0,-1};
		int b_sub[5] = {0,0,0,-1};
		int c_sub[5] = {0,1,3,-1};
		Class classes[3] = {
			{
				.name="A",
				.subordinates=a_sub
			}, {
				.name="B",
				.subordinates=b_sub
			}, {
				.name="C",
				.subordinates=c_sub
			}
		};
		sc.classes = classes;
		LMH_TEST_EQ(true, detect_class_circular_subordination(&sc));
	}
	/* Dataset 8 */ {
		School sc;
		sc.n_classes = 3;
		int a_sub[5] = {0,1,0,-1};
		int b_sub[5] = {0,0,5,-1};
		int c_sub[5] = {0,1,0,-1};
		Class classes[3] = {
			{
				.name="A",
				.subordinates=a_sub
			}, {
				.name="B",
				.subordinates=b_sub
			}, {
				.name="C",
				.subordinates=c_sub
			}
		};
		sc.classes = classes;

		LMH_TEST_EQ(true, detect_class_circular_subordination(&sc));
	}
	/* Dataset 9 */ {
		School sc;
		sc.n_classes = 5;
		int a_sub[7] = {0,1,0,0,0,-1};
		int b_sub[7] = {0,0,1,0,0,-1};
		int c_sub[7] = {0,0,0,1,0,-1};
		int d_sub[7] = {0,0,0,0,1,-1};
		int e_sub[7] = {0,0,0,0,0,-1};
		Class classes[6] = {
			{
				.name="A",
				.subordinates=a_sub
			}, {
				.name="B",
				.subordinates=b_sub
			}, {
				.name="C",
				.subordinates=c_sub
			}, {
				.name="D",
				.subordinates=d_sub
			}, {
				.name="E",
				.subordinates=e_sub
			}
		};
		sc.classes = classes;
		LMH_TEST_EQ(false, detect_class_circular_subordination(&sc));
	}
	/* Dataset 9 */ {
		School sc;
		sc.n_classes = 5;
		int a_sub[7] = {0,0,0,0,0,-1};
		int b_sub[7] = {0,0,1,0,0,-1};
		int c_sub[7] = {0,0,0,1,0,-1};
		int d_sub[7] = {0,0,0,0,1,-1};
		int e_sub[7] = {0,1,0,0,0,-1};
		Class classes[6] = {
			{
				.name="A",
				.subordinates=a_sub
			}, {
				.name="B",
				.subordinates=b_sub
			}, {
				.name="C",
				.subordinates=c_sub
			}, {
				.name="D",
				.subordinates=d_sub
			}, {
				.name="E",
				.subordinates=e_sub
			}
		};
		sc.classes = classes;
		LMH_TEST_EQ(true, detect_class_circular_subordination(&sc));
	}

	return lmh_result;
}

void test_logic_all(){
	bool result = true;
	result = !test_detect_class_circular_subordination()? false:true;

	if(result == false){
		printf("Logic Test suite failed\n");
	} else {
		printf("Logic Test suite passed\n");
	}
}

#endif /* TEST_LOGIC_H */
