Option Strict Off
Option Explicit On
Module IACBAS
	
	' ADOBE SYSTEMS INCORPORATED
	' Copyright (C) 1994-2003 Adobe Systems Incorporated
	' All rights reserved.
	'
	' NOTICE: Adobe permits you to use, modify, and distribute this file
	' in accordance with the terms of the Adobe license agreement
	' accompanying it. If you have received this file from a source other
	' than Adobe, then your use, modification, or distribution of it
	' requires the prior written permission of Adobe.
	'
	' ---------------------------------------------------------------------
	'
	' iac.bas
	'
	' - This file contains enumerated data types and constants necessary 
	'   for some of the OLE method parameters. It is derived from the C 
	'   header file IAC.h
	
	''******************************* AV Things ****************************** ''
	
	'* AV Application Preferences data type *'
	
	Public Const avpPrefsVersion As Short = 0 ' Int32
	Public Const avpOpenDialogAtStartup As Short = 1 ' boolean
	Public Const avpShowSplashAtStartup As Short = 2 ' boolean
	Public Const avpShowToolBar As Short = 3 ' boolean
	Public Const avpRememberDialogs As Short = 4 ' boolean
	Public Const avpShortMenus As Short = 5 ' boolean
	Public Const avpDefaultOverviewType As Short = 6 ' Int32
	Public Const avpDefaultZoomScale As Short = 7 ' Fixed
	Public Const avpDefaultZoomType As Short = 8 ' Int32
	Public Const avpShowLargeImages As Short = 9 ' boolean
	Public Const avpGreekText As Short = 10 ' boolean
	Public Const avpGreekLevel As Short = 11 ' Int32
	Public Const avpSubstituteFontType As Short = 12 ' Int32
	Public Const avpDoCalibratedColor As Short = 13 ' boolean
	Public Const avpSkipWarnings As Short = 14 ' boolean
	Public Const avpPSLevel As Short = 15 ' Int32
	Public Const avpShrinkToFit As Short = 16 ' boolean
	Public Const avpCaseSensitive As Short = 17 ' boolean
	Public Const avpWholeWords As Short = 18 ' boolean
	Public Const avpNoteColor As Short = 19 ' PDColorValue
	Public Const avpNoteLabel As Short = 20 ' char
	Public Const avpMaxThreadZoom As Short = 21 ' Fixed
	Public Const avpEnablePageCache As Short = 22 ' boolean
	Public Const avpFullScreenColor As Short = 23 ' PDColorValue
	Public Const avpUnused1 As Short = 24 ' obsolete
	Public Const avpMaxPageCacheZoom As Short = 25 ' Fixed: pages whose zoom > this will not be cached
	Public Const avpMinPageCacheTicks As Short = 26 ' Int32: pages which take < this to render will not be cached
	Public Const avpMaxPageCacheBytes As Short = 27 ' Int32: overall constraint on storage used by cache
	Public Const avpUnused2 As Short = 28 ' obsolete
	Public Const avpFullScreenChangeTimeDelay As Short = 29 ' Int32
	Public Const avpFullScreenLoop As Short = 30 ' boolean
	Public Const avpThumbViewScale As Short = 31 ' Fixed
	Public Const avpThumbViewTimeout As Short = 32 ' Int32
	Public Const avpDestFitType As Short = 33 ' char
	Public Const avpDestZoomInherit As Short = 34 ' boolean
	Public Const avpHighlightMode As Short = 35 ' Int32
	Public Const avpDefaultSplitterPos As Short = 36 ' Int32
	Public Const avpUnused3 As Short = 37 ' obsolete
	Public Const avpMaxCosDocCache As Short = 38 ' Int32: max # of bytes of Cos object cache per Cos doc.
	Public Const avpPageUnits As Short = 39 ' PageUnits
	Public Const avpNoteFontName As Short = 40 ' char*
	Public Const avpNoteFontSize As Short = 41 ' Int32
	Public Const avpRecentFile1 As Short = 42 ' char *
	Public Const avpRecentFile2 As Short = 43 ' char *
	Public Const avpRecentFile3 As Short = 44 ' char *
	Public Const avpRecentFile4 As Short = 45 ' char *
	Public Const avpHighlightColor As Short = 46 ' PDColorValue
	Public Const avpFullScreenUseTimer As Short = 47 ' boolean
	Public Const avpAntialiasText As Short = 48 ' boolean
	Public Const avpAntialiasLevel As Short = 49 ' Int16
	Public Const avpPersistentCacheSize As Short = 50 ' Int32
	Public Const avpPersistentCacheFolder As Short = 51 ' ASPathName
	Public Const avpPageViewLayoutMode As Short = 52 ' PDLayoutMode
	Public Const avpSaveAsLinearized As Short = 53 ' boolean
	Public Const avpMaxOpenDocuments As Short = 54 ' Int32
	Public Const avpTextSelectWordOrder As Short = 55 ' Int32
	Public Const avpMarkHiddenPages As Short = 56 ' boolean
	Public Const avpFullScreenTransitionType As Short = 57 ' char*
	Public Const avpFullScreenClick As Short = 58 ' boolean
	Public Const avpFullScreenEscape As Short = 59 ' boolean
	Public Const avpFullScreenCursor As Short = 60 ' Int16
	Public Const avpOpenInPlace As Short = 61 ' boolean
	Public Const avpShowHiddenAnnots As Short = 62 ' boolean
	Public Const avpFullScreenUsePageTiming As Short = 63 ' boolean
	Public Const avpDownloadEntireFile As Short = 64 ' boolean
	Public Const avpEmitHalftones As Short = 65 ' boolean
	Public Const avpShowMenuBar As Short = 66 ' /*boolean*/
	Public Const avpIgnorePageClip As Short = 67 '/*boolean*/
	Public Const avpMinimizeBookmarks As Short = 68 '/* boolean*/
	Public Const avpShowAnnotSequence As Short = 69 '/*boolean*/
	Public Const avpUseLogicalPageNumbers As Short = 70 '/*boolean*/
	Public Const avpASExtensionDigCert As Short = 71 '/*###cannot use via IAC*/
	Public Const avpShowLeftToolBar As Short = 72 '/*boolean*/
	Public Const avpConfirmOpenFile As Short = 73 '/*boolean*/
	Public Const avpNoteLabelEncoding As Short = 74 '/*cannot use via IAC*/
	Public Const avpBookmarkShowLocation As Short = 75 '/*boolean*/
	Public Const avpUseLocalFonts As Short = 76 '/*boolean*/
	Public Const avpCurrCMM As Short = 77 '/*char* */
	Public Const avpBrowserIntegration As Short = 78 '/*boolean*/
	Public Const avpPrintAnnots As Short = 79 '/*boolean */
	Public Const avpSendFarEastFonts As Short = 80 '/*boolean */
	Public Const avpSuppressCSA As Short = 81 '/*boolean */
	
	
	'* AVZoomType -- Variable zoom "verbs", corresponding to View menu items *'
	
	Public Const AVZoomNoVary As Short = 0 ' no variable zoom - use this for XYZ zoom
	Public Const AVZoomFitPage As Short = 1 ' fit page to window
	Public Const AVZoomFitWidth As Short = 2 ' fit page width to window
	Public Const AVZoomFitHeight As Short = 3 ' fit page height to window
	Public Const AVZoomFitVisibleWidth As Short = 4 ' fit visible width to window
	Public Const AVZoomPreferred As Short = 5 '/* use page's preferred zoom */
	
	
	Public Const AV_EXTERNAL_VIEW As Short = 1 ' Open the document with tool bar visible
	Public Const AV_DOC_VIEW As Short = 2 ' Draw the page pane and scrollbars
	Public Const AV_PAGE_VIEW As Short = 4 ' Draw only the page pane
	
	
	'********************************* PD Things ***********************************'
	
	'* PDPageMode -- Variable for how the file opens - bookmarks, thumbnails, full screen, none *'
	
	Public Const PDDontCare As Short = 0
	Public Const PDUseNone As Short = 1
	Public Const PDUseThumbs As Short = 2
	Public Const PDUseBookmarks As Short = 3
	Public Const PDFullScreen As Short = 4
	
	
	'* PDLayoutMode -- Variable for how the file is opened - single page, one column, two column *'
	
	Public Const PDLayoutDontCare As Short = 0
	Public Const PDLayoutSinglePage As Short = 1
	Public Const PDLayoutOneColumn As Short = 2
	Public Const PDLayoutTwoColumnLeft As Short = 3
	Public Const PDLayoutTwoColumnRight As Short = 4
	
	
	'* PDDocFlags -- used for Getting or Setting the flags of a PD doc *'
	
	Public Const PDDocNeedsSave As Short = &H1s '/* document has been modified and needs saving (get/set) */
	Public Const PDDocRequiresFullSave As Short = &H2s '/* document cannot be saved incrementally; must
	' be written with PDSaveFull. (get/set)
	' */
	Public Const PDDocIsModified As Short = &H4s '/* document has been modified internally, such as
	'opening/closing bookmarks and annotations, but
	'not in such a way as to warrant saving.
	'(get only)
	'*/
	Public Const PDDocDeleteOnClose As Short = &H8s '/* document is based on a temporary file which must
	'be deleted when the document is closed or saved.
	'(get/set)
	'*/
	Public Const PDDocWasRepaired As Short = &H10s '/* document was repaired when opened (get only) */
	Public Const PDDocNewMajorVersion As Short = &H20s '/* document major version newer than current (get only) */
	Public Const PDDocNewMinorVersion As Short = &H40s '/* document minor version newer than current (get only) */
	Public Const PDDocOldVersion As Short = &H80s '/* document version older than current (get only) */
	Public Const PDDocSuppressErrors As Short = &H100s '/* don't display errors (get/set) */
	Public Const PDDocIsEmbedded As Short = &H200s '/* document is embedded in a compound doc (OLE, OpenDoc) */
	Public Const PDDocIsLinearized As Short = &H400s '/* document is linearized (get only) */
	Public Const PDDocIsOptimized As Short = &H800s '/* document is optimized */
	
	
	'* PDInsertFlags -- used for inserting pages *'
	
	Public Const PDInsertBookmarks As Short = &H1s '' insert bookmarks as well as pages
	Public Const PDInsertAll As Short = &H1000s '' insert all Catalog and Info dict values as well as pages
	Public Const PDInsertThreads As Short = &H2s '/* insert articles as well */
	
	'* Page specification defines -- use where a page number or range or count is required *'
	Public Const PDBeforeFirstPage As Short = -1 '
	Public Const PDLastPage As Short = -2 '
	Public Const PDAllPages As Short = -3 '
	Public Const PDOddPagesOnly As Short = -4 '
	Public Const PDEvenPagesOnly As Short = -5 '
	
	'* PDSaveFlags -- used for PD-level Save
	'* All undefined flags should be set to zero.
	'* If either PDSaveCollectGarbage or PDSaveCopy are used, PDSaveFull must be used.
	
	
	Public Const PDSaveIncremental As Short = &H0s ' write changes only
	Public Const PDSaveFull As Short = &H1s ' write entire file
	Public Const PDSaveCopy As Short = &H2s ' write copy w'o affecting current state
	Public Const PDSaveLinearized As Short = &H4s ' writes the file linearized for
	' page-served remote (net) access.
	Public Const PDSaveBinaryOK As Short = &H10s ' /* OK to store binary in file */
	Public Const PDSaveCollectGarbage As Short = &H20s ' perform garbage collection on unreferenced objects ''
	
	
	'* Enum for routines that set/get the value of a page's Rotate key. *'
	
	Public Const pdRotate0 As Short = 0
	Public Const pdRotate90 As Short = 90
	Public Const pdRotate180 As Short = 180
	Public Const pdRotate270 As Short = 270
End Module