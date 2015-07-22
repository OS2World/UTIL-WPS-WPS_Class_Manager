/* --------------------------------------------------------------------------
 classlst.c :

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#define INCL_DOSSEMAPHORES
#define INCL_DOSMODULEMGR
#define INCL_DOSERRORS
#define INCL_DOSMISC
#define INCL_DOSNLS
#define INCL_DOSPROCESS
#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// definitions --------------------------------------------------------------

typedef struct {
   LONG offNext;          // offset to the next entry
   LONG reserved;         // flag ???
   CHAR achClass[1];      // class name and DLL
} WPSCLASSENTRY, * PWPSCLASSENTRY;

#pragma pack(2)
typedef struct {
   USHORT usRes;          // always 0x0065
   USHORT cbData;         // total data size
   ULONG ulRes;           // always 0x00080000
   WPSCLASSENTRY aClass[1];
} WPSCLASSLIST, * PWPSCLASSLIST;
#pragma pack()

#define PRF_WPCLSAPP     "PM_Objects"
#define PRF_WPCLSKEY     "ClassTable"

#define PNEXTCLASS(_pclass_) \
   ((PWPSCLASSENTRY)((PBYTE)(_pclass_) + (_pclass_)->offNext))

// allocate/free memory via DosAllocMem/DosFreeMem
#define STDALLOCFLAGS    (PAG_READ | PAG_WRITE | PAG_COMMIT)
#define m_sysMemAlloc(_pBuf_, _cbAlloc_) \
   (!DosAllocMem((PPVOID)&(_pBuf_), (_cbAlloc_), STDALLOCFLAGS))

#define m_sysMemFree(_pBuf_) \
   (DosFreeMem(_pBuf_), NULL)

// prototypes ---------------------------------------------------------------


// global variables ---------------------------------------------------------


VOID listClasses(PWPSCLASSLIST pClassList) {
   PWPSCLASSENTRY pClass;
   PBYTE pEnd;
   PSZ pDll;
   HMODULE hmod;
   CHAR achDll[CCHMAXPATH];

   pEnd = (PBYTE)pClassList + pClassList->cbData;
   for (pClass = &pClassList->aClass[0];
        (PBYTE)pClass < pEnd;
        pClass = PNEXTCLASS(pClass))
   {
      pDll = pClass->achClass + strlen(pClass->achClass) + 1;
      if (!DosQueryModuleHandle(pDll, &hmod))
      {
         if (!DosQueryModuleName(hmod, CCHMAXPATH, achDll))
            pDll = achDll;
      }
      printf("%-25s %-50s\n", pClass->achClass, pDll);
   } /* endfor */
}

BOOL findClass(PWPSCLASSLIST pClassList, PSZ pszClassName) {
   PWPSCLASSENTRY pClass;
   PBYTE pEnd;
   pEnd = (PBYTE)pClassList + pClassList->cbData;
   for (pClass = &pClassList->aClass[0];
        (PBYTE)pClass < pEnd;
        pClass = PNEXTCLASS(pClass)) {
      if (!strcmp(pszClassName, pClass->achClass)) {
         printf("Found: %s (%s)\n",
                 pClass->achClass,
                 pClass->achClass + strlen(pClass->achClass) + 1);
         return TRUE;
      } /* endif */
   } /* endfor */
   printf("Did not find %s\n", pszClassName);
   return FALSE;
}

// entry point --------------------------------------------------------------

INT main(INT argc, PSZ* argv) {
   HINI hini;
   PWPSCLASSLIST p;
   ULONG cb;

   if ((argc > 1)
       &&
       ((argv[1][0] == '-') || (argv[1][0] == '/'))
       &&
       ((argv[1][1] == '?') || (argv[1][1] == 'h') || (argv[1][1] == 'H'))
       &&
       !argv[1][2]
      )
   {
      printf("Usage: CLASSLST [iniFile]\n");
      return 0;
   }
   hini = (argc > 1) ? PrfOpenProfile(NULLHANDLE, argv[1]) : HINI_SYSTEM;
   if (!hini)
   {
      printf("Failed to open %s as an INI file\n", argv[1]);
      return 2;
   }
   cb = 0x10000;
   if (!m_sysMemAlloc(p, cb))
   {
      printf("Allocation error\n");
      return 4;
   }
   if (!PrfQueryProfileData(hini, PRF_WPCLSAPP, PRF_WPCLSKEY, p, &cb))
   {
      printf("Failed to read the WPS classdata from the ini file\n");
      m_sysMemFree(p);
      return 6;
   }
   listClasses(p);
   m_sysMemFree(p);
   return 0;
}
