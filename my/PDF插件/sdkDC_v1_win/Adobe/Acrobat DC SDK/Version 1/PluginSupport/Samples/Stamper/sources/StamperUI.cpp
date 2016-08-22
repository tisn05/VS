/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/ 
/**
\file StamperUI.cpp

 - The Stamper plug-in implements a custom annotation type and
   provides an AVTool with which users can drag out a "Stamp"
   annotation.

 - Implementation of the Stamper plug-in's Stamping tool and
   associated UI elements.

*********************************************************************/

#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#else
#include "SafeResources.cpp"
#endif

#include "PIMain.h" // for gHINSTANCE
#include "StamperUI.h"
#include "StamperAnnotHandler.h"

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

extern ASAtom Stamper_K;
extern ASAtom Annotation_K;

AVToolRec stamperTool;
ASBool gStamperToolSelected;
static AVCursor stamperCursor;
static AVToolButton stamperToolButton;

static AVMenuItem menuItem = NULL;


// UI Callbacks
static AVExecuteProc cbActivateStamperTool;
static AVComputeEnabledProc cbStamperIsEnabled;
static AVComputeMarkedProc cbStamperIsMarked;

// For mac cursor
#define CURSStamperCursor			150

// For windows
#define IDC_CURSOR1                     101
#define IDI_ICON1                       102
#define IDB_BITMAP1                     103
#define IDB_BITMAP2                     104

/*-------------------------------------------------------
	Utility Methods
-------------------------------------------------------*/

/* GetStamperCursor
** ------------------------------------------------------
** */ 
/** @return the cursor for the stamp annotation
 */
AVCursor GetStamperCursor(void)
{
#ifdef MAC_PLATFORM
    NSCursor *stamper = nil;
    CFBundleRef mainBundle = CFBundleGetBundleWithIdentifier(CFSTR("com.adobe.acrobat.sdk.Stamper"));
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    NSURL *resources = (NSURL*)resourcesURL;
    NSString *imagePath = @"StamperIcon.png";
    NSURL *imageURL = [resources URLByAppendingPathComponent:imagePath];
    NSImage *stamperImage = [[NSImage alloc] initWithContentsOfURL:imageURL];
    NSSize stamperImageSize = [stamperImage size];
    stamper = [[NSCursor alloc] initWithImage:stamperImage hotSpot:NSMakePoint((stamperImageSize.width / 2.0), (stamperImageSize.height / 2.0))];
    return (AVCursor)stamper;
#elif WIN_PLATFORM
	return(AVCursor)LoadCursor(gHINSTANCE, MAKEINTRESOURCE(IDC_CURSOR1));
#endif
}

/* GetStamperToolButtonIcon
 ** ------------------------------------------------------
** */ 
/** @return the too button icon
*/
void *GetStamperToolButtonIcon(void)
{

#ifdef MAC_PLATFORM
 
	extern CFBundleRef gPluginBundle;
	AVIconDataRec iconData;
        
    // Find a resource in the plugin bundle by name and type.
   CFURLRef pingURL = CFBundleCopyResourceURL( gPluginBundle, 
                    CFSTR("StamperIcon"), 
                    CFSTR("png"), 
                    NULL );

    ASFile asf = NULL;
    ASPathName aspn = ASFileSysCreatePathName (NULL,ASAtomFromString("CFURLRef"),
                            pingURL, NULL);
    ASFileSysOpenFile(NULL, aspn, ASFILE_READ, &asf);

    ASUns32 dataSize = ASFileGetEOF(asf);

    ASUns8 *data = (ASUns8 *)ASmalloc(dataSize + 1);
    ASFileRead(asf, (char *)data, dataSize);
    ASFileClose(asf);
    
    iconData.dataStm = ASMemStmRdOpen((char *)data, dataSize);
    iconData.eColorFormat = kAVIconColor;

   return AVAppCreateIconBundle6(kAVIconPNG, &iconData, 1); 

#elif WIN_PLATFORM
	return(AVCursor)LoadBitmap(gHINSTANCE, MAKEINTRESOURCE(IDB_BITMAP1));
#endif

}

/* GetStamperMenuItemIcon
** ------------------------------------------------------
** */ 
/** @return the menu item icon
 */
void *GetStamperMenuItemIcon(void)
{
#ifdef MAC_PLATFORM
	//On Mac, use same image as for toolbar button
	extern CFBundleRef gPluginBundle;
	AVIconDataRec iconData;
        
    //Locate resource in plugin bundle by name and type.
   CFURLRef pingURL = CFBundleCopyResourceURL( gPluginBundle, 
                    CFSTR("StamperIcon"), 
                    CFSTR("png"), 
                    NULL );

    ASFile asf = NULL;
    ASPathName aspn = ASFileSysCreatePathName (NULL,ASAtomFromString("CFURLRef"),
                            pingURL, NULL);
    ASFileSysOpenFile(NULL, aspn, ASFILE_READ, &asf);

    ASUns32 dataSize = ASFileGetEOF(asf);

    ASUns8 *data = (ASUns8 *)ASmalloc(dataSize + 1);
    ASFileRead(asf, (char *)data, dataSize);
    ASFileClose(asf);
    
    iconData.dataStm = ASMemStmRdOpen((char *)data, dataSize);
    iconData.eColorFormat = kAVIconColor;

   return AVAppCreateIconBundle6(kAVIconPNG, &iconData, 1); 

#elif WIN_PLATFORM
	return(AVCursor)LoadBitmap(gHINSTANCE, MAKEINTRESOURCE(IDI_ICON1));
#endif

}

/* RedrawCurrentPage
** ------------------------------------------------------
** */ 
/** Redraw current page so that annots redraw.
 **
 ** @see AVPageViewInvalidateRect
 ** @see AVPageViewDrawNow
 */
static ACCB1 ASBool ACCB2 RedrawCurrentPage (AVDoc doc, void* clientData)
{
	AVPageView page = AVDocGetPageView(doc);
	AVPageViewInvalidateRect (page, NULL);
	AVPageViewDrawNow(page);

	return true;
}

/* RedrawAllVisibleAnnots
** ------------------------------------------------------
** */ 
/** Enumerate all documents.
 */
static void RedrawAllVisibleAnnots()
{
	AVAppEnumDocs (RedrawCurrentPage, NULL);
}

/*-------------------------------------------------------
	AVTool Callbacks
-------------------------------------------------------*/

/* ToolGetType
** ------------------------------------------------------
** */ 
/** @return the tool type.
 */
static ACCB1 ASAtom ACCB2 ToolGetType(AVTool tool)
{
	return Stamper_K;
}

/* ToolActivate
** ------------------------------------------------------
** */ 
/** Set persistant global upon activation.
 */
static ACCB1 void ACCB2 ToolActivate(AVTool tool, ASBool persistent)
{
	gStamperToolSelected = persistent;
}

/* ToolDeactivate
** ------------------------------------------------------
** */ 
/** Redraw page deselecting any stamps
 */
static ACCB1 void ACCB2 ToolDeactivate (AVTool tool)
{
	gStamperToolSelected = false;
	RedrawAllVisibleAnnots();
}

/* ToolAdjustCursor
** ------------------------------------------------------
** */ 
/** Check to see if we should adjust cursor
**
** @return true if cursor adjusted, else false
**
** @see AVSysGetModifiers
** @see AVPageViewIsAnnotAtPoint
** @see AVSysSetCursor
*/
static ACCB1 ASBool ACCB2 ToolAdjustCursor(AVTool tool, AVPageView pageView, AVDevCoord x, AVDevCoord y)
{
	PDAnnot hitAnnot;
	ASBool shiftKeyIsDown = ((AVSysGetModifiers() & AV_SHIFT) != 0);

	/* Defer to the annotation, if any */
	if (!shiftKeyIsDown && AVPageViewIsAnnotAtPoint(pageView, x, y, &hitAnnot))
	return false;
#ifdef  WIN_PLATFORM
	AVSysSetCursor(stamperCursor);
#else
    NSCursor* Stamper = (NSCursor *)stamperCursor;
    [Stamper set];
#endif
	return true;
}


/* ToolDoClick
** ------------------------------------------------------
** */ 
/** Handle mouse clicks when the tool is the selected tool
 **
 ** @return true if click handled, else false
 **
 ** @see AVSysGetModifiers
 ** @see AVPageViewIsAnnotAtPoint
 ** @see AVPageViewDragOutNewRect
 ** @see AVAppSetActiveTool
 ** @see AVUndoNew
 ** @see AVPageViewGetAVDoc
 */
static ACCB1 ASBool ACCB2 ToolDoClick(AVTool tool, AVPageView pageView, AVDevCoord xHit, AVDevCoord yHit,
									   AVFlagBits16 flags, AVTCount clickNo)
{
	PDAnnot hitAnnot;
	ASBool shiftKeyIsDown = ((AVSysGetModifiers() & AV_SHIFT) != 0);

	if (!shiftKeyIsDown && AVPageViewIsAnnotAtPoint(pageView, xHit, yHit, &hitAnnot))
		return false;

	AVDevRect resultRect;
	AVPageViewDragOutNewRect (pageView, xHit, yHit, &resultRect);

	PDAnnot annot = CreateStampAnnotationAt(pageView, &resultRect);
	if (PDAnnotIsValid (annot))
	{
		/* If we're not in a persistent mode, select the last active
		** tool now that the user's done with the Stamper.
		*/
		if (!gStamperToolSelected)
			AVAppSetActiveTool(AVAppGetLastActiveTool(), true);
			
		//create an undo record
		StamperUndoDataPtr  undoDataPtr = (StamperUndoDataPtr)ASmalloc(sizeof(StamperUndoDataRec));
		undoDataPtr->annot = annot;
		undoDataPtr->undoType = kCreateStamperAnnot;
		undoDataPtr->pageView = pageView;
		AVUndoNew(AVPageViewGetAVDoc(pageView), &gUndoHandler, undoDataPtr);
	}

	return true;
}

/*-------------------------------------------------------
	UI Callbacks
-------------------------------------------------------*/

/* ActivateStamperTool
** ------------------------------------------------------
** */ 
/** This is the AVExecuteProc associated with the AVMenuItem and AVToolButton
 ** created by the Stamper.
 **
 ** @see AVAppSetActiveTool
 */
static ACCB1 void ACCB2 ActivateStamperTool (void *clientData)
{
	AVAppSetActiveTool (&stamperTool, true);

	// Force redraw
	RedrawAllVisibleAnnots();
}

/* StamperIsEnabled
** ------------------------------------------------------
** */ 
/** Returns true if the user has the required permissions
 ** to edit the document.
 **
 ** @see AVAppGetActiveDoc
 ** @see PDDocGetPermissions
 ** @see AVDocGetPDDoc
 */
static ACCB1 ASBool ACCB2 StamperIsEnabled (void *permRequired)
{
	AVDoc avDoc = AVAppGetActiveDoc();
	if (!avDoc)
		return false;
	else
	{
		PDPerms docPerms = PDDocGetPermissions(AVDocGetPDDoc(avDoc));
        return (!permRequired || (((size_t)permRequired & docPerms) != 0));
	}
}

/* StamperIsMarked
** ------------------------------------------------------
** */ 
/** Returns true to mark the toolbutton as active.
 */
static ACCB1 ASBool ACCB2 StamperIsMarked (void *clientData)
{
	return gStamperToolSelected;
}

/*-------------------------------------------------------
	UI Initialization/Cleanup
-------------------------------------------------------*/

/* SetUpTool
** ------------------------------------------------------
** */ 
/** Sets up the Stamper's AVToolRec.  Note that AVTools
 ** are distinct from, but often invoked by, AVToolButtons.
 */
static void SetUpTool(void)
{
	/* Set up the AVToolRec.  Don't forget to set the size field! */
	memset(&stamperTool, 0, sizeof(AVToolRec));
	stamperTool.size = sizeof(AVToolRec);

	stamperTool.Activate = ASCallbackCreateProto(ActivateProcType, &ToolActivate);
	stamperTool.Deactivate = ASCallbackCreateProto(DeactivateProcType, &ToolDeactivate);
	stamperTool.GetType = ASCallbackCreateProto(GetTypeProcType, &ToolGetType);
	stamperTool.AdjustCursor = ASCallbackCreateProto(AdjustCursorProcType, &ToolAdjustCursor);
	stamperTool.DoClick = ASCallbackCreateProto(DoClickProcType, &ToolDoClick);

	// The stamperIsEnabledCallback is shared between the AVTool,
	// AVToolButton and AVMenuItem; no need to ASCallbackCreate() it
	// three times.

	stamperTool.ComputeEnabled = cbStamperIsEnabled;
	stamperTool.computeEnabledData = NULL;

	AVAppRegisterTool(&stamperTool);
}

/* SetUpToolButton
** ------------------------------------------------------
** */ 
/** Create the tool button.
 **
 ** @see AVAppGetToolBar
 ** @see AVToolBarGetButtonByName
 ** @see AVToolButtonNew
 ** @see AVToolBarAddButton
 */
static void SetUpToolButton(void)
{
	// Insert the Stamper tool button just before the "endToolsGroup"
	// AVToolButton separator.

	void *stamperIcon = GetStamperToolButtonIcon();
	AVToolBar toolBar = AVAppGetToolBar();
	AVToolButton separator = AVToolBarGetButtonByName (toolBar, ASAtomFromString("endToolsGroup"));

	stamperToolButton = AVToolButtonNew (Stamper_K, stamperIcon, true, false);
	AVToolButtonSetExecuteProc (stamperToolButton, cbActivateStamperTool, NULL);
	AVToolButtonSetComputeEnabledProc (stamperToolButton, cbStamperIsEnabled, (void *)pdPermEdit);
	AVToolButtonSetComputeMarkedProc (stamperToolButton, cbStamperIsMarked, NULL);
	AVToolButtonSetHelpText (stamperToolButton, "Stamper Annotations");

	AVToolBarAddButton(toolBar, stamperToolButton, true, separator);
}

/* AddMenuItem
** ------------------------------------------------------
** */ 
/** Adds a menu item that enables the tool. New UI element for 6.0.
 **
 ** @see AVMenubarAcquireMenuByName
 ** @see AVMenuNew
 ** @see AVMenuItemNew
 ** @see AVMenuItemSetExecuteProc
 ** @see AVMenuItemSetComputeEnabledProc
 ** @see AVMenuAddMenuItem
 ** @see AVMenuItemRelease
 ** @see AVMenubarAcquireMenuByName
 ** @see AVMenubarAcquireMenuItemByName
 ** @see AVMenuGetMenuItemIndex
 ** @see AVMenuAddMenuItem
 ** @see AVMenuRelease
 ** @see AVMenuItemRelease
 **
 */
void AddMenuItem(void)
{
	AVMenubar menubar = AVAppGetMenubar();
	AVMenu volatile commonMenu = NULL;
	
	AVIcon menuIcon = GetStamperMenuItemIcon();

	if (!menubar)
		return;

	DURING

		// Acquire the common "Acrobat SDK" menu, creating it if necessary.
		commonMenu = AVMenubarAcquireMenuByName (menubar, "ADBE:Acrobat_SDK");
		if (!commonMenu) {
			commonMenu = AVMenuNew ("Acrobat SDK", "ADBE:Acrobat_SDK", gExtensionID);
			AVMenubarAddMenu(menubar, commonMenu, APPEND_MENU);
		}

	   	// Create our main menuitem and add to the common menu
		menuItem = AVMenuItemNew ("Stamper Annotations", "ADBE:Stamper", NULL, true, NO_SHORTCUT, 0, menuIcon, gExtensionID);
		AVMenuItemSetExecuteProc (menuItem, ASCallbackCreateProto(AVExecuteProc, ActivateStamperTool), NULL);
		AVMenuItemSetComputeEnabledProc (menuItem,
							ASCallbackCreateProto(AVComputeEnabledProc, StamperIsEnabled), (void *)pdPermEdit);
		AVMenuAddMenuItem (commonMenu, menuItem, APPEND_MENUITEM);
		AVMenuItemRelease (menuItem);

		AVMenuRelease (commonMenu);
	HANDLER
		if (commonMenu)
			AVMenuRelease (commonMenu);
	END_HANDLER

}

/* SetUpUI
** ------------------------------------------------------
** */ 
/** Performs all the necessary UI initialization for the
 ** plug-in:
 **
 ** Register for the AVAppDidInitialize notification and
 ** create the Stamper toolbutton.
 **
 ** @see AVAppRegisterNotification
 ** @see ASGetConfiguration
 ** @see ASAtomFromString
 */
void SetUpUI(void)
{
	AVAppRegisterNotification(AVAppDidInitializeNSEL, 0, (char *)SetUpTool, NULL);

	/* Create the execute, computeEnabled, and computeMarked callbacks here
	** because they're shared between the AVTool and AVToolButton.
	*/
	cbActivateStamperTool = ASCallbackCreateProto (AVExecuteProc, &ActivateStamperTool);
	cbStamperIsEnabled = ASCallbackCreateProto (AVComputeEnabledProc, &StamperIsEnabled);
	cbStamperIsMarked = ASCallbackCreateProto (AVComputeMarkedProc, &StamperIsMarked);

	stamperCursor = GetStamperCursor();

	if (ASGetConfiguration(ASAtomFromString("CanEdit")))
		SetUpToolButton();
		
	//add a menu item. For 6.0, tools now have associated menu items that enable the tool
	AddMenuItem();
}

/* CleanUpUI
** ------------------------------------------------------
** */ 
/** Unregister notifications and remove the toolbutton.
 **
 ** @see AVAppUnregisterNotification
 ** @see AVToolButtonDestroy
 ** @see AVMenuItemRemove
 */
void CleanUpUI(void)
{
	AVAppUnregisterNotification(AVAppDidInitializeNSEL, 0, (char *)SetUpTool, NULL);

	if(stamperToolButton)
		AVToolButtonDestroy (stamperToolButton);
		
	// Remove any menuitems we created.
	if (menuItem)
		AVMenuItemRemove(menuItem);

}
