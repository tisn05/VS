Attribute VB_Name = "IACBAS"
Option Explicit

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

Public Const avpPrefsVersion = 0           ' Int32
Public Const avpOpenDialogAtStartup = 1    ' boolean
Public Const avpShowSplashAtStartup = 2    ' boolean
Public Const avpShowToolBar = 3            ' boolean
Public Const avpRememberDialogs = 4        ' boolean
Public Const avpShortMenus = 5             ' boolean
Public Const avpDefaultOverviewType = 6    ' Int32
Public Const avpDefaultZoomScale = 7       ' Fixed
Public Const avpDefaultZoomType = 8        ' Int32
Public Const avpShowLargeImages = 9        ' boolean
Public Const avpGreekText = 10             ' boolean
Public Const avpGreekLevel = 11            ' Int32
Public Const avpSubstituteFontType = 12    ' Int32
Public Const avpDoCalibratedColor = 13     ' boolean
Public Const avpSkipWarnings = 14          ' boolean
Public Const avpPSLevel = 15               ' Int32
Public Const avpShrinkToFit = 16           ' boolean
Public Const avpCaseSensitive = 17         ' boolean
Public Const avpWholeWords = 18            ' boolean
Public Const avpNoteColor = 19             ' PDColorValue
Public Const avpNoteLabel = 20             ' char
Public Const avpMaxThreadZoom = 21         ' Fixed
Public Const avpEnablePageCache = 22       ' boolean
Public Const avpFullScreenColor = 23       ' PDColorValue
Public Const avpUnused1 = 24               ' obsolete
Public Const avpMaxPageCacheZoom = 25      ' Fixed: pages whose zoom > this will not be cached
Public Const avpMinPageCacheTicks = 26     ' Int32: pages which take < this to render will not be cached
Public Const avpMaxPageCacheBytes = 27     ' Int32: overall constraint on storage used by cache
Public Const avpUnused2 = 28               ' obsolete
Public Const avpFullScreenChangeTimeDelay = 29 ' Int32
Public Const avpFullScreenLoop = 30        ' boolean
Public Const avpThumbViewScale = 31        ' Fixed
Public Const avpThumbViewTimeout = 32      ' Int32
Public Const avpDestFitType = 33           ' char
Public Const avpDestZoomInherit = 34       ' boolean
Public Const avpHighlightMode = 35         ' Int32
Public Const avpDefaultSplitterPos = 36    ' Int32
Public Const avpUnused3 = 37               ' obsolete
Public Const avpMaxCosDocCache = 38        ' Int32: max # of bytes of Cos object cache per Cos doc.
Public Const avpPageUnits = 39             ' PageUnits
Public Const avpNoteFontName = 40          ' char*
Public Const avpNoteFontSize = 41          ' Int32
Public Const avpRecentFile1 = 42           ' char *
Public Const avpRecentFile2 = 43           ' char *
Public Const avpRecentFile3 = 44           ' char *
Public Const avpRecentFile4 = 45           ' char *
Public Const avpHighlightColor = 46        ' PDColorValue
Public Const avpFullScreenUseTimer = 47    ' boolean
Public Const avpAntialiasText = 48         ' boolean
Public Const avpAntialiasLevel = 49        ' Int16
Public Const avpPersistentCacheSize = 50   ' Int32
Public Const avpPersistentCacheFolder = 51 ' ASPathName
Public Const avpPageViewLayoutMode = 52    ' PDLayoutMode
Public Const avpSaveAsLinearized = 53      ' boolean
Public Const avpMaxOpenDocuments = 54      ' Int32
Public Const avpTextSelectWordOrder = 55   ' Int32
Public Const avpMarkHiddenPages = 56       ' boolean
Public Const avpFullScreenTransitionType = 57 ' char*
Public Const avpFullScreenClick = 58       ' boolean
Public Const avpFullScreenEscape = 59      ' boolean
Public Const avpFullScreenCursor = 60      ' Int16
Public Const avpOpenInPlace = 61           ' boolean
Public Const avpShowHiddenAnnots = 62      ' boolean
Public Const avpFullScreenUsePageTiming = 63  ' boolean
Public Const avpDownloadEntireFile = 64    ' boolean
Public Const avpEmitHalftones = 65         ' boolean
Public Const avpShowMenuBar = 66            ' /*boolean*/
Public Const avpIgnorePageClip = 67         '/*boolean*/
Public Const avpMinimizeBookmarks = 68    '/* boolean*/
Public Const avpShowAnnotSequence = 69      '/*boolean*/
Public Const avpUseLogicalPageNumbers = 70  '/*boolean*/
Public Const avpASExtensionDigCert = 71     '/*###cannot use via IAC*/
Public Const avpShowLeftToolBar = 72        '/*boolean*/
Public Const avpConfirmOpenFile = 73        '/*boolean*/
Public Const avpNoteLabelEncoding = 74      '/*cannot use via IAC*/
Public Const avpBookmarkShowLocation = 75   '/*boolean*/
Public Const avpUseLocalFonts = 76          '/*boolean*/
Public Const avpCurrCMM = 77                '/*char* */
Public Const avpBrowserIntegration = 78     '/*boolean*/
Public Const avpPrintAnnots = 79            '/*boolean */
Public Const avpSendFarEastFonts = 80       '/*boolean */
Public Const avpSuppressCSA = 81            '/*boolean */


'* AVZoomType -- Variable zoom "verbs", corresponding to View menu items *'

Public Const AVZoomNoVary = 0             ' no variable zoom - use this for XYZ zoom
Public Const AVZoomFitPage = 1            ' fit page to window
Public Const AVZoomFitWidth = 2           ' fit page width to window
Public Const AVZoomFitHeight = 3          ' fit page height to window
Public Const AVZoomFitVisibleWidth = 4    ' fit visible width to window
Public Const AVZoomPreferred = 5             '/* use page's preferred zoom */


Public Const AV_EXTERNAL_VIEW = 1     ' Open the document with tool bar visible
Public Const AV_DOC_VIEW = 2          ' Draw the page pane and scrollbars
Public Const AV_PAGE_VIEW = 4         ' Draw only the page pane


'********************************* PD Things ***********************************'

'* PDPageMode -- Variable for how the file opens - bookmarks, thumbnails, full screen, none *'

Public Const PDDontCare = 0
Public Const PDUseNone = 1
Public Const PDUseThumbs = 2
Public Const PDUseBookmarks = 3
Public Const PDFullScreen = 4


'* PDLayoutMode -- Variable for how the file is opened - single page, one column, two column *'

Public Const PDLayoutDontCare = 0
Public Const PDLayoutSinglePage = 1
Public Const PDLayoutOneColumn = 2
Public Const PDLayoutTwoColumnLeft = 3
Public Const PDLayoutTwoColumnRight = 4


'* PDDocFlags -- used for Getting or Setting the flags of a PD doc *'

Public Const PDDocNeedsSave = &H1           '/* document has been modified and needs saving (get/set) */
Public Const PDDocRequiresFullSave = &H2    '/* document cannot be saved incrementally; must
                                            ' be written with PDSaveFull. (get/set)
                                            ' */
Public Const PDDocIsModified = &H4          '/* document has been modified internally, such as
                                            'opening/closing bookmarks and annotations, but
                                            'not in such a way as to warrant saving.
                                            '(get only)
                                            '*/
Public Const PDDocDeleteOnClose = &H8       '/* document is based on a temporary file which must
                                            'be deleted when the document is closed or saved.
                                            '(get/set)
                                            '*/
Public Const PDDocWasRepaired = &H10        '/* document was repaired when opened (get only) */
Public Const PDDocNewMajorVersion = &H20    '/* document major version newer than current (get only) */
Public Const PDDocNewMinorVersion = &H40    '/* document minor version newer than current (get only) */
Public Const PDDocOldVersion = &H80         '/* document version older than current (get only) */
Public Const PDDocSuppressErrors = &H100    '/* don't display errors (get/set) */
Public Const PDDocIsEmbedded = &H200        '/* document is embedded in a compound doc (OLE, OpenDoc) */
Public Const PDDocIsLinearized = &H400      '/* document is linearized (get only) */
Public Const PDDocIsOptimized = &H800       '/* document is optimized */


'* PDInsertFlags -- used for inserting pages *'

Public Const PDInsertBookmarks = &H1       '' insert bookmarks as well as pages
Public Const PDInsertAll = &H1000          '' insert all Catalog and Info dict values as well as pages
Public Const PDInsertThreads = &H2       '/* insert articles as well */

'* Page specification defines -- use where a page number or range or count is required *'
Public Const PDBeforeFirstPage = -1 '
Public Const PDLastPage = -2      '
Public Const PDAllPages = -3 '
Public Const PDOddPagesOnly = -4 '
Public Const PDEvenPagesOnly = -5 '

'* PDSaveFlags -- used for PD-level Save
'* All undefined flags should be set to zero.
'* If either PDSaveCollectGarbage or PDSaveCopy are used, PDSaveFull must be used.


Public Const PDSaveIncremental = &H0   ' write changes only
Public Const PDSaveFull = &H1          ' write entire file
Public Const PDSaveCopy = &H2          ' write copy w'o affecting current state
Public Const PDSaveLinearized = &H4    ' writes the file linearized for
                                        ' page-served remote (net) access.
Public Const PDSaveBinaryOK = &H10       ' /* OK to store binary in file */
Public Const PDSaveCollectGarbage = &H20  ' perform garbage collection on unreferenced objects ''


'* Enum for routines that set/get the value of a page's Rotate key. *'

Public Const pdRotate0 = 0
Public Const pdRotate90 = 90
Public Const pdRotate180 = 180
Public Const pdRotate270 = 270

