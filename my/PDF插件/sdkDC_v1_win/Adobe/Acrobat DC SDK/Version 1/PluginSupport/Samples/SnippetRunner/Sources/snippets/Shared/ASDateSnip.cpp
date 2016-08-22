/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 ASDateSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/

/**  This plug-in does the following: 

	<ul>
	<li>	Post a dialog with today's date</li>
	<li>	Post a dialog with a date 4 months in the future and</li>
	<li>		a date 4*31 days in the future</li>
	<li>	Post a dialog with a date 4 years in the future and</li>
	<li>		a date 4*365 days in the future</li>
	</ul>

	<h3>Details</h3>
	The purpose of this snippet is to show how to use the ASDate, ASCalendarTimeSpan,
	and the ASTimeSpan objects. Shows using ASDate and ASTimeSpan APIs to calculate 
	approximate and exact time differences between current time and four months/four 
	years later. The second dialog compares two dates that will be different 
	when any of the next 4 months have less than 31 days. The third dialog dates
	will be different because of the included leap year.
 
 	@testfile none
 	
	@see ASDateNew
	@see ASDateSetToCurrentLocalTime
	@see ASDateDup
	@see ASDateAddCalendarTimeSpan
	@see ASTimeSpanNew
	@see ASTimeSpanSetFromASInt32
	@see ASDateAddTimeSpan
	@see ASDateCompare
	@see ASDateExactDiff
	@see ASTimeSpanGetASInt32
	@see ASDateCalendarDiff
	@see ASDateGetTimeString
	@see ASTimeSpanDestroy
	@see ASDateDestroy
*/
void ASDateSnip()
{
	ASDate date = ASDateNew();
	ASDateSetToCurrentLocalTime(date);

	char buffer [500];
	char* todayString = ASDateGetTimeString(date, kASTimeUniversalH);
	sprintf(buffer, "Today's Date is: %s", todayString);
	Console::displayString(buffer);
	ASfree(todayString);

	ASDate fourMonthsDate = ASDateDup(date);
	//set year, month, day, hour, minute, second
	ASCalendarTimeSpanRec fourMonthsSpan = { 0, 4, 0, 0, 0, 0 };
	ASDateAddCalendarTimeSpan(fourMonthsDate, &fourMonthsSpan);

	ASDate fourX31Date = ASDateDup(date);
	ASTimeSpan fourX31Span = ASTimeSpanNew();
	//set months*days*hours/day*minutes/hour*seconds/min
	ASTimeSpanSetFromASInt32(fourX31Span, 4*31*24*60*60);
	ASDateAddTimeSpan(fourX31Date, fourX31Span);

	//testing to show API, will always be non-zero
	if(ASDateCompare(fourMonthsDate, fourX31Date))
	{	
		ASBool overflow;
		ASTimeSpan diffTimeSpan = ASTimeSpanNew();
		ASDateExactDiff(fourX31Date, fourMonthsDate, diffTimeSpan);
		ASInt32 diff = ASTimeSpanGetASInt32(diffTimeSpan, &overflow);

		ASCalendarTimeSpanRec diffCalendarSpan;
		ASDateCalendarDiff(fourMonthsDate, fourX31Date, &diffCalendarSpan);

		char* fourMonthsString = ASDateGetTimeString(fourMonthsDate, kASTimeUniversalH);
		char* fourX31String = ASDateGetTimeString(fourX31Date, kASTimeUniversalH);
		sprintf(buffer, "4 months from now is either: \r\t%s or \r\t%s \r\r a difference of %d seconds or\
\r\t%d years\r\t%d months\r\t%d days\r\t%d hours\r\t%d minutes\r\t%d seconds", 
				fourMonthsString,
				fourX31String,
				diff, diffCalendarSpan.Year, diffCalendarSpan.Month, diffCalendarSpan.Day,
				diffCalendarSpan.Hour, diffCalendarSpan.Minute, diffCalendarSpan.Second);

		ASTimeSpanDestroy(diffTimeSpan);
		ASfree(fourMonthsString);
		ASfree(fourX31String);
		Console::displayString(buffer);
	}

	ASDate fourYearDate = ASDateDup(date);
	//set year, month, day, hour, minute, second
	ASCalendarTimeSpanRec fourYearSpan = { 4, 0, 0, 0, 0, 0 };
	ASDateAddCalendarTimeSpan(fourYearDate, &fourYearSpan);

	ASDate fourXYearDate = ASDateDup(date);
	ASTimeSpan fourXYearSpan = ASTimeSpanNew();
	//set years*days/year(365)*hours/day*minutes/hour*seconds/min
	ASTimeSpanSetFromASInt32(fourXYearSpan, 4*365*24*60*60);
	ASDateAddTimeSpan(fourXYearDate, fourXYearSpan);

	//testing to show API, will always be non-zero
	if(ASDateCompare(fourYearDate, fourXYearDate))
	{	
		ASBool overflow;
		ASTimeSpan diffTimeSpan = ASTimeSpanNew();
		ASDateExactDiff(fourYearDate, fourXYearDate, diffTimeSpan);
		ASInt32 diff = ASTimeSpanGetASInt32(diffTimeSpan, &overflow);

		ASCalendarTimeSpanRec diffCalendarSpan;
		ASDateCalendarDiff(fourYearDate, fourXYearDate, &diffCalendarSpan);

		char* fourYearString = ASDateGetTimeString(fourYearDate, kASTimeUniversalH);
		char* fourXYearString = ASDateGetTimeString(fourXYearDate, kASTimeUniversalH);

		sprintf(buffer, "4 years from now is either: \r\t%s or \r\t%s \r\r a difference of %d seconds or\
\r\t%d years\r\t%d months\r\t%d days\r\t%d hours\r\t%d minutes\r\t%d seconds",  
				fourYearString,
				fourXYearString,
				diff, diffCalendarSpan.Year, diffCalendarSpan.Month, diffCalendarSpan.Day,
				diffCalendarSpan.Hour, diffCalendarSpan.Minute, diffCalendarSpan.Second);

		ASfree(fourYearString);
		ASfree(fourXYearString);
		ASTimeSpanDestroy(diffTimeSpan);
		Console::displayString(buffer);
	}

	ASDateDestroy(date);
	ASDateDestroy(fourMonthsDate);
	ASDateDestroy(fourX31Date);
	ASDateDestroy(fourYearDate);
	ASDateDestroy(fourXYearDate);

	//don't need to destroy the ASCalendarTimeSpans
	ASTimeSpanDestroy(fourX31Span);
	ASTimeSpanDestroy(fourXYearSpan);
}

SNIPRUN_REGISTER(ASDateSnip, "AS Layer:ASDate", "Shows using ASDate and ASTimeSpan APIs")

