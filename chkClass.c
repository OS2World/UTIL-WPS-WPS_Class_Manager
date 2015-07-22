/* --------------------------------------------------------------------------
 .c :

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#define INCL_DOSSEMAPHORES
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

#define PRF_WPSCLASSESAPP     "PM_Objects"
#define PRF_WPSCLASSESKEY     "ClassTable"

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
   pEnd = (PBYTE)pClassList + pClassList->cbData;
   for (pClass = &pClassList->aClass[0];
        (PBYTE)pClass < pEnd;
        pClass = PNEXTCLASS(pClass)) {
      printf("- %s (%s)\n",
             pClass->achClass,
             pClass->achClass + strlen(pClass->achClass) + 1);
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
   if ((argc == 1)
       ||
       (argv[1][0] == '-')
       ||
       (argv[1][0] == '/')
       ||
       (argv[1][0] == '?')) {
      printf("Usage: FINDCLASS <className> [iniFile]\n");
      return 0;
   } /* endif */
   hini = (argc > 2) ? PrfOpenProfile(NULLHANDLE, argv[2]) : HINI_SYSTEM;
   if (!hini) {
      printf("Failed to open %s as an INI file\n", argv[2]);
      return 2;
   } /* endif */
   cb = 0x10000;
   if (!m_sysMemAlloc(p, cb)) {
      printf("Allocation error\n");
      return 4;
   } /* endif */
   if (!PrfQueryProfileData(hini, PRF_WPSCLASSESAPP, PRF_WPSCLASSESKEY, p, &cb)) {
      printf("Failed to read the WPS classdata from the ini file\n");
      m_sysMemFree(p);
      return 6;
   } /* endif */
//   listClasses(p);
   findClass(p, argv[1]);
   m_sysMemFree(p);
   return 0;
}
