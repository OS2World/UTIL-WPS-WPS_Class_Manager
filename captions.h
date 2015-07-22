//===========================================================================
// .h :
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _CAPTIONS_H_
   #define _CAPTIONS_H_

#define SZ_TITLE             "WPS Class Manager"
#define SZ_CLASSLIST         "Registered ~WPS classes :"
#define SZ_CLASSNAME         "~Class name :"
#define SZ_MODULENAME        "~Module name :"
#define SZ_REFRESH           "Re~fresh"
#define SZ_EDIT              "~Edit"
#define SZ_ADD               "~Add"
#define SZ_DEREG            "~Deregister"
#define SZ_REMOVE           "~Remove"
#define SZ_HELP              "Help"
#define SZ_UPDATE            "~Apply"
#define SZ_CANCEL            "~Cancel"

#define SZ_CONFIRMTITLE      "Please confirm"
#define SZ_CONFIRMADD        "Click on the OK button to register the "\
                             "WPS class:\n%s (%s)\n" \
                             "Click on Cancel to not register it."
#define SZERR_ADD            "Failed to register the WPS class:\n%s (%s)."
#define SZERR_ADD2           "WinRegisterObjectClass(%s, %s)\n"\
                             "returned an error code but the current class "\
                             "data have been added to OS2SYS.INI.\n"\
                             "%s might not yet be available."

#define SZ_CONFIRMDEL        "Click on the OK button to deregister the "\
                             "WPS class:\n%s (%s)\n" \
                             "Click on Cancel to leave it registered."
#define SZERR_DEL            "Failed to deregister the WPS class:\n%s (%s).\n"\
                             "Please use the \"Remove\" button."
#define SZ_CONFIRMCHANGE     "Click on the OK button to modify the "\
                             "WPS class:\n%s (%s)\n" \
                             "Click on Cancel to leave it unchanged."
#define SZERR_CHANGE1        "Failed to modify (deregister) the WPS class:\n"\
                             "%s (%s)."
#define SZERR_CHANGE2        "Failed to modify (re-register) the WPS class:\n"\
                             "%s (%s)."

#define SZERR_ALLOC          "Allocation error."
#define SZERR_GETCLASSLISTDATA  "Failed to get the registered WPS classes "\
                                "list from OS2SYS.INI."

#define SZERR_UPDATEOS2SYSINI "Failed to update the WPS classes data in "\
                              "the OS2SYS.INI file."

#endif // #ifndef _CAPTIONS_H_
