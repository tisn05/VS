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

 ColorSelectedBookmarksSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
  Implementation
-------------------------------------------------------*/

/**
  Gets an array containing the selected bookmarks. If bookmark isn't black, turns black. If black, turns it red.
  @testfile SnippetRunnerCookbook.pdf
  @requires pdf with pre-selected bookmarks
  @see AVDocGetBookmarks
  @see PDBookmarkGetColor
  @see PDBookmarkSetColor
*/
void ColorSelectedBookmarksSnip()
{
  //set up colors
  PDColorValueRec blackColor, redColor;
  blackColor.space = redColor.space = PDDeviceRGB;
  blackColor.value[0] = blackColor.value[1] = blackColor.value[2] = 0;
  redColor.value[0] = fixedOne;
  redColor.value[1] = redColor.value[2] = 0;

  CDocument document;
  if(((AVDoc)document) == NULL)
    return;

  PDBookmark *pPDBookmarkArray = NULL;
  //two choices, or both: kAVBookmarkFilterSelected | kAVBookmarkFilterFocus
  ASInt32 numSelected = AVDocGetBookmarks((AVDoc)document, kAVBookmarkFilterSelected, &pPDBookmarkArray);

  for(int i = 0; i< numSelected;i++)
  {
    PDBookmark currentBookmark = pPDBookmarkArray[i];

    PDColorValueRec currentColor;
    PDBookmarkGetColor(currentBookmark, &currentColor);

    //if black, turn red
    if(currentColor.value[0] == 0 &&
      currentColor.value[1] == 0 &&
        currentColor.value[2] == 0)
    {
      PDBookmarkSetColor(currentBookmark, &redColor);
    }
    else
      PDBookmarkSetColor(currentBookmark, &blackColor);
  }

  if(pPDBookmarkArray)
    ASfree(pPDBookmarkArray);
}

SNIPRUN_REGISTER(ColorSelectedBookmarksSnip, "AV Layer:Bookmarks:Color Selected", "Gets an array of the selected bookmarks and if black, turns them red, else turns them black.")

