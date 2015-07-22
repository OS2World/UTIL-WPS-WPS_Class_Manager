//===========================================================================
// classmgr.h :
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _CLASSMGR_H_
   #define _CLASSMGR_H_
   #define INCL_WIN
   #define INCL_GPI
   #define INCL_DOS
   #define INCL_DOSMISC
   #define INCL_DOSMODULEMGR
   #define INCL_DOSFILEMGR
   #define INCL_DOSPROCESS
   #define INCL_DOSNLS
   #define INCL_WINWORKPLACE

   #include <os2.h>
   #include <stdlib.h>
   #include <stdio.h>
   #include <malloc.h>
   #include <string.h>
   #include "captions.h"
   #include "ApiExBase.h"
   #include "ApiExGPI.h"
   #include "ApiExPM.h"

   #ifndef RC_INVOKED
   #endif



// id of dialog resources
#define ID_MAIN                       1

#define TXT_CLASSLIST               101
#define LBOX_CLASS                  102
#define TXT_CLASSNAME               103
#define EF_CLASSNAME                104
#define TXT_MODULENAME              105
#define EF_MODULENAME               106
#define BTN_REFRESH                 107
#define BTN_EDIT                    108
#define BTN_ADD                     109
#define BTN_DEREG                   110
#define BTN_REMOVE                  111
#define BTN_HELP                    112
#define BTN_UPDATE                  113
#define BTN_CANCEL                  114

#define HLP_MAIN     ID_MAIN
#define HLPS_MAIN    ID_MAIN

typedef struct {
   LONG offNext;
   LONG cbEntry;
   CHAR achClass[1];
} WPSCLASSENTRY, * PWPSCLASSENTRY;

#pragma pack(2)
typedef struct {
   USHORT usRes;          // always 0x0065
   USHORT cbData;         // total data size
   ULONG ulRes;           // always 0x00080000
   WPSCLASSENTRY aClass[1];
} WPSCLASSLIST, * PWPSCLASSLIST;
#pragma pack()

#define userConfirmAction(_h_, _msg_) \
   (MBID_OK == (WinMessageBox(HWND_DESKTOP, _h_, _msg_, SZ_CONFIRMTITLE, 0, \
                     MB_OKCANCEL | MB_QUERY | MB_MOVEABLE)))

#define notifyError(_h_, _msg_) \
   (WinMessageBox(HWND_DESKTOP, _h_, _msg_, NULL, 0, MB_MOVEABLE))


#define SZ_FORMATSTR          "%-32.32s ³ %s"
#define CB_CLASSFIELD         32

#define PRF_WPSCLASSESAPP     "PM_Objects"
#define PRF_WPSCLASSESKEY     "ClassTable"
#define SZFILE_HLP            "CLASSMGR.HLP"

#define PNEXTCLASS(_pclass_) \
   ((PWPSCLASSENTRY)((PBYTE)(_pclass_) + (_pclass_)->offNext))

#endif // #ifndef _CLASSMGR_H_
