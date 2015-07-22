/**/
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

exename = directory()"\CLASSMGR.EXE"
class = "WPProgram";
title = "WPS Class Manager";
location.1 = "<WP_TOOLS>";
location.2 = "<WP_PROGRAMSFOLDER>";
location.3 = "<WP_DESKTOP>";
location.4 = Value("DESKTOP",,"OS2ENVIRONMENT");
setup = "EXENAME="exename";PROGTYPE=PM;STARTUPDIR="directory()";" ||,
        "OBJECTID=<AFC_WPSCLASSMGR>;CCVIEW=NO;NODROP=YES;NOPRINT=YES;";
option = "R";
dirname.1 = "Programs\Utilities";
dirname.2 = "Programs";
dirname.3 = "Desktop";
dirname.4 = "Desktop";
do i = 1 to 4
   if SysCreateObject(class, title, location.i, setup, option) then do
      say " The "title" object has been successfully created in the "
      say " "dirname.i" folder."
      exit
   end /* do */
end /* do */
say "Failed to create the "title" object"
