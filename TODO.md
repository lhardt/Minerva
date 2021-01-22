# TODO

## Right now:

- Finish the implementation of all actions

## Important:

- Prevent the click of the 'edit' buttons when i_select is wxNOT_FOUND.
- Clean fields on 'delete'.
- Confirm if any data changed before updating.
- Check if actions are being destructed properly.
- Check if a name is not already picked.
- Decide if ShowData() closes Edits.
- Implement DeleteRows and DeleteCols using the parameter 'pos'
- Check data validity on inserting/updating data.
- Consider alphabetical order on the grids (not all of them. Ex: days).
- Check if we are updating lectures on class, subject and teacher edits.
- Blocked cells show message about the lock's reason.
- Grid cell widths. (maybe already solved)
- Implement the active/inactive toggle.
- Remaining constraints on logic.h.

# Not Crucial

- Implement optional 'total' line on PosIntGrid. Maybe even total_of item. 'TOTAL: 25/25'
- Give better feedback to the user, regarding usage errors / bad input.
- Continuous formation of the timetable.
- Statistics Pane.
- The constraints in Ap. III are numbered. Use those numbers (or rearrange them)
	to use in the code, as an enum. It would be easier to show errors to the user.
	Ex:

	class Elimination {
		ConstraintType type;
		Class   * referencedClasses;
		Teacher * referencedTeachers;
		Subject * referencedSubjects;
		bool    * referencedPeriods;
		int       satisfactionDegreeUnmet; /* maybe */
	}

# Speculative

- Search about installers.
- Search about the acessibility of the colors used,
	and its effects on daltonism and eye strain.

# Things to always have attention on

- Realloc was causing dangling pointers on the School data structure.
- Check if we already use can_delete everywhere.
