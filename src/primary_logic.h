/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */
 #ifndef PRIMARY_LOGIC_H
 #define PRIMARY_LOGIC_H

/* PERIOD OVERFLOW RULE:
 *
 * Logic Type: Primary;
 *
 * Simple Explanation:
 *   If teacher A teachers subject D
 *   And class C needs subject D,
 *     If A has 4 free periods and C needs 5 periods of D,
 *       Then A cannot teach D to C.
 *
 * Things to consider:
 * - The 'free' periods are really the size of the intersection btwn free(A) and free(C);
 * - Consider max_per_day in the calculation
 */
bool elim_period_overflow(PrimaryNode * node);

/* VACATION DAY RULE:
 *
 * Logic Type: Secondary;
 *
 * Simple Explanation:
 *   If teacher A needs 1 free day
 *     And now only in day X he has all the periods free,
 *       Then A has the X day as free day.
 *
 * Things to consider:
 * -
 */
bool elim_vacation_day(PrimaryNode * node);

/* TWIN ORDERING RULE:
 *
 * Logic Type: Secondary;
 *
 * Simple Explanation:
 *   If teacher A teaches class C 3 times (L1, L2, L3) a week
 *       Then L2 and L3 can not happen in their first meeting;
 *            L1 and L2 can not happen in their last meeting;
 *            L1 can not happen in their second last meeting;
 *            L3 can not happen in their second meeting;
 *
 * Things to consider:
 * - This creates an order betweek periods;
 * - Therefore it must happen after any user interference (because messing with one period messes with all the other twins);
 * - But it may be reversed as L1.possible = L1.possible U L2.possible U L3.possible.
 * - TODO: IMPORTANT TO ALGORITHM DESIGN:
 *     we may do one of the following:
 *		a) make sure that every constraint applies even to fixed meetings
 *      b) make sure we do not include fixed meetings in the reversal union.
 */
bool elim_twin_ordering(PrimaryNode * node);
bool reverse_twin_ordering(PrimaryNode * node);

/* FIXED MEETING RULE:
 *
 * Logic Type: Secondary;
 *
 * Basic Explanation:
 *   If teacher A teaches class C at period P, in room R
 *       Then A cannot teach any other class at P;
 *            C cannot attend any other lecture at P;
 *		      R cannot be allocated at P by any other meeting.
 *
 *
 * Things to consider:
 * - TODO This algorithm **should** work even if one of the parameters (A,C, or R)
 *        is not set. The exclusion, then, happens by other means.
 */
bool elim_search_fixed_meeting(PrimaryNode * node);
bool elim_fixed_meeting(PrimaryNode * node, int fixed_meeting_index);

/* GOOD ROOM RULE: TODO better name
 *
 * Logic Type: Secondary;
 *
 * Basic Explanation:
 *   If meeting M demands a room with X features & size
 *       Then M cannot happen in any room without those features;
 *
 *
 * Things to consider:
 *  -
 */
bool elim_good_room(PrimaryNode * node);

/* MAX PER DAY RULE:
 *
 * Logic Type: Secondary;
 *
 * Basic Explanation:
 *   If teacher T already teaches Nt classes in a day Dk and his maximum is Nmax:
 *       Then, for any group of non-allocated analogous meetings Mg(M1,M2, ...):
 *			then only more (Nmax-Ntk) meetings in Mg can happen in Dk (otherwise Nt would be bigger Nmax).
 *
 *			Consider the days that thome meetings can happen (D1, D2, ..., Dz)
 *				On D1, just the first (Nmax-Nt1) periods can happen
 *      	    On D2, just the first (Nmax-Nt1-NP(D1)) can happen,
 *      	    On D3, just the first (Nmax-Nt1-(NP(D1) + NP(D2))) can happen.
 *			It also happens in reverse order:
 *				On Dz, just the last (Nmax-Nt1) periods can happen
 *				On Dz-1, just the last (Nmax-Nt1-NP(Dz)) periods can happen
 *				On Dz-2, just the last (Nmax-Nt1-NP(Dz)-NP(Dz-1)) periods can happen
 *
 *			where NP(k) is the number of periods that those meetings can happen in Day K
 * 				(it is obvious that if NP(k) <= Nmax for any k)
 * Things to consider:
 *  -
 */
bool elim_max_per_day(PrimaryNode * node);


/* MAX PER DAY RULE:
 *
 * Logic Type: Secondary;
 *
 * Basic Explanation:
 *   If teacher T already teaches Nt classes in a day Dk and his maximum is Nmax:
 *       Then, for any group of non-allocated analogous meetings Mg(M1,M2, ...):
 *			then only more (Nmax-Ntk) meetings in Mg can happen in Dk (otherwise Nt would be bigger Nmax).
 *
 *			Consider the days that thome meetings can happen (D1, D2, ..., Dz)
 *				On D1, just the first (Nmax-Nt1) periods can happen
 *      	    On D2, just the first (Nmax-Nt1-NP(D1)) can happen,
 *      	    On D3, just the first (Nmax-Nt1-(NP(D1) + NP(D2))) can happen.
 *			It also happens in reverse order:
 *				On Dz, just the last (Nmax-Nt1) periods can happen
 *				On Dz-1, just the last (Nmax-Nt1-NP(Dz)) periods can happen
 *				On Dz-2, just the last (Nmax-Nt1-NP(Dz)-NP(Dz-1)) periods can happen
 *
 *			where NP(k) is the number of periods that those meetings can happen in Day K
 * 				(it is obvious that if NP(k) <= Nmax for any k)
 * Things to consider:
 *  -
 */
 bool elim_max_per_day(PrimaryNode * node);



#endif /* PRIMARY_LOGIC_H */
