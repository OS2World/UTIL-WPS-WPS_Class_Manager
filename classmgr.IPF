:userdoc.
:docprof toc=12345.
:title.WPS Class Manager v. 0.1

.* htmlsource = classmgr.htm
:h1 res=1 x=left y=top width=100% height=20% group=1 rules=border scroll=vertical titlebar=none.WPS Class Manager v&per. 0&per.1
:link auto reftype=hd res=5.
:font facename=default size=0x0.
:ul compact.
:li.:link reftype=hd res=2.License agreement:elink.
:li.:link reftype=hd res=3.Introduction:elink.
:li.:link reftype=hd res=4.Installation:elink.
:li.:link reftype=hd res=5.Features:elink.
:li.:link reftype=hd res=6.Planned features:elink.
:li.:link reftype=hd res=7.Registration:elink.
:eul.

.* License agreement
:h1 res=2 x=left y=bottom width=100% height=80% group=2 rules=border scroll=vertical titlebar=none.License agreement
:font facename='Helv' size=20x12.
License agreement
:font facename=default size=0x0.
:ul compact.
:li.You are allowed to use this program for testing and evaluation until a next version is available&per.
:li.The program, in its current version, may be installed on as many machines as you like&per.
:li.You are free to distribute the program provided that you include all the files in the original archive without any modification&per.
:li.You are not allowed to sell the program, but can charge
a reasonable amount to cover the cost of the distribution media&per.
:li.Under no circumstances will the author be liable for any loss
or damage that may be derived from the use of the program&per.
:li.WPS Class Manager is the copyrighted property of Alessandro Felice
Cantatore, Bitonto, Bari, Italy&per.
:eul.
.br
:hp2.The use of the program implies acceptance of the conditions stated above!:ehp2.
:p.
Alessandro Cantatore
.br
:link reftype=fn refid=2.acantatore&atsign.tin&per.it:elink.
.br
:link reftype=fn refid=3.alscant&atsign.libero&per.it:elink.
.br
:link reftype=fn refid=4.http&colon.&slr.&slr.space&per.tin&per.it&slr.scienza&slr.acantato:elink.
:p.

.* Introduction
:h1 res=3 x=left y=bottom width=100% height=80% group=2 rules=border scroll=vertical titlebar=none.Introduction
:font facename='Helv' size=20x12.
Introduction
:font facename=default size=0x0.
:p.
This is a small applet for the management of the WPS classes&per.
.br
This program should be used only by advanced OS&slr.2 - eCS users, aware
of the consequences of registering&slr.deregistering WPS classes&per.
.br

.* Installation
:h1 res=4 x=left y=bottom width=100% height=80% group=2 rules=border scroll=vertical titlebar=none.Installation
:font facename='Helv' size=20x12.
Installation
:font facename=default size=0x0.
:p.
The application consists in an executable file, this documentation file and
a REXX installation script&per.
.br
To install the program, unzip the archive in directory of your choice and
double click on the install&per.cmd icon to create a program object in your
Desktop&bsl.Programs&bsl.Utilities folder&per.
.br
If the Utilities folder is not found the object will be created either
in your Programs folder or in your Desktop folder&per.
.br

.* Features
:h1 res=5 x=left y=bottom width=100% height=80% group=2 scroll=vertical titlebar=none rules=border.Features
:font facename='Helv' size=20x12.
Features
:font facename=default size=0x0.
:p.
This program allows&colon.
:ul compact.
:li.to register new WPS classes,
:li.to deregister existing WPS classes,
:li.to deregister not-deregistrable WPS classes, by directly removing
them from the OS2SYS&per.INI data,
:li.to modify the path or the name of the DLLs associated with a WPS class name&per.
:eul.
.br
The program consists in just one dialog window presenting the list of the
currently registered WPS classes sorted alphabetically&per.
.br
The list is divided in two columns&per. On the left there is the WPS class name,
on the right the name of the DLL containing its definition&per.
:p.
The :hp2.Refresh:ehp2. button makes the program re-scan the WPS class list
data updating the window content&per.
.br
When an item in the list is selected the :hp1.Edit :ehp1., the :hp1.Deregister :ehp1.
and the :hp1.Remove :ehp1. buttons are enabled&per.
:p.
The :hp2.Edit:ehp2. button allows to change the name or the path of the DLL
containing the definition of the currently selected class&per.
.br
Once you click on :hp1.Edit :ehp1. (or :hp2.double click:ehp2. on a class item in
the list window) the data of the selected class is written in the
:hp1.Class name :ehp1. and the :hp1.Module name :ehp1. entry fields&per.
.br
After having modified the content of the :hp1.Module name :ehp1. entry field
you must click on the :hp2.Apply:ehp2. button to update the module data&per.
.br
You can click any time on the :hp2.Cancel:ehp2. button to exit
:hp1.Edit mode :ehp1. and leave the class data unmodified&per.
:p.
To :hp2.register an new WPS class:ehp2. enter the class name and the name
of the DLL containing the class definition in the entry fields, click
on the :hp2.Add:ehp2. button and on the :hp1.Ok :ehp1. button in the
confirmation dialog which will as a consequence pop up&per.
:p.
To :hp2.deregister the selected WPS class:ehp2. click on the :hp2.Deregister:ehp2.
button&per. A confirmation dialog will ask you to confirm the deregistration&per.
.br
If the current class cannot be deregistered via the standard
:hp1.WinDeregisterObjectClass :ehp1. API, you can still deregister
it via the :hp2.Remove:ehp2. button which works at a lower level
and allows to remove even the underegistrable classes&per.
:p.
.br

.* Planned features
:h1 res=6 x=left y=bottom width=100% height=80% group=2 rules=border scroll=vertical titlebar=none.Planned features
:font facename='Helv' size=20x12.
Planned features
:font facename=default size=0x0.
:p.
Depending on the user requests (and will to pay for the needed
development costs) I might add the following features&colon.
:ul compact.
:li.ability to work on OS&slr.2 - eCS installations residing on accessible drives
(i&per.e&per. not the current OS installation),
:li.backup&slr.restore current class data,
:li.view the class list sorting by class, dll name and registration order,
:li.automatic deregistration of all classes contained in a given dll
with optional deletion of the dll files,
:li.destruction of all abstract objects of a given class (and their
data stored in the INI files),
:li.other features according to user requests&per.
:eul.
.br

.* Registration
:h1 res=7 x=left y=bottom width=100% height=80% group=2 rules=border scroll=vertical titlebar=none.Registration
:font facename='Helv' size=20x12.
Registration
:font facename=default size=0x0.
:p.
In this stage of development, no program registration is needed&per.
.br
You are free to use this version of the program until a new version is available&per.
.br
.br


.* mailto:acantatore@tin.it
:fn id=2.
:p.The link you selected points to an external resource&per. Click the URL below to launch Netscape Navigator
:p.
:link reftype=launch object='netscape.exe ' data='mailto:acantatore@tin.it'.
mailto&colon.acantatore&atsign.tin&per.it
:elink.
:efn.

.* mailto:alscant@libero.it
:fn id=3.
:p.The link you selected points to an external resource&per. Click the URL below to launch Netscape Navigator
:p.
:link reftype=launch object='netscape.exe ' data='mailto:alscant@libero.it'.
mailto&colon.alscant&atsign.libero&per.it
:elink.
:efn.

.* http://space.tin.it/scienza/acantato
:fn id=4.
:p.The link you selected points to an external resource&per. Click the URL below to launch Netscape Navigator
:p.
:link reftype=launch object='netscape.exe ' data='http://space.tin.it/scienza/acantato'.
http&colon.&slr.&slr.space&per.tin&per.it&slr.scienza&slr.acantato
:elink.
:efn.

:euserdoc.