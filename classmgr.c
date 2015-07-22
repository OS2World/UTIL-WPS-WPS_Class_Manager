#include "classmgr.h"


// globals ------------------------------------------------------------------

INT iEditItem;
BOOL bEditMode;

// prototypes ---------------------------------------------------------------

MRESULT EXPENTRY dlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID onCtrlNotify(HWND hwnd, ULONG id, ULONG ulNotify, HWND hCtrl);
VOID onCommand(HWND hwnd, ULONG id);
VOID enumClasses(HWND hwnd, BOOL bClean);
VOID editItem(HWND hwnd);
VOID addItem(HWND hwnd);
VOID deregisterItem(HWND hwnd);
VOID removeItem(HWND hwnd);
VOID updateItem(HWND hwnd);
VOID cancelEditItem(HWND hwnd);
INT getSelectedItemData(HWND hwnd, PSZ buf, ULONG cbBuf, PSZ* ppModule);
PWPSCLASSLIST getWPSClassData(HWND hwnd);
BOOL deleteWPSClassData(HWND hwnd, PWPSCLASSLIST pClassList, PSZ pClassName);
VOID showButton(HWND hwnd, ULONG id, BOOL bEnable, BOOL condition);
VOID setEditMode(HWND hwnd, BOOL bEdit);
HWND initHelp(HWND hwnd);
VOID endHelp(HWND hwnd, HWND hHlp);


// definitions --------------------------------------------------------------

#define OPEN_DEFAULT 0

// --------------------------------------------------------------------------

INT main(VOID) {
   HWND hwnd;
   HAB hab;
   HMQ hmq;
   QMSG qmsg;
   HWND hHlp;

   hmq = WinCreateMsgQueue(hab = WinInitialize(0), 0);
   hwnd = WinLoadDlg(HWND_DESKTOP,  // parent
                     NULLHANDLE,    // owner
                     dlgProc,       // dialog window proc
                     NULLHANDLE,    // module handle
                     ID_MAIN,      // dialog template ID
                     NULL);         // application data pointer

   if (hwnd) {
      hHlp = initHelp(hwnd);
      while (WinGetMsg(hab, &qmsg, NULLHANDLE, 0, 0))
         WinDispatchMsg(hab, &qmsg);
      endHelp(hwnd, hHlp);
   }

   WinDestroyWindow(hwnd);
   WinDestroyMsgQueue(hmq);
   WinTerminate(hab);
   return 0;
} /* end main */



MRESULT EXPENTRY dlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         WinSendMsg(hwnd, WM_SETICON,
                    (MPARAM)WinLoadPointer(HWND_DESKTOP, 0, 1), MPVOID);
         enumClasses(hwnd, FALSE);
         break;
      case WM_CONTROL:
         onCtrlNotify(hwnd,  SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), (HWND)mp2);
         break;
      case WM_COMMAND:
         onCommand(hwnd, (ULONG)mp1);
         break;
      case WM_CLOSE:
         WinPostMsg(hwnd, WM_QUIT, 0, 0);
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}



//===========================================================================
// Process notification messages.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// ULONG id       : control ID
// ULONG ulNotify : notify code
// HWND hCtrl     : control handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID onCtrlNotify(HWND hwnd, ULONG id, ULONG ulNotify, HWND hCtrl) {
   SHORT iitem;
   CHAR buf[360];
   switch (id) {
      case LBOX_CLASS:
         if (ulNotify == LN_SELECT) {
            iitem = WinQueryLboxSelectedItem(hCtrl);
            ulNotify = (iitem >= 0) && !bEditMode;
            WinEnableControl(hwnd, BTN_EDIT, ulNotify);
            WinEnableControl(hwnd, BTN_DEREG, ulNotify);
            WinEnableControl(hwnd, BTN_REMOVE, ulNotify);
         } else if (ulNotify == LN_ENTER) {
            WinPostMsg(hwnd, WM_COMMAND, (MPARAM)BTN_EDIT, MPVOID);
         //   onCommand(hwnd, BTN_EDIT);
         } /* endif */
         break;
      case EF_CLASSNAME:
      case EF_MODULENAME:
         if (ulNotify == EN_CHANGE) {
            ulNotify = DlgItemTextLength(hwnd, EF_CLASSNAME)
                       && DlgItemTextLength(hwnd, EF_MODULENAME)
                       && !bEditMode;
            DlgItemEnable(hwnd, BTN_ADD, ulNotify);
         } /* endif */
         break;
   } /* endswitch */
}

//===========================================================================
//
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// ULONG id  : button id
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID onCommand(HWND hwnd, ULONG id) {
   switch (id) {
      case BTN_REFRESH:
         enumClasses(hwnd, TRUE);
         break;
      case BTN_EDIT:
         editItem(hwnd);
         break;
      case BTN_ADD:
         addItem(hwnd);
         break;
      case BTN_DEREG:
         deregisterItem(hwnd);
         break;
      case BTN_REMOVE:
         removeItem(hwnd);
         break;
      case BTN_UPDATE:
         updateItem(hwnd);
         break;
      case BTN_CANCEL:
         cancelEditItem(hwnd);
         break;
//      case BTN_HELP:
//         { HOBJECT ho;
//            ho = WinQueryObject("F:\\dev\\PRJ\\WPSClassMgr\\002\\classmgr.htm");
//         WinOpenObject(ho,
//                       OPEN_DEFAULT, TRUE); }
//         break;
   } /* endswitch */
}


//===========================================================================
// Enumerate the registered classes.
// If bClean is TRUE delete the listbox content first.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID enumClasses(HWND hwnd, BOOL bClean) {
   PWPSCLASSLIST pClassList;
   PWPSCLASSENTRY pClass;
   INT iitem;
   CHAR buf[512];
   CHAR prevItem[CB_CLASSFIELD + 1];
   PBYTE pEnd;
   INT citems = 0;
   HWND hlbox;
   prevItem[0] = prevItem[CB_CLASSFIELD] = 0;
   hlbox = DlgItemHwnd(hwnd, LBOX_CLASS);
   // store the text of the selected item and clean the listbox
   if (bClean) {
      iitem = wLbxItemSelected(hlbox);
      if (iitem >= 0)
         wLbxItemText(hlbox, iitem, CB_CLASSFIELD, prevItem);
      wLbxEmpty(hlbox);
      WinEnableControl(hwnd, BTN_EDIT, FALSE);
      WinEnableControl(hwnd, BTN_DEREG, FALSE);
      WinEnableControl(hwnd, BTN_REMOVE, FALSE);
   } /* endif */
   // get the WPS classes data from OS2SYS.INI
   if (NULL != (pClassList = getWPSClassData(hwnd))) {
      pEnd = (PBYTE)pClassList + pClassList->cbData;
      // loop through the whole class list
      for (pClass = &pClassList->aClass[0];
           (PBYTE)pClass < pEnd;
           pClass = PNEXTCLASS(pClass), ++citems) {
         sprintf(buf, SZ_FORMATSTR,
                 pClass->achClass,
                 pClass->achClass + strlen(pClass->achClass) + 1);
         wLbxItemIns(hlbox, LIT_SORTASCENDING, buf);
      } /* endfor */
   } /* endif */
   free(pClassList);
   // if any items have been inserted
   if (citems) {
      // index of the item to be selected
      iitem = -1;
      // if an item was previously selected get its new index
      if (prevItem[0])
         iitem = wLbxItemTextSearch(hlbox, LSS_PREFIX, LIT_FIRST, prevItem);
      wLbxItemSelect(hlbox, iitem);
   } else {
      WinPostMsg(hwnd, WM_QUIT, MPVOID, MPVOID);
   } /* endif */
}


//===========================================================================
// Allow to change the currently selected item:
// - disable the listbox
// - put the data of the selected item in the entry fields
// - hide the Add and Deregister buttons
// - show the Update and Cancel buttons
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID editItem(HWND hwnd) {
   CHAR buf[360];
   PSZ pModule;
   if (0 <= (iEditItem = getSelectedItemData(hwnd, buf, sizeof(buf), &pModule))) {
      setEditMode(hwnd, TRUE);
      DlgItemTextSet(hwnd, EF_CLASSNAME, buf);
      DlgItemTextSet(hwnd, EF_MODULENAME, pModule);
   } /* endif */
}


//===========================================================================
// Register a new class.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID addItem(HWND hwnd) {
   PWPSCLASSLIST pClassList;
   ULONG cbClassList = 0;
   CHAR className[64];
   CHAR moduleName[CCHMAXPATH];
   CHAR buf[1024];
   DlgItemText(hwnd, EF_CLASSNAME, sizeof(className), className);
   DlgItemText(hwnd, EF_MODULENAME, sizeof(moduleName), moduleName);
   sprintf(buf, SZ_CONFIRMADD, className, moduleName);
   if (NULL != (pClassList = getWPSClassData(hwnd))) {
      cbClassList = pClassList->cbData;
      free(pClassList);
   } /* endif */
   if (userConfirmAction(hwnd, buf)) {
      if (!WinRegisterObjectClass(className, moduleName)) {
         buf[0] = 0;
         if (cbClassList && (NULL != (pClassList = getWPSClassData(hwnd)))) {
            if (cbClassList != pClassList->cbData)
               sprintf(buf, SZERR_ADD2, className, moduleName);
            free(pClassList);
         } /* endif */
         if (!buf[0]) sprintf(buf, SZERR_ADD, className, moduleName);
         notifyError(hwnd, buf);
      } else {
         sprintf(buf, SZ_FORMATSTR, className, moduleName);
         dLbxItemIns(hwnd, LBOX_CLASS, LIT_SORTASCENDING, buf);
      } /* endif */
   } /* endif */
}


//===========================================================================
// Deregister the selected class.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID deregisterItem(HWND hwnd) {
   CHAR achItem[360];
   CHAR buf[1024];
   PSZ pModule;
   INT iitem;
   if (0 <= (iitem = getSelectedItemData(hwnd, achItem, sizeof(achItem), &pModule))) {
      sprintf(buf, SZ_CONFIRMDEL, achItem, pModule);
      if (userConfirmAction(hwnd, buf)) {
         if (!WinDeregisterObjectClass(achItem)) {
            sprintf(buf, SZERR_DEL, achItem, pModule);
            notifyError(hwnd, buf);
         } else {
            dLbxItemDel(hwnd, LBOX_CLASS, iitem);
         } /* endif */
      } /* endif */
   } /* endif */
}


//===========================================================================
// Deregister a WPS class by modifying OS2SYS.INI.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID removeItem(HWND hwnd) {
   CHAR achItem[360];
   CHAR buf[1024];
   PSZ pModule;
   INT iitem;
   PWPSCLASSLIST pClassData;
   // get the WPS classes data from OS2SYS.INI
   if (NULL != (pClassData = getWPSClassData(hwnd))) {
      // get the name of the selected item
      if (0 <= (iitem
             = getSelectedItemData(hwnd, achItem, sizeof(achItem), &pModule))) {
         sprintf(buf, SZ_CONFIRMDEL, achItem, pModule);
         if (userConfirmAction(hwnd, buf)) {
            if (deleteWPSClassData(hwnd, pClassData, achItem)) {
               dLbxItemDel(hwnd, LBOX_CLASS, iitem);
            } /* endif */
         } /* endif */
      } /* endif */
      free(pClassData);
   } /* endif */
}


//===========================================================================
// Update the selected class (deregister it first and then re-register it).
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID updateItem(HWND hwnd) {
   CHAR achClass[260];
   CHAR achModule[260];
   CHAR buf[1024];
   PWPSCLASSLIST pClassList;
   LONG cbClass, cbModule, offPrev;
   PWPSCLASSENTRY pClass, pNextClass, pSrc, pDest;
   PBYTE pEnd;
   BOOL rc = FALSE;
   // get the name of the class to be modified
   cbClass = DlgItemText(hwnd, EF_CLASSNAME, sizeof(achClass), achClass);
   cbModule = DlgItemText(hwnd, EF_MODULENAME, sizeof(achModule), achModule);
   if (cbModule) WinUpper(WinHAB(hwnd), 0, 0, achModule);
   // ask for user confirmation before modifying the selected item
   sprintf(buf, SZ_CONFIRMCHANGE, achClass, achModule);
   if (userConfirmAction(hwnd, buf)) {
      setEditMode(hwnd, FALSE);
      // get the WPS classes data from OS2SYS.INI
      if (NULL != (pClassList = getWPSClassData(hwnd))) {
         pEnd = (PBYTE)pClassList + pClassList->cbData;
         // loop through the whole class list
         for (pClass = &pClassList->aClass[0];
              (PBYTE)pClass < pEnd;
              pClass = PNEXTCLASS(pClass)) {
            // match found
            if (!strcmp(pClass->achClass, achClass)) {
               // update the class data
               offPrev = pClass->offNext;
               pSrc = PNEXTCLASS(pClass);
               pClass->offNext = cbClass + cbModule + 10;
               // the second ULONG is usually the size of the class data
               if (pClass->cbEntry == offPrev - 8)
                  pClass->cbEntry = cbClass + cbModule + 2;
               // shift the data of the other classes to leave space for the new data
               pDest = PNEXTCLASS(pClass);
               memmove(pDest, pSrc, (ULONG)pEnd - (ULONG)pSrc);
               // copy the new data
               memcpy(pClass->achClass, achClass, cbClass + 1);
               memcpy(pClass->achClass + cbClass + 1, achModule, cbModule + 1);
               // update OS2SYS.INI
               pClassList->cbData += pClass->offNext - offPrev;
               if (PrfWriteProfileData(HINI_SYSTEM,
                                       PRF_WPSCLASSESAPP, PRF_WPSCLASSESKEY,
                                       pClassList, pClassList->cbData)) {
                  rc = TRUE;
                  // update the listbox deleting the old item and inserting the new one
                  dLbxItemDel(hwnd, LBOX_CLASS, iEditItem);
                  sprintf(buf, "%-32.32s ³ %s", achClass, achModule);
                  iEditItem = dLbxItemIns(hwnd, LBOX_CLASS, LIT_SORTASCENDING, buf);
                  dLbxItemSelect(hwnd, LBOX_CLASS, iEditItem);
                  DlgItemTextSet(hwnd, EF_CLASSNAME, "");
                  DlgItemTextSet(hwnd, EF_MODULENAME, "");
               } else {
                  notifyError(hwnd, SZERR_UPDATEOS2SYSINI);
               } /* endif */
            } /* endif */
         } /* endfor */
         free(pClassList);
      } /* endif */
   } /* endif */
}


//===========================================================================
//
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID cancelEditItem(HWND hwnd) {
   setEditMode(hwnd, FALSE);
   DlgItemTextSet(hwnd, EF_CLASSNAME, "");
   DlgItemTextSet(hwnd, EF_MODULENAME, "");
}


//===========================================================================
// Get the text of the selected item separating the class name from the
// module name.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// INT : selected item index.
//===========================================================================
static
INT getSelectedItemData(HWND hwnd, PSZ buf, ULONG cbBuf, PSZ* ppModule) {
   INT iItem;
   PSZ p;
   if (0 <= (iItem = dLbxItemSelected(hwnd, LBOX_CLASS))) {
      dLbxItemText(hwnd, LBOX_CLASS, iItem, cbBuf, buf);
      if (ppModule) *ppModule = buf + CB_CLASSFIELD + 3;
      for (p = buf + CB_CLASSFIELD; *p == ' '; --p) ;
      *(p + 1) = 0;
   } /* endif */
   return iItem;
}


//===========================================================================
//
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
// VOID
//===========================================================================
static
PWPSCLASSLIST getWPSClassData(HWND hwnd) {
   PWPSCLASSLIST p;
   ULONG cb = 0x10000;
   if (NULL == (p = malloc(cb))) {
      notifyError(hwnd, SZERR_ALLOC);
      return NULL;
   } /* endif */
   if (!PrfQueryProfileData(HINI_SYSTEM,
                            PRF_WPSCLASSESAPP, PRF_WPSCLASSESKEY, p, &cb)) {
      notifyError(hwnd, SZERR_GETCLASSLISTDATA);
      free(p);
      return NULL;
   } /* endif */
   return p;
}


//===========================================================================
// Deregister a WPS class by removing its data from OS2SYS.INI.
// Parameters --------------------------------------------------------------
// HWND hwnd                : window handle
// PSWPCLASSLIST pClassList : WPS class list data previously retrieved
//                            from OS2SYS.INI.
// PSZ pClassName           : name of the WPS to be removed.
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL deleteWPSClassData(HWND hwnd, PWPSCLASSLIST pClassList, PSZ pClassName) {
   PWPSCLASSENTRY pClass, pNextClass;
   PBYTE pEnd;
   ULONG cbDel;
   pEnd = (PBYTE)pClassList + pClassList->cbData;
   // loop through the whole class list
   for (pClass = &pClassList->aClass[0];
        (PBYTE)pClass < pEnd;
        pClass = (PWPSCLASSENTRY)((PBYTE)pClass + pClass->offNext)) {
      // match found
      if (!strcmp(pClass->achClass, pClassName)) {
         cbDel = pClass->offNext;
         pNextClass = PNEXTCLASS(pClass);
         // remove all the data of the current class
         memmove(pClass, pNextClass, (ULONG)pEnd - (ULONG)pNextClass);
         pClassList->cbData -= cbDel;
         // update OS2SYS.INI
         if (PrfWriteProfileData(HINI_SYSTEM,
                                 PRF_WPSCLASSESAPP, PRF_WPSCLASSESKEY,
                                 pClassList, pClassList->cbData)) {
            return TRUE;
         } else {
            notifyError(hwnd, SZERR_UPDATEOS2SYSINI);
         } /* endif */
      } /* endif */
   } /* endfor */
   return FALSE;
}


//===========================================================================
// Show and enable a button.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID showButton(HWND hwnd, ULONG id, BOOL bEnable, BOOL condition) {
   DlgItemShow(hwnd, id, bEnable);
   DlgItemEnable(hwnd, id, bEnable && condition);
}


//===========================================================================
// Set/unset the edit mode.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// BOOL bEdit : TRUE/FALSE -> edit/non-edit mode.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID setEditMode(HWND hwnd, BOOL bEdit) {
   bEditMode = bEdit;
   WinSetWindowBits(DlgItemHwnd(hwnd, EF_CLASSNAME), QWL_STYLE,
                    ES_READONLY * bEdit, ES_READONLY);
   DlgItemEnable(hwnd, LBOX_CLASS, !bEdit);
   showButton(hwnd, BTN_REFRESH, !bEdit, !bEdit);
   showButton(hwnd, BTN_EDIT, !bEdit,
              (!bEdit && (0 <= dLbxItemSelected(hwnd, LBOX_CLASS))));
   showButton(hwnd, BTN_ADD, !bEdit,
              (DlgItemTextLength(hwnd, EF_CLASSNAME)
               && DlgItemTextLength(hwnd, EF_MODULENAME)));
   showButton(hwnd, BTN_DEREG, !bEdit, !bEdit);
   showButton(hwnd, BTN_REMOVE, !bEdit, !bEdit);
   showButton(hwnd, BTN_UPDATE, bEdit, bEdit);
   showButton(hwnd, BTN_CANCEL, bEdit, bEdit);
}


//===========================================================================
//
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
// VOID
//===========================================================================
static
HWND initHelp(HWND hwnd) {
   HWND hHlp;
   CHAR buf[256];
   HELPINIT hi;
   memset(&hi, 0, sizeof(HELPINIT));
   hi.cb = sizeof(HELPINIT);
   hi.phtHelpTable = PHLPTBLFROMID(HLP_MAIN);
   hi.pszHelpWindowTitle = SZ_TITLE;
   hi.fShowPanelId = CMIC_HIDE_PANEL_ID;
   hi.pszHelpLibraryName = SZFILE_HLP;
   if (!(hHlp = WinCreateHelpInstance(WinQueryAnchorBlock(hwnd), &hi)))
      return NULLHANDLE;
   if (hi.ulReturnCode) {
      WinDestroyHelpInstance(hHlp);
      hHlp = NULLHANDLE;
   } else {
      WinAssociateHelpInstance(hHlp, hwnd);
   } /* endif */
   return hHlp;
}


//===========================================================================
//
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
// VOID
//===========================================================================
static
VOID endHelp(HWND hwnd, HWND hHlp) {
   if (hHlp) {
      WinAssociateHelpInstance(NULLHANDLE, hwnd);
      WinDestroyHelpInstance(hHlp);
   } /* endif */
}


