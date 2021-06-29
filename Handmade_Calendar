#include <stdio.h>
#include <math.h>

void calendar (int month, int year, int firstDay, int dayInMonth);

int main()
{	
  // Variable input is separated into the three parts, such as year, month, and first day in a month. 
	int input;
	printf ("按照年、月、起始日輸入 (如：2021021): ");
	scanf ("%d", &input);
	
	int firstDay = input % 10;
	input = input / 10;
	
	int i, month = 0;
	for ( i = 0; i < 2; i++ ) {
		month += (input % 10) * pow(10, i);
		input = input / 10;
	}
	
	int year = 0;
	for ( i = 0; i < 4; i++ ) {
		year += (input % 10) * pow(10, i);
		input = input / 10;
	}
	
	// Check numbers of days in each month 
	int dayInMonth;
	switch (month) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			dayInMonth = 31;
			break;
			
		case 2:
			if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
				dayInMonth = 29;
			else
				dayInMonth = 28;
			break;
			
		default:
			dayInMonth = 30;
			break;
	}
	
	// Exception control.
	if ( !(month >= 1 && month <= 12) ) {
		printf ("月份不符！！");
	} 
	else if ( !(firstDay >= 0 && firstDay <= 6) ) {
		printf ("星期不符！！");
	}
	else { 
		calendar(month, year, firstDay, dayInMonth);
	}
	
	return 0;
}

/* Print out the calendar. */
void calendar (int month, int year, int firstDay, int dayInMonth)
{	
	if ( month == 1 )
		printf ("\t\t      January, %d\n", year);
	else if ( month == 2 )
		printf ("\t\t     February, %d\n", year);
	else if ( month == 3 )
		printf ("\t\t\tMarch, %d\n", year);
	else if ( month == 4 )
		printf ("\t\t\tApril, %d\n", year);
	else if ( month == 5 )
		printf ("\t\t\t  May, %d\n", year);
	else if ( month == 6 )
		printf ("\t\t\t June, %d\n", year);
	else if ( month == 7 )
		printf ("\t\t\t July, %d\n", year);
	else if ( month == 8 )
		printf ("\t\t       August, %d\n", year);
	else if ( month == 9 )
		printf ("\t\t    September, %d\n", year);
	else if ( month == 10 )
		printf ("\t\t      October, %d\n", year);
	else if ( month == 11 )
		printf ("\t\t     November, %d\n", year);
	else if ( month == 12 )
		printf ("\t\t     December, %d\n", year);

	printf ("  SUN  MON  TUE  WED  THU  FRI  SAT\n");
	
	int space, day, column = 0;
  
  // Output the spaces at the left side of the beginning date
	for (space = 0; space <= (firstDay - 1); space++) {
		printf ("     ");
		++column;
	}
  
  // Output the date and control the indents
	for (day = 1; day <= dayInMonth; day++) {
		if (day <= 9) {
			printf ("    %d", day);
    } else {
			printf ("   %d", day);
    }
		++column;
    
    // A new line every 7 days
		if (column % 7 == 0) {
			puts("");
    }
	}
}
